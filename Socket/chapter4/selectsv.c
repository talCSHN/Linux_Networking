#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t addr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	if (argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error");
	}
	if (listen(serv_sock, 5) == -1){
		error_handling("listen() error");
	}

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while(1){
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
			break;
		}
		if(fd_num == 0) continue;
		for (i = 0; i < fd_max+1; i++){
			if(FD_ISSET(i, &cpy_reads)){
				if(i == serv_sock){
					addr_sz = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock){
						fd_max = clnt_sock;
					}
					printf("connected client: %d \n", clnt_sock);
				}
				else{
					str_len = read(i, buf, BUF_SIZE);
					if(str_len == 0){
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
					}
					else{
						write(i, buf, str_len);
						buf[str_len] = '\0';
						printf("message from client %d: %s\n", i, buf);
						for (int j = 0; j < fd_max + 1; j++){
							if(FD_ISSET(j, &reads)){
								if(j != serv_sock && j != i){
									write(j, buf, str_len);
								}
							}
						}
					}
				}
			}
		}
	}
	close(serv_sock);

	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
}
