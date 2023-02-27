#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <poll.h>

#define MUN 300  // 最多允许的最大客户端数量
int tcpserver;
struct pollfd fds[MUN];//监视的fd结构体数组

void hand(int val)
{
	// 7. 关闭socket，释放资源。
	close(tcpserver);
	for (int i = 0; i < MUN; i++)
	{
		if (fds[i].fd != -1)
		{
			close(fds[i].fd);
		}
	}
	printf("bye bye!\n");
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc != 3)printf("请输入ip地址和端口号！\n"), exit(0);
	printf("ip: %s    port: %d\n", argv[1], atoi(argv[2]));
	signal(2, hand);
	//1 创建socket
	tcpserver = socket(AF_INET, SOCK_STREAM, 0);//参数: 协议域 类型 指定协议
	if (-1 == tcpserver) printf("创建socket失败:%m\n"), exit(-1);
	printf("创建tcpserver_socket成功!\n");
	//2 创建服务器协议地址簇
	struct sockaddr_in  servAddr;  // 服务端地址信息的数据结构。
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;//协议类型 和socket函数第一个参数一致
	servAddr.sin_addr.s_addr = inet_addr(argv[1]); // 字符串 转成 整形, 点分十进制的IPv4地址转换成网络字节序列的长整型。  // htonl(INADDR_ANY);  任意ip地址。
	servAddr.sin_port = htons(atoi(argv[2])); // 整型 转成 整型 小端转大端

	// 3. 绑定
	int r = bind(tcpserver, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (-1 == r)printf("绑定失败：%m\n"), close(tcpserver), exit(-2);
	printf("绑定成功!\n");

	// 4. 监听
	r = listen(tcpserver, 10);
	if (-1 == r) printf("监听失败:%m\n"), close(tcpserver), exit(-2);
	printf("监听成功!\n");

	int tempFd;
	char buff[1024] = {0};
	// 5. 接受客户端连接
	struct sockaddr_in clientAddr;  // 客户端的地址信息。
	memset(&clientAddr, 0, sizeof(clientAddr));
	int socklen = sizeof(struct sockaddr_in); // struct sockaddr_in 的大小

	//
	int fdNum = 0; //当前描述符号数量, 及当前客户端数量

	// 初始化客户端描述符号
	for (int i = 0; i < MUN; i++)
	{
		fds[i].fd = -1;
		fds[i].events = POLLIN;
	}
	//把要监视的描述符号设置好, 把服务端描述符号放入
	fds[0].fd = tcpserver;
	fdNum++;
	while (1)
	{
		r = poll(fds, MUN, 10); //1. 要监控的文件描述符的结构体数组 2. nfds：数组的长度3. timeout：超时时间，单位是毫秒
		if (-1 == r)
		{
			printf("监视出错:%m!\n");
			close(tcpserver);
			exit(-1);
		}
		else if (0 == r)
		{
			//printf("延时....\n");
			continue;
		}
		else
		{
			if (fds[0].revents & POLLIN)
			{
				//有客尸端连接服务器的动作
				tempFd = accept(tcpserver, (struct sockaddr*)&clientAddr, (socklen_t*)&socklen);
				if (-1 == tempFd)
				{
					printf("服务器崩溃:%m\n");
					continue;
				}
				printf("有客户端连接上服务器了:%d - %s\n", tempFd, inet_ntoa(clientAddr.sin_addr));

				// 保存客户端描述符
				for (int i = 0; i < MUN; i++)
				{
					if (-1 == fds[fdNum].fd)
					{
						fds[fdNum].fd = tempFd;
						//最大描述符号变化
						fdNum++;
						break;
					}
				}
			}

			// 6. 通信
			for (int i = 1; i < MUN; i++)
			{
				if (-1 != fds[i].fd && (fds[i].revents & POLLIN))
				{
					int iret = recv(fds[i].fd, buff, sizeof(buff), 0); // 255表示长度 0表示默认方式 //接收客户端的请求报文
					if (iret > 0)
					{
						buff[iret] = '\0';//添加结束符号
						printf("接收 %d客户端的数据： %s\n", fds[i].fd, buff);
					}
					strcpy(buff, "ok");
					iret = send(fds[i].fd, buff, sizeof(buff), 0);// 向客户端发送响应结果。
					if (iret > 0)
					{
						buff[iret] = '\0';//添加结束符号
						printf("发送给客户端：%s\n", buff);
					}
					else
					{
						//客户端掉线
						fds[i].fd = -1;
						fdNum--;
					}
				}
			}
		}
	}
	return 0;
}