#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> //close()

typedef struct
{
	int id;
	char name[20];
	int age;
	double grade;
}student;

int main(void)
{
	int fd = open("1.txt", O_WRONLY);
	if (-1 == fd)
	{
		int fd = open("1.txt", O_CREAT | O_WRONLY, 0666);
		if (-1 == fd)
		{
			printf("创建文件失败\n");
			return -1;
		}
		printf("创建文件成功\n");
	}
	printf("打开文件成功\n");

	student stu[5] = {
		{1,"关羽",50,66.66},
		{2,"张飞",48,12.22},
		{3,"赵云",35,55.55},
		{4,"马超",30,44.44},
		{5,"黄忠",66,88.8},
	};

	write(fd, stu, sizeof(student) * 5);  // 写入

	close(fd);
	return 0;
}