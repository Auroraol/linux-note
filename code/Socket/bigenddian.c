#include <stdio.h>
#include <unistd.h>

union u
{
	char c[4];
	int n;
};

// ��С������
int main(void)
{
	//���� ���� �洢��ʽ��ռ�ÿռ��С 4�ֽ�
	//int ��x86�ܹ� arm�ܹ� ...��
	union u uu;
	uu.n = 0x11223344;
	printf("%x %x %x %x\n", uu.c[0], uu.c[1], uu.c[2], uu.c[3]);

	return 0;
}