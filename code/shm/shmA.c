#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

int* p = NULL;

void hand(int val)
{
	//5 卸载共享内存
	shmdt(p);
	printf("bye bye!\n");
	exit(0);
}

int main(int agrc, char* agrv[])
{
	signal(2, hand);
	//1 创建key
	key_t key = ftok(".", 'm');  // 参数:文件路径  整数(256以类)
	if (-1 == key) printf("%m\n"), exit(-1); //输出了错误信息 后退出程序
	//2 创建共享内存 最小单位: 页 4k
	int shmid = shmget(key, 4096, IPC_CREAT);
	if (-1 == shmid) printf("shmget error: %m\n"), exit(-1);
	printf("shmget Success!\n");
	//3 挂载共享内存
	p = (int*)shmat(shmid, NULL, 0);
	if ((void*)-1 == p) printf("shmat error :%m\n"), exit(-1);
	printf("shmat Success!\n");
	//4 使用共享内存
	while (1)
	{
		printf("%d\n", *p);
		sleep(1);
	}
	return 0;
}