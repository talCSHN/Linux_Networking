#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64	// Time-To-Live. 라우터를몇번거칠수있는지설정

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in mul_addr;
	int time_live = TTL;

	if (argc != 4){
		printf("Usage : %s <GroupIP> <Port> <Message>\n", argv[0]);
		exit(1);
	}

	// 1. UDP소켓생성
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (send_sock == -1){
		error_handling("socket() error);
	}

	// 2. 멀티캐스트그룹주소정보설정
	memset(&mul_addr, 0, sizeof(mul_addr));
	mul_addr.sin_family = AF_INET;
	mul_addr.sin_addr.s_addr = inet_addr(argv[1]);	// 멀티캐스트그룹IP
	mul_addr.sin_port = htons(atoi(argv[2]));	// 포트번호

	// 3. IP_MULTICAST_TTL소켓옵션설정
	// 멀티캐스트패킷의TTL설정
	if(setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live)) == -1){
		error_handling("setsocketopt() error");
	}

	// 4. 메시지전송
	if (sendto(send_sock, argv[3], strlen(argv[3]), 0, (struct sockaddr*)&mul_addr, sizeof(mul_addr)) == -1){
		error_handling("sendto() error");
	}

	printf("Multicast message sent to %s:%s\n", argv[1], argv[2]);
	close(send_sock);

	return 0;
}

void error_handling(char *message)
{
	perror(message);
	exit(1);
}
