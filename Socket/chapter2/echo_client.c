#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
 
int main(int argc, char** argv)
{
	int sock_fd;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	int recv_len;

	if (argc != 3){
		printf("%s <ip> <port> \n", argv[0]);
		exit(1);
	}

	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket() failed");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		close(sock_fd);
		exit(1);
	}
	puts("server connected...");

	while(1){
		fputs("Exit(Q to quit): ", stdout);
		fgets(buffer, BUFFER_SIZE - 1, stdin);
		if(!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n")) break;
		send(sock_fd, buffer, strlen(buffer), 0);
		recv_len = recv(sock_fd, buffer, sizeof(buffer)-1, 0);
		if (recv_len == -1) printf("receive failed");
		buffer[recv_len] = '\0';
		printf("Message from server: %s\n", buffer);
	}
	close(sock_fd);

	return 0;
}
