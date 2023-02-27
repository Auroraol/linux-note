#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	struct pollfd fds[10];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	int r;
	char buff[1024];
	while (1)
	{
		r = poll(fds, 1, 0);
		if (-1 == r)
		{
			printf("监视出错!\n");
		}
		else if (0 == r)
		{
			continue;
		}
		else
		{
			printf("有动静!\n");
			if (fds[0].revents && POLLIN)
			{
				memset(buff, 0, sizeof(buff));
				scanf("%s", buff);
				printf(">>%s\n", buff);
			}
		}
	}
	return 0;
}