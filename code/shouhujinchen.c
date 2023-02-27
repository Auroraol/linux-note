#include <stdio.h>
#include <fcntl.h>  // open()
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> //
#include <stdlib.h>
#include <signal.h>   //exit()

int main(void)
{
	//1．重设文件权限 umask
	umask(0);
	//2.创建子进程 fork
	int res = fork();
	if (res < 0)
	{
		printf("创建进程失败");
		exit(EXIT_FAILURE);
	}
	else if (res > 0)
	{
		//3．让父进程结束
		printf("父进程结束\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("pid = % d\n", getpid());
		//4．创建新会话 setsid
		setsid();
		//5．防止子进程成为僵尸进程  忽略掉SIGCHLD SIGHUP信号
		signal(SIGCHLD, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
		//6.改变当前工作目录 chdir
		chdir("/");
		//7．重定向文件描述符号  open() dup2(fd, o) dup2(fd, 1)
		int fd = open("/dev/NULL", O_RDWR);
		dup2(fd, 0);
		dup2(fd, 1);
	}

	while (true)
	{
		sleep(1); //模拟守护进程工作
	}
	return 0;
}