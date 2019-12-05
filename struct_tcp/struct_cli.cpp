/*************************************************************************
	> File Name: tcp_cli.cpp
	> Author: sheena
	> content:结构化数据的客户端
	> Created Time: Tue 03 Dec 2019 05:11:43 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
//#include <signal>
#include "tcp_struct.hpp"

//打印异常退出。信号回调函数。为了重定向信号量。
/*
 *void sigcb(int signum)
  {
	   printf("recv a signum : %d\n", signum);
  }
 */
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cerr << "./tcp_cli ip port\n";
		return -1;
	}

	//signal(SIGPIPE, sigcb);

	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
	CHECK_RET(cli_sock.Socket());

	//不推荐主动绑定客户端,需要系统自动绑定，故不需要该步。
	CHECK_RET(cli_sock.Connect(ip, port));

	int sockfd = cli_sock.GetFd();
	
	//发送数据和接收数据
	while(1)
	{
		cal_t tmp;
		tmp.num1 = 11;
		tmp.num2 = 12;
		tmp.op = '+';
		send(sockfd, (void*)&tmp, sizeof(cal_t), 0);
	}

	cli_sock.Close();

	return 0;
}

