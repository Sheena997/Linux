/*
 *修改信号处理接口的代码。
 *接口1：signal接口
 *接口2：sigaction(int signum, const struct sigatcion *act, struct sigaction *oldact)
 *       signum:信号值
 *       *act:信号新动作，signum当前要修改的新动作
 *       *oldact:信号旧动作，用于获取signnum信号原有的动作（便于以后还原回去。）
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

struct sigaction oldact;
void sigcb(int signum)
{
   printf("recieve a singnal:%d\n", signum);
   sigaction(signum, &oldact, NULL);//在回调函数中将该信号还原回原来的信号处理方式
}
int main()
{
    
   //signal(2, SIG_IGN);//将2号信号（Ctrl+c）原本的中断处理修改为忽略处理。
   signal(3, sigcb);//将3号信号（Ctrl+|）的处理修改为自定义处理，改为进出sigcb函数。
  
   //常用
   //定义
   struct sigaction newact;
   struct sigaction oldact;
   newact.sa_handler = sigcb;
   newact.sa_flags = 0;//等于0（默认）的情况下，使用的是sa_handler该回调函数
   //newact.sa_mask;//临时要阻塞的信号集合，当前要处理的不希望收到其它信号影响。
   sigemptyset(&newact.sa_mask);//清空临时要阻塞的信号集合
   
   //使用
   sigaction(2, &newact, &oldact);
   while(1)
   {
       printf("sheena\n");
       sleep(10);
   }
   return 0;
}

