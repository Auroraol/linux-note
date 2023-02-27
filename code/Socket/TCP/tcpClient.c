#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

int tcpClient;

void hand(int val)
{
	// 7. 关闭socket，释放资源。
	close(tcpClient);
	printf("bye bye!\n");
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc != 3)printf("请输入ip地址和端口号！\n"), exit(0);
	printf("ip: %s    port: %d\n", argv[1], atoi(argv[2]));
	signal(2, hand);
	// 1 创建socket
	tcpClient = socket(AF_INET, SOCK_STREAM, 0);//参数: 协议域 类型 指定协议
	if (-1 == tcpClient) printf("创建socket失败:%m\n"), exit(-1);
	printf("创建tcpClient_socket成功!\n");
	// 2 创建服务器协议地址簇
	struct sockaddr_in  servAddr;  // 服务端地址信息的数据结构。
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;//协议类型 和socket函数第一个参数一致
	servAddr.sin_addr.s_addr = inet_addr(argv[1]); // 字符串 转成 整形, 点分十进制的IPv4地址转换成网络字节序列的长整型。  // htonl(INADDR_ANY);  任意ip地址。
	servAddr.sin_port = htons(atoi(argv[2])); // 整型 转成 整型 小端转大端
	//3 连接服务器
	int r = connect(tcpClient, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (-1 == r)printf("连接服务器失败:%m\n"), close(tcpClient), exit(-2);
	printf("连接服务器成功!\n");
	// 4. 通信
	char buff[1024] = {0};
	while (1)
	{
		printf("发送给服务器：");
		scanf("%s", buff);
		int iret = send(tcpClient, buff, sizeof(buff), 0);
		if (iret <= 0)
		{
			printf("发送失败%m");
		}

		iret = recv(tcpClient, buff, sizeof(buff), 0);
		if (iret > 0)
		{
			buff[iret] = '\0';//添加结束符号
			printf("接收服务器的数据： %s\n", buff);
		}
	}
	return 0;
}
