/*
 *实现一个UdpSocket类封装udp常用操作
 */
//gg = shirft+g(全文对齐)
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class UdpSocket
{
	private:
		//封装一个地址结构
		void MakeAddr(struct sockaddr_in &addr, const std::string &ip, const uint16_t port)
		{
			addr.sin_family = AF_INET;//地址域
			//uint32_t htonl(uint32_t hostlong);将4个字节的数据主机字节序转换成网络字节序
			//uint16_t htons(uint16_t hostshort);将2个字节的数据主机字节序转换网络字节序
			//uint32_t ntohl(uint32_t netlong);将网络字节序转换成主机字节序
			//uint16_t ntohs(uint16_t netlong);将网络字节序转换成主机字节序
			addr.sin_port = htons(port);//端口，port是主机字节序，将一个主机字节序转换成网络字节序的地址
			//将点分十进制字符串转换成网络字节序的IP地址
			//in_addr_t inet_addr(const char* p)
			//int inet_pton(int af, const char* src, void dst);
			//
			//将网络字节序地址转换成字符串点分十进制IP地址
			//const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
			//char* inet_nota(struct in_addr in);
			addr.sin_addr.s_addr = inet_addr(ip.c_str());//inet_addr:将一个点分十进制转换成网络字节序的IP地址
		}
	public:
		UdpSocket()
			:_sock(-1)
		{}
		~UdpSocket()
		{}
		bool Socket()
	    {
			//1、创建套接字
			//int socket(int domain, int type, int protcocol);IPv4地址域:AF_INET, 套接字类型:SOCK_DGRAM(用户数据报套接字)，协议:IPPROTO_UDP(UDP协议)
			_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//创建失败，返回值为-1。
			if(_sock < 0)
			{
				perror("socket error\n");
				return false;
			}
			return true;
		}


		bool Bind(const std::string &ip, const uint16_t port)
		{
			//2、绑定地址信息
			//int bind(int sockfd, struct sockaddr *addr. socklen_t addrlen);
			//sockfd:套接字描述符，addr:地址信息(IPv4是struct addr_in该结构体)
			struct sockaddr_in addr;
			MakeAddr(addr, ip, port);
			socklen_t len = sizeof(struct sockaddr_in);
		
		    int ret = bind(_sock, (struct sockaddr*)&addr, len);
			if(ret < 0)
			{
				perror("bind error\n");
				return false;
			}
			return true;
		}

		bool Recv(std::string &buf, std::string  *ip = NULL, uint16_t *port = NULL)
		{
			//3、接收信息
			//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
			//sockfd:套接字描述符，buf:接收信息的缓冲区，len:要接收的数据的长度，flag:参数0为阻塞，src_addr:对端的地址信息，*addrlen:地址信息长度
			struct sockaddr_in addr;
			char tmp[1500] = {0};
			socklen_t len = sizeof(struct sockaddr_in);
			int ret = recvfrom(_sock, tmp, 1500, 0, (struct sockaddr*)&addr, &len);
			if(ret < 0)
			{
				perror("recvfrom error\n");
				return false;
			}

			buf.assign(tmp, ret);
			if(ip != NULL)
			{
				//inet_ntoa将网络字节序整数IP地址转换为字符串IP地址
				*ip = inet_ntoa(addr.sin_addr);
			}
			if(port != NULL)
			{
				*port = ntohs(addr.sin_port);
			}
			return true;
		}

		bool Send(const std::string &buf, std::string &dip, uint16_t dport)
		{
			//4、发送信息
			//ssize_t sendto(int sockfd, const void* buf, size_t len, int flag, struct sockaddr* dest_addr, socklen_t addrlen);
			//sockfd:套接字描述符，buf:要发送的数据的缓冲区，len:要发送的数据长度，flag:0为阻塞发送，dest_addr:要发送的地址信息，addrlenL:地址信息长度
			struct sockaddr_in addr;
			MakeAddr(addr, dip, dport);
			socklen_t len = sizeof(struct sockaddr_in);
			//ret用于获取实际发送的数据长度
			int ret = sendto(_sock, &buf[0], buf.size(), 0, (struct sockaddr*)&addr, len);
			if(ret < 0)
			{
				perror("sendto error\n");
				return false;
			}
			return true;
		}
		bool Close()
		{
			//5、关闭套接字
			//close(int sockfd);
			close(_sock);
			_sock = -1;
		}
	
	private:
		int _sock;
};
