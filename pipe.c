/*
 *简单的父子进程了解匿名管道的使用。
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //先创建管道，管道要在子进程之前创建
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret < 0)
    {
        printf("pipe error\n");
        return -1;
    }
    
    //创建子进程
    int pid = 0;
    pid = fork();
    if(pid == 0)
    {
       //子进程，子进程读取管道数据。
       char buf[1024] = {0};
       int ret = read(pipefd[0], buf, 1023);
       if(ret < 0)
       {
           printf("read error\n");  
           return -1;
       }
       printf("child read data:[%s]\n", buf);
    }
    else
    {
       //父进程，父进程向管道输入数据。
       char *ptr = "sheena is tired";
       int ret = write(pipefd[1], ptr, strlen(ptr));
       if(ret < 0)
       {
           printf("write error\n");
           return -1;
       }
    }
    return 0;
}
