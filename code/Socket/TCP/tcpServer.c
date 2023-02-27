#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

int tcpserver, clientConnection;

void hand(int val)
{
	// 7. 关闭socket，释放资源。
	close(tcpserver);
	close(clientConnection);
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

	// 5. 接受客户端连接
	struct sockaddr_in clientAddr;  // 客户端的地址信息。
	memset(&clientAddr, 0, sizeof(clientAddr));
	int socklen = sizeof(struct sockaddr_in); // struct sockaddr_in 的大小
	clientConnection = accept(tcpserver, (struct sockaddr*)&clientAddr, (socklen_t*)&socklen);  // 进行通讯 //接受客服端的套接字对象【及保存客户端的IP，port】
	if (-1 == clientConnection) printf("服务器崩溃:%m\n"), close(tcpserver), exit(-2);
	printf("有客户端连接上服务器了: %s!\n", inet_ntoa(clientAddr.sin_addr));
	// 6. 通信
	char buff[1024] = {0};
	while (1)
	{
		int iret = recv(clientConnection, buff, sizeof(buff), 0); // 255表示长度 0表示默认方式 //接收客户端的请求报文
		if (iret > 0)
		{
			buff[iret] = '\0';//添加结束符号
			printf("接收客户端的数据： %s\n", buff);
		}

		strcpy(buff, "ok");
		iret = send(clientConnection, buff, sizeof(buff), 0);// 向客户端发送响应结果。
		if (iret > 0)
		{
			buff[iret] = '\0';//添加结束符号
			printf("发送给客户端：%s\n", buff);
		}
	}
	return 0;
}