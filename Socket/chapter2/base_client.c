#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	int sock_fd;
	struct sockaddr_in server_addr;
	char buf[1024];
	int recv_len;

	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		close(sock_fd);
		exit(1);
	}
	recv_len = recv(sock_fd, buf, sizeof(buf) - 1, 0);
	if (recv_len == -1) printf("receive failed");
	buf[recv_len] = '\0';
	printf("Message from server: %s %ld %c\n", buf, sizeof(buf), buf[recv_len - 1]);

	close(sock_fd);

	return 0;
}
