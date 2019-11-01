/*
 *信号的阻塞
 *int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
 *how:   SIG_BLOCK : blocked | set
 *       SIG_UNBLOCK : blocked & (~set)
 *       SIG_SETMASK : blocked = set
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigcb(int signum)//为了方便查看获取了哪些信号
{
   printf("recieve signal: %d\n", signum);
}

int main()
{
   signal(SIGINT, sigcb);//非可靠信号
   signal(40, sigcb);//可靠信号
   
   sigset_t mask_set, old_set;
   sigemptyset(&mask_set);//清空信号集合
   sigfillset(&mask_set);//添加所有信号到集合中
   sigprocmask(SIG_BLOCK, &mask_set, &old_set);//阻塞mask_set所有信号
   
   printf("press enter to continue\n");
   getchar();//获取回车，不按回车则程序的流程卡在此处
   sigprocmask(SIG_UNBLOCK, &mask_set, NULL);//解除阻塞
   return 0;
}
