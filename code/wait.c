#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	if (0 != fork())
	{
		printf("父进程被创建了!\n");
		wait(0);
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			printf("子进程 :%d \n", i);
			sleep(1);
		}
	}
	printf("父进程结束！n");
	return 0;
}