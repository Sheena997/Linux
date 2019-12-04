/*************************************************************************
	> File Name: tcp_thread.cpp
	> Author: sheena
	> content:tcp服务端用多线程解决多个执行流的问题。
	  多线程版本的注意事项：通过创建普通线程时候的传参将新获取的套接字传入普通线程中
	                        在普通线程中与客户端进行通信，但是主线程千万不能关闭套接字
							（线程间文件描述符表示共享的，共用同一份，在一个线程中关闭，
							 其它线程也就关闭了）
	> Created Time: Wed 04 Dec 2019 06:03:21 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <iostream>
#include "tcpsocket.hpp"

//通信部分
void *thr_start(void *arg)
{
	TcpSocket *cli_sock = (TcpSocket*)arg;
	while(1)
	{
		std::string buf;
		bool ret = cli_sock->Recv(buf);
		if(ret == false)
		{
			cli_sock->Close();
			break;
		}

		std::cout << "client say : " << buf << "\n";
		std::cout << "server say : ";
		fflush(stdout);
		buf.clear();
		std::cin >> buf;
		ret = cli_sock->Send(buf);
		if(ret == false)
		{
			cli_sock->Close();
			break;
		}
	}

	cli_sock->Close();
	delete cli_sock;//new的空间当其用完之后需要delete，否则会资源泄漏
	return NULL;
}
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "./tcp_srv ip sort\n";
		return -1;
	}

	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());

	while(1)
	{
		//TcpSocket cli_sock;//栈上申请。while循环走完后，资源释放，客户端就找不到
		TcpSocket *cli_sock = new TcpSocket();//堆上申请。不会被释放，每次进来都会new一块新的空间，值也不会被覆盖
		std::string cli_ip;
		uint16_t cli_port;
		if(lst_sock.Accept(cli_sock, &cli_ip, &cli_port) == false)//cli_sock传的是地址
		{
			continue;
		}

		//获取新连接之后，创建线程
		std::cout << "new connect: " << cli_ip << ":" << cli_port << "\n";
		pthread_t tid;
		//通过传参形式或者全局变量将cli_sock传给线程
		pthread_create(&tid, NULL, thr_start, (void*)cli_sock);

		pthread_detach(tid);

		//cli_sock.CClose();//在主线程中不可以，因为线程间是资源共享，主线程将其关了意味着描述符被释放了，其余的线程的描述符与其是同一个。

	}
	lst_sock.Close();

	return 0;
}


