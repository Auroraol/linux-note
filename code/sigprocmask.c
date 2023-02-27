#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void hand(int val)
{
	printf("基本信号处理函数:%d\n", val);
}
int main(int argc, char* argv[])
{
	sigset_t set, oldSet;
	sigemptyset(&set);  //清空信号集
	sigaddset(&set, SIGINT);  // 往信号集中添加一个关闭信号

	//先五秒钟不设置信号屏蔽
	signal(SIGINT, hand);
	sleep(5);
	int ret;
	//设置信号屏蔽20秒
	if (1 == sigismember(&set, SIGINT)) //判断某个信号是否在信号集中
	{
		printf("设置信号屏蔽!\n");
		ret = sigprocmask(SIG_BLOCK, &set, &oldSet);
		if (0 == ret)
			printf("设置信号屏蔽成功!\n");
		else
			printf("设置信号屏蔽失败:%m\n");
	}
	sleep(20);
	//解除信号屏蔽
	if (1 == sigismember(&set, SIGINT))
	{
		printf("解除信号屏蔽!\n");
		ret = sigprocmask(SIG_UNBLOCK, &set, &oldSet);
		if (0 == ret)
			printf("解除信号屏蔽成功!\n");
		else
			printf("解除信号屏蔽失败:%m\n");
	}

	// 从信号集中删除 会一个信号
	sigdelset(&set, SIGINT);
	return 0;
}