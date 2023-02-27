#include <stdio.h>
#include <unistd.h>

union u
{
	char c[4];
	int n;
};

// 大小端问题
int main(void)
{
	//类型 决定 存储方式和占用空间大小 4字节
	//int 有x86架构 arm架构 ...等
	union u uu;
	uu.n = 0x11223344;
	printf("%x %x %x %x\n", uu.c[0], uu.c[1], uu.c[2], uu.c[3]);

	return 0;
}