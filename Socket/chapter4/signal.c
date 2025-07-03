#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
	if(sig == SIGALRM) puts("timeout");
	alarm(2);
}
void keycontrol(int sig){
	if(sig == SIGINT) puts("CTRL+C pressed");
}
void main()
{
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);	// alarm(2)호출- 2초뒤 SIGALRM 발생

	for(int i = 0; i < 5; i++) {
		puts("wait...");
		sleep(100);
	}
}
