#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

int tcpserver, clientConnection;

void hand(int val)
{
	// 7. 关闭socket，释放资源。
	close(tcpserver);
	close(clientConnection);
	printf("bye bye!\n");
	exit(0);
}

int main()
{
	signal(2, hand);
	//1 创建socket
	tcpserver = socket(AF_INET, SOCK_STREAM, 0);//参数: 协议域 类型 指定协议
	if (-1 == tcpserver) printf("创建socket失败:%m\n"), exit(-1);
	printf("创建tcpserver_socket成功!\n");
	//2 创建服务器协议地址簇
	struct sockaddr_in  servAddr;  // 服务端地址信息的数据结构。
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;//协议类型 和socket函数第一个参数一致
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 字符串 转成 整形, 点分十进制的IPv4地址转换成网络字节序列的长整型。  // htonl(INADDR_ANY);  任意ip地址。
	servAddr.sin_port = htons(10860); // 整型 转成 整型 小端转大端

	// 3. 绑定
	int r = bind(tcpserver, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (-1 == r)printf("绑定失败：%m\n"), close(tcpserver), exit(-2);
	printf("绑定成功!\n");

	// 4. 监听
	r = listen(tcpserver, 10);
	if (-1 == r) printf("监听失败:%m\n"), close(tcpserver), exit(-2);
	printf("监听成功!\n");

	// 5. 接受客户端连接
	struct sockaddr_in clientAddr;  // 客户端的地址信息。
	memset(&clientAddr, 0, sizeof(clientAddr));
	int socklen = sizeof(struct sockaddr_in); // struct sockaddr_in 的大小
	clientConnection = accept(tcpserver, (struct sockaddr*)&clientAddr, (socklen_t*)&socklen);  // 进行通讯 //接受客服端的套接字对象【及保存客户端的IP，port】
	if (-1 == clientConnection) printf("服务器崩溃:%m\n"), close(tcpserver), exit(-2);
	printf("有客户端连接上服务器了: %s!\n", inet_ntoa(clientAddr.sin_addr));
	// 6. 通信
	char buff[1024];
	char fileName[256] = {0};
	int fileSize;
	int count = 0;
	r = recv(clientConnection, fileName, 255, 0);
	if (r > 0)
	{
		printf("接受到文件名:%s\n", fileName);
	}

	r = recv(clientConnection, (char*)&fileSize, 4, 0);
	if (4 == r)
	{
		printf("接收到文件大小:%d \n", fileSize);
	}
	char str[20];
	sprintf(str, "cp%s", fileName);
	printf("生成文件名: %s\n", str);
	//int fd = open(str, O_CREAT | O_WRONLY, 0666);
	int fd = open("cp1.txt", O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		printf("创建文件失败\n");
	int flag = 0;
	while (1)
	{
		r = recv(clientConnection, buff, 1024, 0);
		if (r > 0)
		{
			write(fd, buff, r);
			count += r;
			if (count >= fileSize)
				break;
		}
		/*else
		{
			break;
		}*/
	}

	printf("接收完毕!\n");
	close(fd);
	close(tcpserver);
	close(clientConnection);

	return 0;
}