/*************************************************************************
	> File Name: tcp_srv.cpp
	> Author: sheena
	> content:服务器和客户端用tcp进行连接通信的代码
	> Created Time: Tue 03 Dec 2019 04:53:23 PM CST
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "tcp_struct.hpp"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "./tcp_srv ip port\n";
		return -1;
	}

	//通过参数获取地址信息
	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

	TcpSocket lst_sock;//实例化对象，监听套接字。
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));

	//3、开始监听
	CHECK_RET(lst_sock.Listen());

	TcpSocket cli_sock;
	//4、获取新连接
	while(1)
	{
		//cli_sock用于与指定客户端进行通信
		//lst_sock只用于获取新连接
		std::string cli_ip;
		uint16_t cli_port;
    	if(lst_sock.Accept(&cli_sock, &cli_ip, &cli_port) == false)//获取新连接,Accept完之后cli_sock里放的是新接下来与客户端通信的套接字:
		{
			//获取新连接失败后，继续获取下一个连接
			continue;
		}
		int sockfd = cli_sock.GetFd();

		cal_t tmp;
		recv(sockfd, (void*)&tmp, sizeof(cal_t), 0);

		printf("%d %c %d\n", tmp.num1, tmp.op, tmp.num2);
	}
	lst_sock.Close();
	return 0;
}


