/*
 *创建一个线程。(在一个程序里包含多个死循环)
 *线程终止：1.return（main函数中的return是退出进程）
 *          2.void pthread_exit(void *retval);调用该函数接口的线程退出
 *          3.void pthread_cancel(pthread_t tid);取消一个指定的线程
 *线程等待。pthread_join(pthread_t tid, void** retval);
 *线程分离：pthread_detach(pthread_t tid);
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void *thre_start(void *arg)
{
    pthread_detach(pthread_self());//线程分离自己
   //return NULL;//在线程函数中使用return，线程退出
    char *ptr = "sheena return \n";
    pthread_exit(ptr);//线程等待时
   // pthread_exit(NULL);//线程退出方式2，调用该函数接口的线程退出
    while(1)
    {
       printf("child thread---%s\n", (char*) arg);
       sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid; 
    char *ptr = "so sleepy!";
    int ret = pthread_create(&tid, NULL, thre_start, (void *)ptr);
    if(ret != 0)
    { 
         fprintf(stderr, "pthread_create: %s\n", strerror(ret));
         return -1;
    }
    
    pthread_detach(tid);//分离指定线程，该线程被分离后不能被等待：
    char *retval;
    pthread_join(tid, (void**)&retval);//线程等待。
    printf("retval: %s\n", retval);

    //pthread_exit(NULL);//在主线程中调用该接口退出，主线程变成了僵尸线程，其余线程依然运行，进程变为僵尸状态，因为查看的进程信息是主线程信息
   // sleep(5);
    //pthread_cancel(tid);//指定tid该线程退出。 
    while(1)
    {
        printf("main thread----%d\n", tid);
        sleep(1);
    }
    return 0;
}
