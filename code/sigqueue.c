#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int pid = atoi(argv[1]);//字符串转整数
	int sig = atoi(argv[2]);
	union sigval v;
	v.sival_int = 100;  //	或者v.sival_ptr
	sigqueue(pid, sig, v);  // 发送信号
	return 0;
}