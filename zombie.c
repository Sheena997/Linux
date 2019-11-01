/*
 *修改SIGCHLD信号的默认处理方式
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signum)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);//当waitpid的返回值<=0时结束循环，即当出错或没有子进程退出时结束。
    //相当于使所有的僵尸进程退出。
    /*
     waitpid()
     <0出错
     =0没有子进程退出
     >0有子进程
    */
}

int main()
{
    signal(SIGCHLD, sigcb);
    int pid = fork();
    if(pid == 0)
    {
       //子进程
       sleep(3);
       exit(0);
    }
    //父进程
    while(1)
    {
       printf("sheena/....\n");
       sleep(1);
    }
    return 0;
}

