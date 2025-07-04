#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main()
{
	struct pollfd pfd;	// 감시할 FD와 이벤트 담을 구조체

	pfd.fd = STDIN_FILENO;	// 표준입력등록(0번FD)
	pfd.events = POLLIN;

	printf("Wait 5 seconds...\n");

	int ret = poll(&pfd, 1, 5000);

	if(ret == -1){
		perror("poll error\n");
	} else if(ret == 0){
		printf("timeout\n");
	} else{
		if(pfd.revents & POLLIN){
			char buffer[100];
			fgets(buffer, sizeof(buffer), stdin);
			printf("Entered: %s\n", buffer);
		}
	}








	return 0;
}
