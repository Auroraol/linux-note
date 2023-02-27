#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> //提供IP地址转换函数
#include <netdb.h>
#include <signal.h>
#include <sys/epoll.h> // epoll

#define NUM 30000  // 最多允许的最大客户端数量
struct epoll_event events[NUM]; // 等待处理事件的时候用, 用于存储监控的结果
int tcpserver;

void hand(int val)
{
	// 7. 关闭socket，释放资源。
	close(tcpserver);
	for (int i = 0; i < NUM; i++)
	{
		if (events[i].data.fd != -1)
		{
			close(events[i].data.fd);
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

	// 监视
	int tempFd;
	char buff[1024] = {0};
	// 5. 接受客户端连接
	// 客户端的地址信息
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int socklen = sizeof(struct sockaddr_in); // struct sockaddr_in 的大小
	//5.1 创建epol1参数可忽略给设置的最大描述符号数量
	int epfd = epoll_create(NUM);
	if (-1 == epfd)
	{
		printf("创建epoll失败:%m\n"), close(tcpserver), exit(-1);
	}
	printf("创建epoll成功\n");

	//5.2 注册事件
	struct epoll_event ev;  //描述符注册时候用
	// 初始化客户端描述符号
	for (int i = 0; i < NUM; i++)
	{
		events[i].data.fd = -1;
	}

	ev.events = EPOLLIN;   // 要监控的事件
	ev.data.fd = tcpserver;
	r = epoll_ctl(epfd, EPOLL_CTL_ADD, tcpserver, &ev);
	if (-1 == r)
	{
		printf("注册tcpserver事件失败：%m\n");
		close(epfd);
		close(tcpserver);
		exit(-2);
	}
	printf("注册tcpserver事件成功!\n");
	int nfds; //epoll wait的返回值
	while (1)
	{
		nfds = epoll_wait(epfd, events, NUM, 1000);
		/*
		epfd：epoll_create的返回值
		events：用于存储监控的结果
		maxevents：events数组的长度
		timeout：超时时间
		返回值：返回事件发生的描述符的数量
		*/
		if (nfds < 0)
		{
			printf("epoll_wait失败：%m\n");
			close(epfd);
			close(tcpserver);
			exit(-3);
		}
		else if (0 == nfds)
		{
			continue;
		}
		else
		{
			for (int i = 0; i < nfds; i++)
			{
				if (tcpserver == events[i].data.fd)
				{
					tempFd = accept(tcpserver, NULL, NULL);
					if (-1 == tempFd)
					{
						printf("服务器崩溃:%m\n");
						close(epfd);
						close(tcpserver);
						exit(-4);
					}
					printf("有客户连接上服务器了:%d\n", tempFd);  //打印注册客户端描述符号
					// 注册客户端描述符号
					ev.events = EPOLLIN;
					ev.data.fd = tempFd;
					epoll_ctl(epfd, EPOLL_CTL_ADD, tempFd, &ev);
				}

				// 6. 通信
				else if (events[i].events & EPOLLIN)
				{
					//某个客户端发数据过来了
					int iret = recv(events[i].data.fd, buff, 1023, 0);
					if (iret > 0)
					{
						buff[iret] = '\0';//添加结束符号
						printf("接收 %d客户端的数据： %s\n", events[i].data.fd, buff);
					}
					strcpy(buff, "ok");
					iret = send(events[i].data.fd, buff, sizeof(buff), 0);// 向客户端发送响应结果。
					if (iret > 0)
					{
						buff[iret] = '\0';//添加结束符号
						printf("发送给客户端：%s\n", buff);
					}
				}
			}
		}
	}
	return 0;
}