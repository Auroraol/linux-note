#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

int n = 0;
void* fun(void* a)
{
	for (int i = 0; i < 50000000; i++)
		n++;
}

int main(void)
{
	pthread_t p1, p2;
	pthread_create(&p1, NULL, fun, NULL);
	pthread_create(&p2, NULL, fun, NULL);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	printf("n: %d\n", n);//100000000return 0;
	return 0;
}