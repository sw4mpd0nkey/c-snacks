#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 256
#define PORT 8080
#define BUFF_SIZE 4096

typedef enum {
    STATE_NEW,
    STATE_CONNECTED,
    STATE_DISCONNECTED,
} state_e;

typedef struct {
    int fd;
    state_e state;
    char buffer[4096];
} clientstate_t;

clientstate_t clientStates[MAX_CLIENTS];

void init_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clientStates[i].fd = -1;
        clientStates[i].state = STATE_NEW;
        memset(&clientStates[i].buffer, '\0', BUFF_SIZE);
    }
}

int find_free_slot() {
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clientStates[i].fd == -1) {
            return i;
        }
    }

    // no freespace found
    return -1;
}

int find_slot_by_fd(int fd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientStates[i].fd == fd) {
            return i;
        }
    }
    // fd not found
    return -1;
}


int main() {

    int listen_fd,
        conn_fd,
        num_fds = 1,
        opt = 1,
        free_slot;

    struct sockaddr_in server_addr,
                       client_addr;
    
    socklen_t client_len = sizeof(client_addr);

    struct pollfd fds[MAX_CLIENTS + 1];

    init_clients();

    //create listening socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // set up server struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen with 10 backlog
    if (listen(listen_fd, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    } 

    printf("Server listening on port %d\n", PORT);

    memset(fds, 0, sizeof(fds));
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    num_fds = 1;

    // main loop logic
    while(1) {

        int ii = 1;

        // add active connections to the read set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if(clientStates[i].fd != -1) {
                //offset by 1 for listen_fd
                fds[ii].fd = clientStates[i].fd;
                fds[ii].events = POLLIN;
                ii++;
            }
        }

        // wait for activity on one of the sockets
        int n_events = poll(fds, num_fds, -1);
        if (n_events == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // check for new connections
        if (fds[0].revents && POLLIN) {
            if((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, 
            &client_len)) == -1
            ) {
                perror("accept");
                continue;
            } 

            printf("New connection from %s:%d\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // find free slot for the new connection
            free_slot = find_free_slot();
            if (free_slot == -1) {
                printf("server full: closing new connection\n");
                close(conn_fd);
            } else {
                clientStates[free_slot].fd = conn_fd;
                clientStates[free_slot].state = STATE_CONNECTED;
                num_fds++;
                printf("Slot %d has fd %d\n", free_slot, clientStates[free_slot].fd);
            }

            n_events--;
        }

        // check each client for read/write activity
        // start from 1 to skip listen_fd
        for (int i = 1; i <= num_fds && n_events > 0; i++) {
            if (fds[i].revents && POLLIN) {

                n_events--;
                int fd = fds[i].fd;
                int slot = find_slot_by_fd(fd);

                size_t bytes_read = read(clientStates[i].fd, &clientStates[i].buffer,
                    sizeof(clientStates[i].buffer));

                if (bytes_read <= 0) {
                    close(fd);
                    if (slot == -1) {
                        printf("Tried to close fd that doesnt exist....");
                    } else {
                        clientStates[slot].fd = -1;
                        clientStates[slot].state = STATE_DISCONNECTED;
                        printf("Client discconected or error, fd %d", clientStates[i].fd);
                        num_fds--;
                    }
                    
                } else {
                    printf("Recieved data from client: %s\n", clientStates[i].buffer);
                }
            }
        }
    }

    return 0;
}
