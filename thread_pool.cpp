/*
 *封装一个固定数量线程的线程池
 *用一个以上的线程和线程安全的任务队列实现线程池
 *
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <stdlib.h>

//任务类
typedef void (*TaskHandler)(int data);//定义一个函数指针类型
class ThreadTask
{
 private:
   int _data;//要处理的数据
   TaskHandler _handler;//如何处理数据的函数
 public:
   ThreadTask()
   {}
   ThreadTask(int data, TaskHandler handler)
     :_data(data)
     ,_handler(handler)
   {}
   void SetTask(int data, TaskHandler handler)
   {
      _data = data;
      _handler = handler;
   }
   void Run()//向外提供的接口，线程池里调用的接口
   {
      _handler(_data);
   }
   
};


//线程池里的线程唯一目的就是从队列中拿到一个任务去Run
//线程中去任务pop(tt), 去Run就是tt.Run();
//线程池只管取出任务去处理，要处理什么数据如何处理和线程池无关是任务类的事


#define MAX_THR 5
//线程池类
class ThreadPool
{
 private:
   int _thr_num;//线程池中线程的最大数量
   std::queue<ThreadTask> _task_queue;//任务队列
   pthread_mutex_t _mutex;//保护队列安全操作
   pthread_cond_t _pro;//实现队列同步操作，任务的生产者
   pthread_cond_t _con;//实现队列同步操作，任务的消费者

 private:
   static void *thr_start(void *arg)//若不加static的话该函数有两个参数，前面还有一个this。但是该函数必须只有一个void*的参数，所以加static
   {
      ThreadPool* pool = (ThreadPool*) arg; //此时加上static就可以访问公有成员函数了
      //出队
      //先加锁然后判断任务队列中是否有任务
      while(1)
      {
          //pthread_mutex_lock(&_mutex);//无法访问私有成员
          pool->ThreadLock();
          while(/*_task_queue.empty()*/pool->TaskIsEmpty())
          {
            // pthread_cond_wait(&_con, &_mutex);
            pool->ThreadWait();
          }
          ThreadTask tt;
          /*tt = _task_queue.front();
          _task_queue.pop();*/
          pool->QueuePop(tt);
         //pthread_mutex_unlock(&_mutex);
          pool->ThreadUnlock();
          tt.Run();
      }
      return NULL;
   }
  
 public://向static提供公有接口来完成对私有成员的访问
   bool QueuePop(ThreadTask &tt)
   {
      tt = _task_queue.front();
      _task_queue.pop();
      return true;
   }
   bool TaskIsEmpty()
   {
      return _task_queue.empty();
   }
   void ThreadLock()
   {
      pthread_mutex_lock(&_mutex);
   }
   void ThreadUnlock()
   {
      pthread_mutex_unlock(&_mutex);
   }
   void ThreadWakeup()
   {
      pthread_cond_signal(&_con);
   }
   void ThreadWait()
   {
      pthread_cond_wait(&_con, &_mutex); 
   }

 public:
   ThreadPool(int max_thr = MAX_THR)
      :_thr_num(max_thr)
   {}
   bool PoolInit()
   {
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_pro, NULL);
      pthread_cond_init(&_con, NULL);//初始化一定要在创建线程的前面
      //创建线程
      int i = 0;
      for(i = 0 ; i < _thr_num; ++i)
      {  
         pthread_t tid;
         int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
         if(ret != 0)
         {
            std::cerr << "create thread error\n";
            return false;
         }
         pthread_detach(tid);//将一个线程分离出去，分离之后释放之后自动回收资源，防止其成为僵尸线程
      }  
      return true;
   }
   //入队一个任务进线程池中
   bool TaskPush(ThreadTask &tt)//入队
   {  
      pthread_mutex_lock(&_mutex);//保证线程安全加锁
      //任务入队
      _task_queue.push(tt);
      pthread_mutex_unlock(&_mutex);
      pthread_cond_signal(&_con);//入队完成之后，唤醒消费者（唤醒线程池里的线程。）
      return true;
   }
   
};

void test(int data)
{
   srand(data);
   int sec = rand()%5;
   printf("thread:%p----get data:%d, sleep %d sec\n",pthread_self(), data, sec);
   sleep(sec);
}
int main()
{ 
   ThreadPool pool;
   pool.PoolInit();
   for(int i = 0; i < 10; ++i)
   {
      ThreadTask task(i,test);
      pool.TaskPush(task);
   }
   while(1)
   {
      sleep(1);
   }
   return 0;
}
