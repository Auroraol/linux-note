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
	int fd = open("mmapWrite.txt", O_RDONLY);

	//3 映射虚拟内存
	student* ptr = (student*)mmap(NULL, 3 * sizeof(student),
		PROT_READ,
		MAP_SHARED, fd, 0);
	if ((void*)-1 == ptr)
	{
		printf("映射虚拟内存失败\n");
		close(fd);
		return -1;
	}
	//4 使用内存
	student* s = ptr;

	for (int i = 0; i < 3; i++)
	{
		printf("id:%d, name:%s, age:%d, grade:%g\n",
			s->id, s->name, s->age, s->grade);
		s++;
	}
	int ret = munmap(ptr, 3 * sizeof(student));
	//6 关闭文件
	close(fd);

	return 0;
}