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
	struct ip_mreq join_addr;
	char buf[BUF_SIZE];

	if (argc != 3){
		printf("Usage : %s <GroupIP> <Port>\n", argv[0]);
		exit(1);
	}

	// 1. UDP 소켓생성
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (recv_sock == -1){
		error_handling("socket() error");
	}

	// 2. SO_REUSEADDR 옵션설정(선택적이지만권장)
	// 여러 수신자가동일한 포트 사용할수있도록
	int reuse = 1;
	if (setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
		error_handling("setsockopt(SO_REUSEADDR) error");
	}

	// 3. 수신주소정보설정및bind
	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_addr.sin_port = htons(atoi(argv[2]));

	if (bind(recv_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1){
		error_handling("bind() error");
	}

	// 4. IP_ADD_MEMBERSHIP옵션사용하여멀티캐스트그룹에가입
	join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);	// 가입할그룹IP
	join_addr.imr_interface.s_addr = htonl(INADDR_ANY);	// 그룹에가입할자신의IP주소

	if (setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr)) == -1){
		error_handling("setsockopt() Group Join error");
	}

	printf("Joined multicast group %s on prot %s. Waiting for message...\n", argv[1], argv[2]);

	// 5. 메시지수신
	while(1){
		int str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (str_len < 0) {
			break;
		}
		buf[str_len] = 0;
		printf("Received multicast message: %s\n", buf);
	}
	close(recv_sock);

	return 0;
}
void error_handling(char *message)
{
	perror(message);
}
