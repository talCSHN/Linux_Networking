#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main()
{
	int epfd = epoll_create1(0);	// 인스턴스생성(저장공간)
	struct epoll_event event, events[10];	// epoll event 구조체 선언

	event.events = EPOLLIN;	// 감시할 이벤트(수신)지정
	event.data.fd = STDIN_FILENO;	// 감시할FD저장
	// STDIN_FILENO fd를 epfd에 등록하여 EPOLLIN이벤트 감시
	epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

	printf("키보드입력대기중...\n");
	// epfd안의 fd에 이벤트발생하면그이벤트정보를events배열에넣음.한번에10개까지
	int n = epoll_wait(epfd, events, 10, 5000);	// 5초대기.이벤트발생한수반환
	if(n == 0){
		printf("Timeout\n");
	} else if (n > 0){
		printf("n: %d\n", n);
		char buf[100];
		fgets(buf, sizeof(buf), stdin);
		printf("입력됨: %s", buf);
	}
	
	close(epfd);

	return 0;
}
