#include <aio.h>  // AIO
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#pragma comment(lib,"winmm.1ib")

#define BUFF_SIZE 1024

int main(void)
{
	//1 准备缓冲区
	struct aiocb cb;
	memset(&cb, 0, sizeof(struct aiocb));
	int fd = open("test.txt", O_RDONLY);
	if (-1 == fd)
	{
		printf("打开文件失败%m\n"), exit(-1);
	}
	printf("打开文件成功\n");

	//2 异步操作异步读异步写
	cb.aio_buf = malloc(BUFF_SIZE + 1); // 开内存
	memset(cb.aio_buf, 0, sizeof(BUFF_SIZE + 1)); // 清空
	cb.aio_fildes = fd;//文件描述符号
	cb.aio_nbytes = BUFF_SIZE;//读取数据大小
	cb.aio_offset = 0;  //文件中的偏移量

	int r = aio_read(&cb);  // 异步读
	if (-1 == r)
	{
		printf("异步读失败: %m\n");
		close(fd);
		exit(-2);
	}
	printf("异步读成功\n");

	//3 检查是否操作完毕, 检查异步请求的状态
	// 循环等待异步读操作结束
	int n = 0;
	while (aio_error(&cb))
	{
		++n;
	}

	//4 得到数据, 返回读取到的数据的长度, 失败返回-1
	r = aio_return(&cb);

	if (r > 0)
	{
		printf("拿到数据了:n %d,数据长度: %d,data: %s \n", n, r, cb.aio_buf);
	}

	free(cb.aio_buf);
	close(fd);

	return 0;
}