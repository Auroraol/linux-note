#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <iomanip>  // 格式化打印时间

int main(void)
{
	char userName[25] = "admin";
	char passWord[25] = "123";
	char user[25];
	char pass[25];
	while (1)
	{
		printf("请输入用户名：");
		scanf("%s", user);
		printf("请输入密码：");
		scanf("%s", pass);
		if (0 == strcmp(user, userName) &&
			0 == strcmp(pass, passWord))
		{
			printf("登录成功!\n");
			break;
		}
		else
		{
			printf("登录失败,请重新输入！\n");
			sleep(2);
			system("clear");  // 执行clear命令
		}
	}

	if (0 != fork())
	{
		//父进程
		while (1)
		{
			char* wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
			time_t now = time(NULL);
			tm* p_tm = localtime(&now); /*获取本地时区时间*/

			printf("%d-%d-%d ", (p_tm->tm_year + 1900), (p_tm->tm_mon + 1), p_tm->tm_mday);
			printf("%s %d:%d:%d\n", wday[p_tm->tm_wday], p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
			sleep(1);
		}
	}
	else
	{
		//子进程
		srand((unsigned int)time(NULL));   // 随机数种子
		while (1)
		{
			printf("============%07d==========\n", rand() % 1000000);
			usleep(500000); //让程序休眠若干「微秒」
		}
	}

	return 0;
}