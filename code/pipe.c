#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
// 使用匿名管道循 父进程 不断等待用户输入 并写入管道  子进程循环读取管道内容并输出
int main(void)
{
	//1 创建管道描述符号
	int fds[2];
	//2 创建管道
	int r = pipe(fds);
	if (-1 == r)
		printf("创建管道失败:%m\n"), exit(-1);
	printf("创建管道成功!\n");

	//3 使用管道
	if (fork())
	{
		char buff[2024];
		while (true)
		{
			scanf("%s", buff);
			write(fds[1], buff, strlen(buff)); // 往管道输入端输入
		}
	}
	else
	{
		char temp[1024];
		while (true)
		{
			r = read(fds[0], temp, 1024);//从管道输出端读取
			if (r > 0)
			{
				temp[r] = 0; //添加结束符号
				printf(">> %s \n", temp);
			}
		}
	}

	// 4. 关闭管道
	close(fds[0]);
	close(fds[1]);

	return 0;
}