#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd;
	char str[] = "Hello socket\n";

	fd = open("hello.txt", O_CREAT | O_WRONLY);
	if (fd < 0){
		perror("could not open hello.txt");
		exit(1);
	}
	if(write(fd, str, strlen(str) + 1) < 0){
		perror("Failed to write to file");
		exit(1);
	}

	close(fd);
	
	return 0;
}

/*
#include <unistd.h>
ssize_t write(int fd, const void* buf, size_t nbytes);

*/
