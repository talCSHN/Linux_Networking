#include <stdio.h>
#include <arpa/inet.h>

void main()
{
	const char* addr = "192,168.0.1";
	struct sockaddr_in addr_inet;

	if (inet_aton(addr, &addr_inet.sin_addr)){
		perror("Conversion failed");
	}
	printf("Conversion addr: %#x\n", addr_inet.sin_addr.s_addr);
}
