#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

//void hand(int val)
//{
//	//删除信号量
//	union semun sem_union;
//	if (semctl(shmid, 0, IPC_RMID, sem_union) == -1)
//		printf("%m\n"), exit(-1);
//}

union semun
{
	int val;
	struct semid_ds* buf;
	unsigned short* arry;
};

int main(void)
{
	//signal(2, hand);
	//1 创建key
	key_t key = ftok(".", 'm');  // 参数:文件路径  整数(256以类)
	if (-1 == key) printf("%m\n"), exit(-1); //输出了错误信息 后退出程序
	//2 创建信号量
	int shmid = semget(key, 1, IPC_CREAT | 0666); //1表示一个信号量, 0654表示权限
	//3 初始化
	union semun sem_union;
	sem_union.val = 1;
	//semct1(semid, 0, IPC_SET, sem_union);
	if (semctl(shmid, 0, SETVAL, sem_union) == -1)
		return 0;   //0表示从哪里开始 //IPC STAT获取状态 IPC SET设置 IPC RMID删除 ...
	//4 使用信号量
	struct sembuf buf;
	buf.sem_num = 0; // 信号量的索引
	buf.sem_op = -1; // 加还是减, 加多少减多少,这里就表示减1
	buf.sem_flg = 0; // 一般设置为0
	int n = 0;
	while (1)
	{
		if (semop(shmid, &buf, 1) == -1)
		{ // 参数3:操作次数 参数2: 怎样操作
			printf("%m\n"), exit(-1);
		}
		printf("卖出去%d辆劳斯莱斯!\n", ++n);
	}
	return 0;
}