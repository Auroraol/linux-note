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
	int fd = open("1.txt", O_RDONLY);
	if (-1 == fd)
	{
		printf("打开文件失败\n");
		return -1;
	}
	printf("文件打开成功\n");
	student s;
	for (int i = 0; i < 5; i++)
	{
		read(fd, &s, sizeof(student));
		printf("id:%d, name:%s, age:%d, grade:%g\n",
			s.id, s.name, s.age, s.grade);
	}

	close(fd);
	return 0;
}