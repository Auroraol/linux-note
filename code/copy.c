#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char buff[256] = {0};
	sprintf(buff, "cp %s %s", argv[1], argv[2]);
	printf("%s\n", buff);
	system(buff);  // Ö´ÐÐÃüÁî
	return 0;
}