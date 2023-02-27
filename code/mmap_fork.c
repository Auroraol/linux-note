#include <stdio.h>
#include <fcntl.h>  // open()
#include <unistd.h> //close()
#include <sys/mman.h>  // mmap
#include <string.h>
#include<stdlib.h>

int main(void)
{
	// 1.打开文件,写入
	int fd = open("mmap_fork.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (-1 == fd)
	{
		printf("创建文件失败\n");
		return -1;
	}

	// 2 修改文件大小
	ftruncate(fd, 4);

	//3 映射虚拟内存
	int* ptr = (int*)mmap(0, 4,
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_SHARED, fd, 0);
	if ((void*)-1 == ptr)
	{
		printf("映射虚拟内存失败\n");
		close(fd);
		return -1;
	}
	*ptr = 0;
	if (fork())
	{
		int n = 0;
		while (1)
		{
			while (*ptr);//开关
			printf("n :%d\n", n++);
			sleep(1);
		}
	}
	else
	{
		int n = 0;
		char ch;
		while (1)
		{
			read(0, &ch, 1);  // linux当中的read()函数 0表示从控制台, 从控制台读出数据 write(0, buff, strlen(buff)); // 写入终端
			if (ch == '\n')
			{
				n++;
			}
			if (n % 2)
				*ptr = 0;
			else
				*ptr = 1;
		}
	}
	return 0;
}