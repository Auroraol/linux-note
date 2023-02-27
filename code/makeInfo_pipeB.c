#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	//2 打开管道文件
	int fd = open("test.pipe", O_RDONLY);
	if (-1 == fd) printf("打开管道文件失败:%m\n"), unlink("test.pipe"), exit(-1);
	printf("打开管道文件成功!\n");
	//3 循环写入
	int n = 0;
	char buff[1024] = {0};
	while (1)
	{
		int r = read(fd, buff, 1023);
		if (r > 0)
		{
			printf(">> %s \n", buff);
		}
		sleep(1);
	}
	//4 关闭
	close(fd);
	//5 删除管道文件
	unlink("test.pipe");

	return 0;
}