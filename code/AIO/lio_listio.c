#include <aio.h>  // AIO
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#pragma comment(lib,"winmm.1ib")

#define BUFF_SIZE 1024

//aio个数
#define AIO_LIST_NUM 2

int main(void)
{
	//1 准备缓冲区
	struct aiocb rcb;
	struct aiocb wcb;
	struct aiocb* aiocb_list[AIO_LIST_NUM] = {0};//aio SuSpend第一个参数结构体指针数组
	memset(&rcb, 0, sizeof(struct aiocb));
	memset(&wcb, 0, sizeof(struct aiocb));

	//2 异步操作异步读/异步写
	// 异步读
	int rfd = open("test.txt", O_RDONLY);
	if (-1 == rfd)
	{
		printf("打开文件失败%m\n"), exit(-1);
	}
	printf("打开文件成功\n");

	rcb.aio_buf = malloc(BUFF_SIZE + 1); // 开内存
	memset(rcb.aio_buf, 0, sizeof(BUFF_SIZE + 1)); // 清空
	rcb.aio_fildes = rfd;//文件描述符号
	rcb.aio_nbytes = BUFF_SIZE;//读取数据大小
	rcb.aio_offset = 0;  //文件中的偏移量
	rcb.aio_lio_opcode = LIO_READ;   //lio操作类型为异步读

	aiocb_list[0] = &rcb;

	int r = aio_read(&rcb);
	if (-1 == r)
	{
		printf("异步读失败: %m\n");
		close(rfd);
		exit(-2);
	}
	printf("异步读成功\n");

	// 异步写
	int wfd = open("test2.txt", O_WRONLY | O_APPEND);
	if (-1 == wfd)
	{
		printf("打开文件失败%m\n"), exit(-1);
	}
	printf("打开文件成功\n");

	wcb.aio_buf = (char*)malloc(BUFF_SIZE + 1); // 开内存
	memset(wcb.aio_buf, 0, sizeof(BUFF_SIZE + 1)); // 清空
	strcpy(wcb.aio_buf, "哈哈哈哈哈");  // 写数据
	wcb.aio_fildes = wfd;//文件描述符号
	wcb.aio_nbytes = strlen("哈哈哈哈哈");//读取数据大小
	wcb.aio_lio_opcode = LIO_WRITE;   //lio操作类型为异步写

	aiocb_list[1] = &wcb;     //将异步写事件添加到list中

	int w = aio_write(&wcb);
	if (-1 == w)
	{
		printf("异步写失败: %m\n");
	}
	printf("异步写成功\n");

	// 4 1io 1istio监控多个io操作
	int ret = lio_listio(LIO_WAIT, aiocb_list, AIO_LIST_NUM, NULL);

	//4 得到数据, 返回读取到的数据的长度, 失败返回-1
	r = aio_return(&rcb);
	if (r > 0)
	{
		printf("读出数据长度: % d, data : % s \n", r, rcb.aio_buf);
	}

	r = aio_return(&wcb);
	if (r > 0)
	{
		printf("写入数据长度: %d\n", r);
	}

	// 关闭
	free(rcb.aio_buf);
	free(wcb.aio_buf);
	close(rfd);
	close(wfd);

	return 0;
}