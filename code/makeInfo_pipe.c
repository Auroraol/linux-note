#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	//1 创建管道文件
	int r = mkfifo("test.pipe", 0666);
	if (-1 == r) printf("创建管道文件失败:%m\n"), exit(-1);
	printf("创建管道文件成功!\n");
	//2 打开管道文件
	int fd = open("test.pipe", O_WRONLY);
	if (-1 == fd) printf("打开管道文件失败:%m\n"), unlink("test.pipe"), exit(-1);
	printf("打开管道文件成功!\n");
	//3 循环写入
	int n = 0;
	char buff[1024] = {0};
	while (1)
	{
		sprintf(buff, "好帅:%d\n", n++);
		write(fd, buff, strlen(buff));
		sleep(1);
	}
	//4 关闭
	close(fd);
	//5 删除管道文件
	unlink("test.pipe");

	return 0;
}