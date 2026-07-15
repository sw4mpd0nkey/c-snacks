#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    int cfd = accept(fd, (struct sockaddr*)&clientInfo, &clientSize);
    if(cfd == -1) {
        perror("accept");
        close(fd);
        return -1;
    }

    close(cfd);
}