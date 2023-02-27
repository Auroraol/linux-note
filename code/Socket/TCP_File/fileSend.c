#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/stat.h> //stat()
#include <fcntl.h>

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
	signal(2, hand);
	// 1 创建socket
	tcpClient = socket(AF_INET, SOCK_STREAM, 0);//参数: 协议域 类型 指定协议
	if (-1 == tcpClient) printf("创建socket失败:%m\n"), exit(-1);
	printf("创建tcpClient_socket成功!\n");
	// 2 创建服务器协议地址簇
	struct sockaddr_in  servAddr;  // 服务端地址信息的数据结构。
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;//协议类型 和socket函数第一个参数一致
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 字符串 转成 整形, 点分十进制的IPv4地址转换成网络字节序列的长整型。  // htonl(INADDR_ANY);  任意ip地址。
	servAddr.sin_port = htons(10860); // 整型 转成 整型 小端转大端
	//3 连接服务器
	int r = connect(tcpClient, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (-1 == r)printf("连接服务器失败:%m\n"), close(tcpClient), exit(-2);
	printf("连接服务器成功!\n");
	// 4. 通信
	int fd = open(argv[1], O_RDONLY);
	if (-1 == fd) printf("打开文件失败:%m\n"), close(tcpClient), exit(-1);
	printf("打开文件成功！\n");
	//获取文件大小
	struct stat st = {0};
	stat(argv[1], &st);
	printf("文件大小为:%d\n", st.st_size);

	r = send(tcpClient, argv[1], strlen(argv[1]), 0);
	printf("发送文件名大小:%d\n", r);
	send(tcpClient, (char*)&st.st_size, 4, 0); // 发送文件大小
	char buff[1024];
	while (1)
	{
		r = read(fd, buff, 1024);
		if (r > 0)
		{
			send(tcpClient, buff, r, 0);
		}
		else
		{
			break;
		}
	}
	printf("发送完毕！\n");
	sleep(5);
	close(fd);
	close(tcpClient);

	return 0;
}