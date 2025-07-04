#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int recv_sock;
	struct sockaddr_in recv_addr;
	struct sockaddr_in from_addr;
	socklen_t addr_sz;
	char buf[BUF_SIZE];

	if (argc != 2){
		printf("Usage : %s <Port>\n", argv[0]);
		exit(1);
	}

	// 1. UDP소켓생성
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (recv_sock == -1){
		error_handling("socket() error");
	}

	// 2. 수신주소정보설정
	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 모든IP로부터수신
	recv_addr.sin_port = htons(atoi(argv[1]));	// 모든포트번호

	// 3. 소켓에주소할당(bind)
	if(bind(recv_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1){
		error_handling("bind() error");
		exit(1);
	}
	printf("Waiting for broadcast message on port %s...\n", argv[1]);

	// 4. 메시지수신
	while(1){
		addr_sz = sizeof(from_addr);
		int str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, (struct sockaddr*)&from_addr, &addr_sz);
		if (str_len < 0){
			break;
		}
		buf[str_len] = 0;
		printf("Received message from %s: %s\n", inet_ntoa(from_addr.sin_addr), buf);
	}
	close(recv_sock);

	return 0;
}

void error_handling(char *message)
{
	 perror(message);
    exit(1);
}

