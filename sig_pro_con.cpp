/*
 *使用信号量实现一个线程安全的环形队列。
 *
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <semaphore.h>

#define MAX_QUE 10
class RingQueue
{
  private:
    std::vector<int> _queue;//使用数组实现环形队列
    int _capacity;//初始化环形队列的节点数量
    int _step_read;//当前读位置的数组下标
    int _step_write;//当前写位置的数组下标
    sem_t _sem_lock;//用于实现互斥的锁
    sem_t _sem_con;//消费者的等待队列，计数器对有数据的空间进行计数。
    sem_t _sem_pro;//生产者等待队列，计数器对空闲空间进行计数。

  public:
    RingQueue(int max_que = MAX_QUE)
      :_capacity(max_que)
      ,_queue(max_que)
      ,_step_read(0)
      ,_step_write(0)
    {
        //初始化信号量
        sem_init(&_sem_lock, 0, 1);//该锁是用于实现线程间互斥的故第二个参数为0，若信号量要实现互斥只有一个0或1的计数器，要是让线程可以访问，开始为1。
        sem_init(&_sem_con, 0, 0);//实现用于同步的一个信号量，消费者的等待队列对有数据的空间进行计数，用于线程间，一开始记为0，没有数据
        sem_init(&_sem_pro, 0, max_que);//实现用于同步的一个信号量，生产者的等待队列对空闲空间进行计数，就是当前还有多少结点可用，用于线程间。 
    }
    ~RingQueue()//销毁信号量
    {
        sem_destroy(&_sem_lock);
        sem_destroy(&_sem_con);
        sem_destroy(&_sem_con);
    }
    bool QueuePush(int &data)
    {
        //数据入队
        //1.判断是否能够访问临界资源（判断空闲空间计数是否大于0）
        sem_wait(&_sem_pro);
        //2.加锁
        sem_wait(&_sem_lock);
        //3.数据的入队操作
        _queue[_step_write] = data;
        _step_write = (_step_write + 1) % _capacity;//指向下一个位置。
        //4.解锁
        sem_post(&_sem_lock);
        //5.数据资源的空间计数+1.唤醒消费者
        sem_post(&_sem_con);
        return true;
    }
    bool QueuePop(int &data)
    {  
        sem_wait(&_sem_con);
        sem_wait(&_sem_lock);
        data = _queue[_step_read];
        _step_read = (_step_read + 1) % _capacity;
        sem_post(&_sem_lock);
        sem_post(&_sem_pro);
        return true;
    }
};

#define MAX_THREAD 4

void* consumer(void* arg)
{
   RingQueue *queue = (RingQueue*)arg;
   while(1)
   {
      int data;
      queue->QueuePop(data);
      printf( "consumer get a data:%d\n", data);
   }
   return NULL;
}
void* productor(void* arg)
{
   int i = 0;
   RingQueue *queue = (RingQueue*)arg;
   while(1)
   {
      queue->QueuePush(i);
      printf( "productor put a data:%d\n",  (i++));
   }
   return NULL;
}

int main()
{
   pthread_t ctid[MAX_THREAD], ptid[MAX_THREAD];
   RingQueue queue;   

   int i = 0;
   for(i = 0; i < MAX_THREAD; ++i)
   {
      int ret = pthread_create(&ctid[i], NULL, consumer, (void*)& queue);
      if(ret != 0)
      {
          std::cerr << "thread create error\n";
          return -1;
      }
   }
   for(i = 0; i < MAX_THREAD; ++i)
   {
      int ret = pthread_create(&ptid[i], NULL, productor, (void*)& queue);
      if(ret != 0)
      {
          std::cerr << "thread create error\n";
          return -1;
      }
   }

   for(i = 0; i < MAX_THREAD; ++i)
   {
      pthread_join(ctid[i], NULL);
      pthread_join(ptid[i], NULL);
   }
   return 0;
}

