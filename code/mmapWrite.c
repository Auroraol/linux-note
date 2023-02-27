#include <stdio.h>
#include <fcntl.h>  // open()
#include <unistd.h> //close()
#include <sys/mman.h>  // mmap
#include <string.h>
#include<stdlib.h>

typedef struct
{
	int id;
	char name[20];
	int age;
	double grade;
}student;

int main(void)
{
	// 1.打开文件,写入
	int fd = open("mmapWrite.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (-1 == fd)
	{
		printf("创建文件失败\n");
		return -1;
	}

	// 2 修改文件大小
	ftruncate(fd, 3 * sizeof(student));

	//3 映射虚拟内存
	student* ptr = (student*)mmap(0, 3 * sizeof(student),
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_SHARED, fd, 0);
	if ((void*)-1 == ptr)
	{
		printf("映射虚拟内存失败\n");
		close(fd);
		return -1;
	}
	//4 使用内存
	student* ppp = ptr;
	ppp->id = 1;
	strcpy(ppp->name, "张三");
	ppp->age = 33;
	ppp->grade = 1.3;
	ppp++;

	ppp->id = 2;
	strcpy(ppp->name, "李四");
	ppp->age = 44;
	ppp->grade = 4.3;
	ppp++;

	ppp->id = 3;
	strcpy(ppp->name, "李五");
	ppp->age = 32;
	ppp->grade = 3.33;

	//5 接触虚拟内存映射
	int ret = munmap(ptr, 3 * sizeof(student));
	if (ret)
	{
		printf("munmmap失败");
	}

	//6 关闭文件
	close(fd);

	return 0;
}