#include <stdio.h>
#include <arpa/inet.h>

void main()
{
	char* addr = "192.168.0.1";
	unsigned int cv_addr = inet_addr(addr);
	if(cv_addr == INADDR_NONE) perror("ERROR");
	printf("address: %#x\n", cv_addr);

	printf("unsigned int: %u\n", sizeof(unsigned int));
	printf("unsigned long: %ld\n", sizeof(unsigned long int));
}
