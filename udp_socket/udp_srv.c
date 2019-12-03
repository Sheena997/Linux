/*************************************************************************
  > File Name: udp_srv.c
  > Author: sheena
  > content:实现用udpsocket类服务器
  > Created Time: Thu 28 Nov 2019 11:35:17 AM CST
 ************************************************************************/
//全文对齐：gg=shift+g

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	//通过参数灵活获取IP地址和端口信息
	if(argc != 3)
	{
		printf("./udp_srv ip port\n");
		return -1;
	}

	//获取本地主机端口号和主机地址
	uint16_t port = atoi(argv[2]);
	char *ip = argv[1];

	//1.创建套接字
	//AF_INET:IPv4套接字，SOCK_DGRAM:套接字类型为用户数据报类型，IPPROTO_UDP:协议为UDP协议
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
	{
		perror("socket create error\n");
		return -1;
	}
	
	//2.绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(addr);
	int ret = bind(sockfd, (struct sockaddr*)&addr, len);
	if(ret < 0)
	{
		perror("bind error\n");
		return -1;
	}



	while(1)
	{
		//3.接收信息
		char buf[1024] = {0};
		struct sockaddr_in cliaddr;
		ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len);
		if(ret < 0)
		{
			perror("recvfrom error\n");
			return -1;
		}
		printf("client say: %s\n", buf);

		//4.发送信息
		printf("server say:");
		fflush(stdout);
		memset(buf, 0x00, 1024);
		scanf("%s", buf);
		ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
		if(ret < 0)
		{
			perror("sendto error\n");
			return -1;
		}
	}

	//5.关闭套接字
	close(sockfd);
	return 0;
}

