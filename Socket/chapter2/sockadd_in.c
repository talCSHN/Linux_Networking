#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>

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
	memset(&myaddr, 0, sizeof(myaddr));

	printf("sin_family: %d\n", myaddr.sin_family);
	printf("sin_addr: %d\n", myaddr.sin_addr.s_addr);
	printf("sin_port: %d\n", myaddr.sin_port);
}
