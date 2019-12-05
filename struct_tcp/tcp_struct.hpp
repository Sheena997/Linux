/*
 *封装TcpSocket类，通过实例化的tcpsocket对象，来实现数据的通信
 *向外提供简单的接口。
 */
//防止头文件重复包含
#ifndef __My_TCP_H__
#define __My_TCP_H__

#include <iostream>
#include <string>
#include <stdio.h> //perror用到的
#include <unistd.h> //close用到的
#include <netinet/in.h> //放的是各种地址结构体
#include <arpa/inet.h> //inet_addr用到的
#include <sys/socket.h> //套接字接口的头文件

#define CHECK_RET(q) if((q) == false){return -1;}
#define MAX_LISTEN 10//通常该数字不会定义很大，只是限制同一时间的可连接数，而不是最大连接数。

typedef struct
{
	int num1;
	int num2;
	char op;
}cal_t;

class TcpSocket
{
	private:
		int _sockfd;
	private:
		void MakeAddr(struct sockaddr_in &addr, const std::string &ip, const uint16_t port)
		{
			addr.sin_family = AF_INET;//IPv4
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = inet_addr(ip.c_str());
		}
	public:
		TcpSocket()
			:_sockfd(-1)
		{}
		
		int GetFd()
		{
			return _sockfd;
		}
		//1.创建套接字
		bool Socket()
		{
			_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INET:IPv4的地址域，SOCK_STREAM:套接字类型为流式套接字，TPPROTO_TCP:协议为tcp
			if(_sockfd < 0)
			{
				perror("socket error");
				return false;
			}
			return true;
		}

		//2.绑定地址信息
		bool Bind(const std::string &ip, const uint16_t port)
		{
			struct sockaddr_in addr;
			MakeAddr(addr, ip, port);
			socklen_t len = sizeof(struct sockaddr_in);

			int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
			if(ret < 0)
			{
				perror("bind error");
				return false;
			}
			return true;
		}

		//3.开始监听（服务端）
		bool Listen(int backlog = MAX_LISTEN)
		{
			//backlog:决定了同一时间并发连接数，不能写死，采用默认参数方式。
			int ret = listen(_sockfd, backlog);
			if(ret < 0)
			{
				perror("listen error");
				return false;
			}
			return true;
		}

		//3.发起连接请求（客户端）
		bool Connect(const std::string &srvip, const uint16_t port)
		{
			//int connect(int sockfd, struct sockaddr*, socklen_t)
			struct sockaddr_in addr;
			MakeAddr(addr, srvip, port);//服务端的地址信息
			socklen_t len = sizeof(struct sockaddr_in);

			int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
			if(ret < 0)
			{
				perror("connect error");
				return false;
			}
			return true;
		}

		//4.获取新连接（服务端）
		bool Accept(TcpSocket* sock, std::string *ip = NULL, uint16_t *port = NULL)
		{
			//int accept(int sockfd, struct sockaddr* cliaddr, socklen_t len)
			//sockfd:监听套接字的描述符, cliaddr:客户端地址
			//成功返回和客户端通信的套接字的描述符
			struct sockaddr_in addr;
			socklen_t len = sizeof(addr);
			
			//获取新连接的套接字描述符，并且获取这个新连接对应的客户端地址
			int newfd = accept(_sockfd, (struct sockaddr*)&addr, &len);
			if(newfd < 0)
			{
				perror("accept error");
				return false;
			}
			sock->_sockfd = newfd;
			if(ip != NULL)
			{
				*ip = inet_ntoa(addr.sin_addr);//将一个网络字节序的整数ip转换为字符串点分十进制的ip(IPv4);
			}
			if(port != NULL)
			{
				*port = ntohs(addr.sin_port);
			}
			return true;
		}

		//5.接收数据
		bool Recv(std::string &buf)
		{
			//ssize_t recv(int sockfd, char* buf, int len, int flag)
			char tmp[4096] = {0};
			int rlen = recv(_sockfd, tmp, 4096, 0);
			if(rlen < 0)//连接失败
			{
				perror("recv error");
				return false;
			}
			else if(rlen == 0)//连接断开
			{
				std::cerr << "peer shutdown\n";
				return false;
			}

			buf.assign(tmp, rlen);
			return true;
		}

		//6.发送数据
		bool Send(std::string &buf)
		{
			//ssize_t send(int fd, char* buf, int len, int flag)
			int ret = send(_sockfd, &buf[0], buf.size(), 0);
			if(ret < 0)
			{
				perror("send error");
				return false;
			}
			return true;
		}
	
		//7.关闭套接字
		bool Close()
		{
			close(_sockfd);
			return true;
		}
};

#endif //防止头文件重复包含
