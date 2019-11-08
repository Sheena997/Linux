/*
 *通过黄牛抢票的例子认识互斥锁以及体会线程安全
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THR 4
int tickets = 100;//100张票
pthread_mutex_t mutex;//定义互斥锁变量。

void *cattle(void *arg)
{
    while(1)
    {
         pthread_mutex_lock(&mutex);//互斥锁加锁的位置要在第一次访问临界资源之前。阻塞加锁，加锁失败就阻塞。
         //pthread_mutex_trylock(&mutex);尝试加锁，要是加锁失败立即报错直接返回。非阻塞加锁
         //pthread_mutex_timedlock(&mutex);限时等待加锁，超时之和立即报错返回。限制阻塞时长的阻塞加锁。
         if(tickets > 0)
         {
             printf("%p:get a ticket:%d\n", pthread_self(), tickets);
             usleep(1000);
             --tickets;
             pthread_mutex_unlock(&mutex);//互斥锁只保护临界资源，若临界资源使用完之后就要解锁。
         }
         else 
         {
             printf("there are no tickets\n");
             pthread_mutex_unlock(&mutex);//加锁之后要在任意有可能退出线程的地方进行解锁。
             pthread_exit(NULL);//退出线程
         }
         sleep(1);//给时间片一个缓冲的时间，让其他黄牛也可以抢到票
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid[MAX_THR];
   
    pthread_mutex_init(&mutex, NULL);//使用之前要初始化。第二个参数互斥锁属性一般置空。

    int i;
    for(i = 0; i < MAX_THR; ++i)
    {
         int ret = pthread_create(&tid[i], NULL, cattle, NULL);//线程创建
         if(ret != 0)
         {
             printf("thread create error!\n");
             return -1;
         }
    }
    for(i = 0; i < MAX_THR; ++i)
    {
         pthread_join(tid[i], NULL);//等待线程退出
    }
    pthread_mutex_destroy(&mutex);//当使用完后就销毁
    return 0; 
}
