#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid = fork();

	if(pid == 0) puts("Hi I'm child process");
	else {
		printf("child process ID: %d\n", pid);
		sleep(30);
	}

	if(pid == 0) puts("end child process");
	else puts("end parent process");
	
	return 0;
}
