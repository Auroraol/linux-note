#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义变量
pthread_rwlock_t rwlock;
int n = 0;
void* fun1(void* a)
{
	for (int i = 0; i < 5000; i++)
	{
		pthread_rwlock_rdlock(&rwlock);
		n++;
		pthread_rwlock_unlock(&rwlock);
	}
}

void* fun2(void* a)
{
	for (int i = 0; i < 5000; i++)
	{
		pthread_rwlock_rdlock(&rwlock);
		n++;
		pthread_rwlock_unlock(&rwlock);
	}
}

int main(void)
{
	//1初始化读写锁
	pthread_rwlock_init(&rwlock, NULL);

	pthread_t p1, p2;
	pthread_create(&p1, NULL, fun1, NULL);
	pthread_create(&p2, NULL, fun2, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	printf("n: %d\n", n);

	// 销毁
	pthread_rwlock_destroy(&rwlock);
	return 0;
}