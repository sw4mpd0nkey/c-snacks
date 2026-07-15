#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef enum {
    PROTO_HELLO,
} proto_type_e;

typedef struct {
    proto_type_e type;
    unsigned short len;
} proto_hdr_t;

void handle_client(int fd) {
    char buf[4096] = {0};
    read(fd, buf, sizeof(proto_hdr_t) + sizeof(int));

    proto_hdr_t *hdr = (proto_hdr_t *)buf;
    hdr->type = ntohl(hdr->type);
    hdr->len = ntohs(hdr->len);

    int *data = (int *) &hdr[1];
    *data = ntohl(*data);

    if(hdr->type != PROTO_HELLO) {
        printf("Protocol mismatch, failing...\n");
        return;
    }

    if(*data != 1) {
        printf("Protocol version mismatch, failing\n");
    }

    printf("server connected, protocol v1.\n");
}

int main() {

    struct sockaddr_in serverInfo = {0};
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = 0;
    serverInfo.sin_port = htons(5555);

    struct sockaddr_in clientInfo = {0};
    int clientSize = 0;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("socket");
        return -1;
    }
    int rv = bind(fd, (const struct sockaddr *)&serverInfo, sizeof(serverInfo));
    
    if(rv == -1) {
        perror("bind");
        close(fd);
        return -1;
    }

    if(listen(fd, 0) == -1) {
        perror("listen");
        close(fd);
        return -1;
    }

    while(1) {
        int cfd = accept(fd, (struct sockaddr*)&clientInfo, &clientSize);
        if(cfd == -1) {
            perror("accept");
            close(fd);
            return -1;
        }

        handle_client(cfd);
        close(cfd);
    }
   return 0; 
}
