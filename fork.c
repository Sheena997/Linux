/*************************************************************************
	> File Name: fork.c
	> Author: sheena
	> content:
	> Created Time: Thu 04 Jun 2020 10:19:14 AM CST
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//fork()为当前进程创建一个子进程
//pid_t fork(void)
//返回值：如果是子进程返回0，如果是父进程返回子进程id，出错返回-1
int main()
{
	pid_t pid;

	printf("Before: pid is %d\n", getpid());
	if((pid = fork()) == -1)
		perror("fork()"), exit(1);
	printf("After: pid is %d, fork return %d \n", getpid(), pid);

	sleep(1);

	return 0;
}
