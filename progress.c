/*************************************************************************
	> File Name: progress.c
	> Author: sheena
	> content:
	> Created Time: Thu 04 Jun 2020 10:41:58 AM CST
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/*
 * 进程创建
 * pid_t fork(void)
 * 返回值：
 *       如果是子进程返回0，父进程返回子进程pid，出错返回-1
 * 创建一个子进程，拷贝数据时是用写时拷贝技术。父子进程代码共享，数据独有
 *
 * pid_t vfork(void)
 * 创建一个子进程，父子进程共用了同一个虚拟地址空间，为了避免出现调用栈紊，
 * 因此vfork创建子进程之后父进程会阻塞，直到子进程退出或者程序替换
 * 
 *
 *
 * 进程终止
 * void _exit(int status);
 * 参数：
 *       status定义了进程的终止状态，父进程通过wait来获取该值。
 * 该函数为系统调用函数，退出时会直接释放所有资源。
 *
 * void exit(int status)
 * 该函数为库函数接口，在退出前会刷新缓冲区。
 *
 * return退出
 * 在main函数中用return，在退出时会刷新缓冲区。
 *
 *
 * 进程等待：
 * wait方法
 * pid_t wait(int* status)
 * 返回值：
 *       成功返回被等待进程pid，失败返回-1
 * 参数：
 *       输出型参数，获取子进程退出状态。不关心则可以设置为NULL
 *
 *
 * waitpid方法
 * pid_t waitpid(pid_t pid, int* status, int options)
 * 返回值：
 *       当正常返回的时候waitpid返回收集到的子进程pid
 *       如果设置了选项WNOHANG，而调用中waitpid发现没有已经退出的子进程可收集，则返回0
 *       如果调用中出错，则返回-1，这时errno会被设置成相应的值以示错误所在
 * 参数：
 *      pid：
 *         pid == -1，等待任意一个子进程退出，与wait等效
 *         pid > 0  ，等待进程ID为pid的子进程退出
 *      status：
 *         WIFEXITED(status)：若为正常终止子进程返回的状态，则为真（查看子进程是否正常退出）
 *         WEXITSTATUS(status)：若WIFEXITED非0，提取子进程的退出码（查看子进程的退出码）
 *      option：
 *         WNOHANG：若pid指定的子进程没有结束，则waitpid()函数返回0，不予以等待。若正常结束，则返回该退出的子进程的pid
 */
int main()
{
	pid_t pid;//创建进程
	pid = fork();//创建子进程

	if(pid < 0)
	{
		//子进程创建失败
		printf("fork error\n");
		return 1;
	}
	else if(pid == 0)
	{
		//如果是子进程
		printf("child is run, and pid is：%d\n", getpid());
		sleep(5);
		exit(257);
	}
	else
	{
		//如果是父进程
		int status = 0;
		pid_t ret = waitpid(-1, &status, 0);//阻塞式等待，等待5s
		printf("this is for wait!\n");
		if(WIFEXITED(status) && ret == pid)
		{
			printf("wait child 5s success, child return code is：%d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("wait child failed, and return\n");
			return 1;
		}
	}

	return 0; 
}
