#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid = fork();

	if(pid == 0) exit(0);

	sleep(10);	// Child는 끝났지만  Parent는 기다리는 상태

	return 0;
}
