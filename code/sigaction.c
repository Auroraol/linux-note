#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void hand(int val)
{
	printf("基本处理函数: %d\n", val);
}

void handler(int n, siginfo_t* siginfo, void* arg)
{
	printf("高级的信号处理函数!\n");
	printf("sig:%d,msg:%d, msg:%d\n", n, siginfo->si_value.sival_int, siginfo->si_int);
}

int main(int argc, char* argv[])
{
	struct sigaction act = {0};
	struct sigaction oldAct = {0};
	/* 设置信号忽略 */
	act.sa_handler = hand; //比如:act.sa_handler = SIG_IGN;
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;
	//SIGINT 就是 2
	sigaction(2, &act, &oldAct); // 注册高级信号处理;//原来的备份到oldAct里面

	printf("pid:%d\n", getpid());
	int n = 0;
	while (n < 500)
	{
		printf("n:%d\n", n++);
		sleep(1);
	}

	return 0;
}