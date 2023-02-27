#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义变量
pthread_mutex_t mutex;
int n = 0;
void* fun(void* a)
{
	for (int i = 0; i < 50000000; i++)
	{
		// 上锁
		pthread_mutex_lock(&mutex);
		n++;
		// 解锁
		pthread_mutex_unlock(&mutex);
	}
}

int main(void)
{
	// 初始化互斥锁
	pthread_mutex_init(&mutex, NULL);
	pthread_t p1, p2;
	pthread_create(&p1, NULL, fun, NULL);
	pthread_create(&p2, NULL, fun, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("n: %d\n", n);//100000000
	pthread_mutex_destroy(&mutex);
	return 0;
}