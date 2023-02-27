#include <stdio.h>
#include <fcntl.h>  // open()
#include <unistd.h> //close()
#include <sys/mman.h>  // mmap
#include <string.h>
#include<stdlib.h>

int main(void)
{
	char* buf;
	//打开文件
	int fd = open("testdata", O_RDWR);
	//创建mmap
	char* start = (char*)mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//读取文件
	strcpy(buf, start);
	//写入文件
	strcpy(start, "Write to file!\n");

	munmap(start, 128);
	close(fd);
	return 0;
}