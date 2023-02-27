#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

struct MyInfo
{
	int age;
	char name[20];
};

void* pthread_fun1(void* arg)
{
	struct MyInfo* p = (struct MyInfo*)arg;
	for (int i = 0; i < 10; i++)
	{
		printf("线程函数---run1");
		printf("age:%d, name:%s\n", p->age, p->name);
		sleep(1);
	}
}

void* pthread_fun2(void* arg)
{
	for (int i = 0; i < 10; i++)
	{
		printf("线程函数---run2\n");
		pthread_testcancel();
		sleep(1);
	}
}

int main(void)
{
	int n = 0;
	struct MyInfo info = {18, "小家电"};
	pthread_t pid1;
	pthread_t pid2;
	int res1 = pthread_create(&pid1, NULL, pthread_fun1, (void*)&info);
	int res2 = pthread_create(&pid2, NULL, pthread_fun2, NULL);
	pthread_join(pid1, NULL); //等待pid1线程退出
	pthread_join(pid2, NULL);
	assert(res1 == 0);  // 打印错误信息
	assert(res2 == 0);
	for (int i = 0; i < 5; i++)
	{
		if (3 == i)
		{
			pthread_cancel(pid2);  // 线程id为pid2的线程结束
		}
		printf("main函数---run\n");
		sleep(1);
	}
	return 0;
}