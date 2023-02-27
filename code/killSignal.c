#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int pid = atoi(argv[1]);//字符串转整数
	int sig = atoi(argv[2]);
	printf("-----pid: %s, sig : %s\n", argv[1], argv[2]);
	printf("pid:%d,sig:%d\n", pid, sig);
	kill(pid, sig);
	return 0;
}