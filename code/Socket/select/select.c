#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>

int main(void)
{
	fd_set fds; // 描述符号集合

	FD_ZERO(&fds); // 清空
	FD_SET(0, &fds);

	int r;
	char buff[1024];
	while (1)
	{
		r = select(1, &fds, NULL, NULL, NULL); // 阻塞
		if (r > 0)
		{
			printf("%d有动静\n", r);
			scanf("%s", buff);
			printf("接收到了: %s\n", buff);
		}
		else
		{
			printf("没有动静: %d\n", r);
		}
	}
	return 0;
}