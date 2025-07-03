#include <stdio.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys.types.h>
//#include <sys/stat.h>

int main(){

	int fd1, fd2, fd3;

	//fd1 = socket(PF_INET, SOCK_STREAM, 0);
	//fd2 = open("test.dat", O_CREAT|O_WRONLY);
	//fd3 = socket(PF_INET, SOCK_DGRAM, 0);

	fd1 = fileno(stdin);		// 표준입력
	fd2 = fileno(stdout);	// 표준출력
	fd3 = fileno(stderr);	// 표준에러

	printf("fd 1 : %d\n", fd1);
	printf("fd 2 : %d\n", fd2);
	printf("fd 3 : %d\n", fd3);

	close(fd1);
	close(fd2);
	close(fd3);
	
	return 0;
}
