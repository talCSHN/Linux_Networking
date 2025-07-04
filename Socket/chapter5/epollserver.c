/* Level Trigger Epoll */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#define MAX_EVENTS 10
int main(int argc, char* argv[])
{
	int server_fd, epfd, client_fd, event_cnt;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	struct epoll_event ev, events[MAX_EVENTS];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));
	if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind error");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 5) == -1){
		perror("Listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	epfd = epoll_create1(0);	// 1. epoll 인스턴스생성(관찰대상등록)
	ev.events = EPOLLIN;	// epoll 이벤트 구조체변수, 배열
	ev.data.fd = server_fd;	// 관찰할fd로 server_fd저장
	/* 2. 서버 fd의 읽기 이벤트를 감지하도록epfd인스턴스에추가 */
	epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

	while(1){
		/* 3. 이벤트발생대기, 인스턴스에 이벤트가발생하면이벤트발생정보를events에 */
		event_cnt = epoll_wait(epfd, events, MAX_EVENTS, -1);	// 이벤트발생수
		if(event_cnt == -1){
			perror("epoll_wait() failed");
			break;
		}
		for (int i = 0; i < event_cnt; i++){
			if (events[i].data.fd == server_fd){
				client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
				ev.events = EPOLLIN;	// 이벤트에읽기이벤트저장
				ev.data.fd = client_fd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);	// epoll인스턴스에추가
				printf("connected client: %d\n", client_fd);
			}
			else{	// 발생한이벤트가클라이언트fd면
				char buffer[1024] = {0};
				int read_len = read(events[i].data.fd, buffer, sizeof(buffer));
				if (read_len <= 0){	// 읽어온게없으면
					epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					close(events[i].data.fd);
					printf("closed client: %d\n", events[i].data.fd);
				} else{	// 에코
					printf("client[%d] %s", events[i].data.fd, buffer);
					write(events[i].data.fd, buffer, read_len);
				}
			}
		}
	}
	return 0;
}
