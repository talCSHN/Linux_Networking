#include <stdio.h>
#include <unistd.h>

void main()
{
	int fds[2], fds2[2];
	char str[] = "who are you";
	char str2[] = "thank you";
	char buf[100];

	pid_t pid;
	pipe(fds), pipe(fds2);
	pid = fork();

	if (pid == 0){
		write(fds[1], str, sizeof(str));
		read(fds2[0], buf, sizeof(buf));
		printf("child: %s\n", buf);
	} else {
		read(fds[0], buf, sizeof(buf));
		printf("parent: %s\n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}

}
