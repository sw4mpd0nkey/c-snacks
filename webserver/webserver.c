#include <string.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


void server_fail(const char* msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main() {

    int port = 8080;
    // struct for the socket data
    struct sockaddr addr = {
        AF_INET,
        ntohs(port),
        0
    };

    // creates an unbound socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (fd < 0) {
        server_fail("failed on socket()");
    }
    
    // assigns a local socket address to a socket, in this case the one we defined previously
    int res = bind(fd, &addr, sizeof(addr));
    if (res) {
        server_fail("failed on biind()");
    }


    // listens for a socket connection and assigns to our socket var
    listen(fd, 10);

    // extacts first connection in the queue of pending connections
    int client_fd = accept(fd, 0, 0);
    // buffer for the file
    char buffer[256] = {0};
    // recieves a mesage from the socket
    recv(client_fd, buffer, 256, 0);


    // GET /file.html ....
    // start 5 bytes ahead of the buffer
    char* file = buffer + 5;
    // set our file pointer to the location of the space
    *strchr(file, ' ') = 0;
    // opens the file from the get request
    int opened_fd = open(file, O_RDONLY);
    // send the file to the requestor
    sendfile(client_fd, opened_fd, 0, 256);
    close(opened_fd);
    close(client_fd);
    close(fd);

    return 0;

}

