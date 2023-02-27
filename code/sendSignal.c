#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void hand(int val)
{
	printf("val: %d\n", val);
	printf("想把老子干掉, 没门\n");
}
int main(int argc, char* argv[])
{
	signal(SIGINT, hand);  //ctrl +c发送SIGINT
	printf("pid:%d\n", getpid());
	int n = 0;
	while (1)
	{
		printf("n:%d\n", n++);
		sleep(1);
	}
	return 0;
}