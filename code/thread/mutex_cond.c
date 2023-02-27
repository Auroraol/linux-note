#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义变量
pthread_mutex_t mutex;
pthread_cond_t cond;
int n = 0;
void* fun1(void* a)
{
	for (int i = 0; i < 10; i++)
	{
		// 上锁
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);  //上锁临界变量等待
		printf("线程1 -------------------%d\n", n++);
		// 解锁
		pthread_mutex_unlock(&mutex);
	}
}

void* fun2(void* a)
{
	for (int i = 0; i < 10; i++)
	{
		// 上锁
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		printf("线程2 -------------------%d\n", n++);
		// 解锁
		pthread_mutex_unlock(&mutex);
	}
}

int main(void)
{
	// 初始化互斥锁
	pthread_mutex_init(&mutex, NULL);
	// 初始化临界变量
	pthread_cond_init(&cond, NULL);

	pthread_t p1, p2;
	pthread_create(&p1, NULL, fun1, NULL);
	pthread_create(&p2, NULL, fun2, NULL);

	for (int i = 0; i < 20; i++)
	{// 放了20个消息
		pthread_cond_signal(&cond); // 发信号给临界变量
		sleep(1);
	}

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	printf("n: %d\n", n);//100000000

	// 销毁互斥量,临界变量
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}