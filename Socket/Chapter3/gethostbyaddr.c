#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	struct hostent *host;
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	inet_aton(argv[1], &addr.sin_addr);
	host = gethostbyaddr(&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);

	printf("Hostname: %s\n", host -> h_name);

	for(int i = 0; host -> h_addr_list[i]; i++)
		printf("IP: %d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host -> h_addr_list[i]));
		
	return 0;
}
