/*
 * 通过匿名管道实现管道符
 * ps -ef | grep ssh
 * ps和grep进程是兄弟进程，ps进程将结果交给grep进程使用进程间通信---匿名管道
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
   //首先创建管道
   int pipefd[2] = {0};  
   int ret = pipe(pipefd);
   if(ret < 0) 
   {
       perror("pipe error\n");
       return -1;
   }

   //创建两个子进程
   int pid1 = fork();//子进程1实现ps
   if(pid1 == 0)
   {
      //子进程1
      close(pipefd[0]);//写的时候关闭读端
      dup2(pipefd[1], 1);
      //1为标准输出，将标准输出重定向到pipefd[1](管道写入端)，这样从标准输出中读数据就可以写入到管道中
      execlp("ps", "ps", "-ef", NULL);//程序替换
      exit(0);
   }
   else if(pid1 < 0)
   {  
      perror("fork error\n");//父进程，子进程创建失败
      return -1;
   }
   int pid2 = fork();//子进程2实现grep
   if(pid2 == 0)
   {
      //子进程1
      close(pipefd[1]);//读的时候关闭写端
      dup2(pipefd[0], 0);
      //0为标准输入，将标准输入重定向到pipefd[0](管道读取端)，这样从标准输入中写数据就可以从管道中读取到
      execlp("grep", "grep", "ssh", NULL);//程序替换
      exit(0);
   }
   else if(pid2 < 0)
   {  
      perror("fork error\n");//父进程，子进程创建失败
      return -1;
   }
   close(pipefd[0]);
   close(pipefd[1]);//管道是在父进程创建的，父进程中也要关闭写端和读端。
   waitpid(pid1, NULL, 0);
   waitpid(pid2, NULL, 0);//等待子进程退出，防止产生僵尸进程。
   return 0; 
}
