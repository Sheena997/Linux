/*
 *用静态成员实现单例模式的饿汉方式。
 *一个类中不管实例化多少对象，静态成员的空间只有一个
 *
 * 用指针实现单例模式的懒汉方式
 */

#include <iostream>
#include <stdio.h>
#include <mutex>


/* 饿汉方式的实现
class Test 
{
   private:
      static int data;
   public:
      int *get_instance()
      {
         return &data;
      }
};
*/
//懒汉方式的实现
std::mutex lock;
class Test
{
  private:
    volatile static int *data;//volatile:防止编译器过度优化
  public:
     volatile int *get_instance()
     {
       if(data == NULL)//减小锁冲突的概率，让data为NULL的线程才等待到锁上。
       {
         lock.lock();//C++中线程封装的一个锁的接口
         if(data == NULL)//临界资源
         {
             data = new int;
             *data = 4;
         }
         lock.unlock();
       }
       return data;
     }
};
//int Test::data = 10;

volatile int *Test::data = NULL;

int main()
{
   Test a, b;
   
   printf("%d-%p\n", *a.get_instance(), a.get_instance());
   printf("%d-%p\n", *b.get_instance(), b.get_instance());
}
