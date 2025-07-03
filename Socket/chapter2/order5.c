#include <stdio.h>
#include <arpa/inet.h>

void main()
{
	struct sockaddr_in addr;
	char* ptr;

	addr.sin_addr.s_addr = htonl(0x100a8c0);
	printf("%#x\n", addr.sin_addr.s_addr);

	ptr = inet_ntoa(addr.sin_addr);
	printf("%s\n", ptr);
}
