#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char** argv)
{
	int serv_fd;
	FILE* fp;
	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_addr;

	if (argc != 3){
		printf("%s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	serv_fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	connect(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while((read_cnt = read(serv_fd, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);

	puts("Received file data");
	write(serv_fd, "Thank you", 10);

	fclose(fp);
	close(serv_fd);
	
	return 0;
}
