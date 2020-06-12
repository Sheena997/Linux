/*
 * 封装一个select类，通过实例化的对象就能完成select简单操作
 */

#include <stdio.h>
#include <vector>
#include <sys/select.h>
#include "tcpsocket.hpp"

#define MAX_TIMEOUT 3000

class Select
{
	public:
		Select()
			:_maxfd(-1)
		{
			FD_ZERO(&_rfds);
		}
		bool Add(TcpSocket& sock)//添加描述符的监控
		{
			//获取到套接字描述符
			int fd = sock.GetFd();
			//添加到事件的描述符集合中
			FD_SET(fd, &_rfds);
			//判断重新确定当前集合中的最大描述符
			_maxfd = _maxfd > fd ? _maxfd : fd;
			return true;
		}
		bool Del(TcpSocket& sock)//移除描述符的监控
		{
			//获取到套接字秒描述符
			int fd = sock.GetFd();
			//从将集合中移除指定的描述符
			FD_CLR(fd, &_rfds);
			//判断重新确定当前最大的描述符
			if(fd != _maxfd)
				return true;
			//假设集合中以前最大的是8，8移除之后，从7开始判断，还在集合中的第一个就是最大的
			for(int i = _maxfd - 1; i >= 0; --i)
			{
				if(FD_SET(i, &_rfds))
				{
					_maxfd = i;
					break;
				}
			}
		}

		//开始监控，转接向外部返回就绪的TcpSocket
		bool Wait(std::vector<TcpSocket>* list, int timeout)
		{
			//select开始监控，定义超时时间，添加到描述符到集合中
			struct timeval tv;
			tv.tv_sec = timeout / 1000;//以毫秒为单位
			tv.tv_usec = (timeout % 1000) * 1000;
			fd_set tmp_set = _rfds;//每次使用临时的集合进行监控
			int ret = select(_maxfd + 1, &_tmp_set, NULL, NULL, &tv);

			if(ret < 0)
			{
				perror("监控错误select error !\n");
				return false;
			}
			else if(ret == 0)
			{
				list->clear();//清空超时等待的对象
				printf("超时等待！wait timeout! /n");
				return true;
			}
			
		
			//判断哪些描述符就绪了，组织TcpSocket对象，添加到list中
			//从0到max_fd逐个开始进行判断，哪个数字在集合中哪个数据就是就绪的对象的描述符
			for(int i = 0; i <= _maxfd; ++i)
			{
				if(!FD_ISSET(i, &tmp_set))
					continue;
				TcpSocket sock;
				sock.SetFd(i);
				list->push_back(sock);
			}
			return true;
		}
	private:
		fd_set _rfds;//可读事件的描述符集合
		int _maxfd;//保存集合每次集合操作后的最大的描述符

};
