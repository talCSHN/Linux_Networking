#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
int main(int argc, char** argv)
{
	int serv_fd, clnt_fd;
	FILE* fp;
	char buf[BUF_SIZE];
	char webpage[] = "HTTP/1.1 200 OK\r\n"
						"Server:Linux Web Server\r\n"
						"Content-Type: text/html; charset=UTF-8\r\n\r\n"
						"<!DOCTYPE html>\r\n"
						"<html><head><title> My Web Page </title>\r\n"
						"<style>body {background-color: #FFFF00 }</style></head>\r\n"
						"<body><center><h1>Hello World!</h1><br>\r\n"
						"<img src=\"goat.jpg\"></center></body></html>\r\n";
	int read_cnt;

	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_sz = sizeof(clnt_addr);

	if(argc != 2){
		printf("%s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(serv_fd, 5);

	while(1){
		clnt_fd = accept(serv_fd, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
		read(clnt_fd, buf, sizeof(buf));
		// 1. 보니까 클라에서 요청메시지를 두번보냄
		printf("client request msg: \n%s\n", buf);

		// 3. 여기서 두번째 요청메시지받아서 사진파일write
		if (strncmp(buf, "GET /goat.jpg", 13) == 0){
			// 일단 파일 열고
			fp = fopen("goat.jpg", "rb");
			if (fp == NULL){
				perror("fopen");
				exit(1);
			}
			//  응답헤더에 성공메시지 넣어서 보내주고
			char header[] =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: image/jpeg\r\n\r\n";
            write(clnt_fd, header, strlen(header));

			char img_buf[1024];
			int img_fd;
			// 사진파일읽어서 img_buf에 넣어서 보내주면 되겠노
			while((img_fd = fread(img_buf, 1, sizeof(img_buf), fp)) > 0){
				write(clnt_fd, img_buf, img_fd);
			}
			fclose(fp);
		// 2. 일단첫번째 요청메시지 받았을때 else분기로 와서 html응답메시지 write
		// 그럼 일단 화면에 html내용 렌더링되고
		}else{
			write(clnt_fd, webpage, strlen(webpage));
		}
		shutdown(clnt_fd, SHUT_WR);
		close(clnt_fd);
	}

	printf("webpage running\n");
	close(serv_fd);

	return 0;
}

