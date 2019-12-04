/*************************************************************************
	> File Name: tcp_process.cpp
	> Author: sheena
	> content:服务器的改进：加父子进程
	          1.多进程注意：进程等待，使用信号回调，父进程不必等待子进程退出了。
	          2.等待新连接的父进程，在每次获取新连接创建子进程之后，都需要将新套接字关闭。
			  3.因为父子进程代码共享、数据独有，描述符各自有一个（各自都有一个cli_sock，父进程只负责接收新连接，不涉及与客户端进行通信）
			  4.父进程保留该描述符毫无意义，反而容易造成资源泄漏。
	> Created Time: Wed 04 Dec 2019 10:43:40 AM CST
 ************************************************************************/
/*
 *当前的服务器端程序只有一个执行流，但是这个执行流却包含多种功能操作（accept，recv，send），
 *而每一种功能都有可能造成执行流阻塞。
 *故可采取使用多进程/多线程进行多任务处理，每个执行流只负责一个功能。
 */
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

//加信号量等待：修改SIGCHLD信号的处理方式，一次性将所有的僵尸进程全部处理完毕
void sigcb(int signo)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "./tcp_process ip port\n";
		return -1;
	}
	signal(SIGCHLD, sigcb);//操作系统在子进程退出后用SIGCHLD信号通知父进程

	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());

	while(1)
	{
		TcpSocket cli_sock;
		bool ret = lst_sock.Accept(&cli_sock);
		if(ret == false)
		{
			continue;
		}
		std::cout << "new connect\n";

		//创建子进程与客户端进行通信
		//父进程永远只管一件事，获取新连接
		pid_t pid = fork();
		if(pid < 0)
		{
			cli_sock.Close();
			continue;
		}
		else if(pid == 0)
		{
			//子进程
			while(1)
			{
				//死循环：一个进程只与一个客户端通信
				std::string buf;
				cli_sock.Recv(buf);
				std::cout << "Client say:" << buf << "\n";
				std::cout << "server say :";
				fflush(stdout);
				buf.clear();
				std::cin >> buf;
				cli_sock.Send(buf);
			}
			cli_sock.Close();
			exit(0);
		}
		//父进程必须关闭sock：父子进程数据独有，父进程的关闭不影响子进程
		//若不关闭，则会造成资源泄露到最忌没有描述符了（被用完了）
		cli_sock.Close();
	}
	lst_sock.Close();
	return 0;
}

