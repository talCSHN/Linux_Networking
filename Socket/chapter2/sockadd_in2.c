#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct myin_addr
{
	uint32_t s_addr;
};

struct mysockaddr_in
{
	unsigned short sin_family;
	struct myin_addr sin_addr;
	uint16_t sin_port;
};

void main()
{
	struct mysockaddr_in myaddr;
	const char* serv_ip = "127.0.0.1";
	const char* serv_port = "8080";

	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = PF_INET;
	myaddr.sin_addr.s_addr = inet_addr(serv_ip);
	myaddr.sin_port = htons(atoi(serv_port));

	printf("sin_family: %d\n", myaddr.sin_family);
	printf("sin_addr: %#x\n", myaddr.sin_addr.s_addr);
	printf("sin_port: %#x\n", myaddr.sin_port);

}
