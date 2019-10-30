/*
 *了解共享内存的基本操作
 *1.int shmget(key_t key, size_t size, int shmflg);
 *功能是创建共享内存
 *key:共享内存在内核中的标识，其它进程通过相同的标识打开同一个内存
 *size:共享内存大小
 *shmflg: IPC_CREAT / IPC_EXCL | mode
          IPC_CREAT:如果共享内存不存在，则创建一个共享内存，若存在，则打开。
          IPC_EXCL:若共享内存不存在，则创建一个共享内存，若存在，则报错。
          mode:模式
 *成功时返回共享内存的操作句柄，失败时返回-1。
 *
 * 
 * 2.void *shmat(int shmid, const void *shmaddr, int shmflg);
 *功能是用来允许本进程访问一块共享内存的函数（启动对该共享内存的访问）
 *shmid:shmget的返回值
 *shmaddr:如果shmaddr是NULL,系统会自动选择一个合适的地址
 *shmflg:SHM_RDONLY(以只读方式连接)，一般为0，以读写方式连接。
 *成功返回共享内存的首地址，失败返回-1
 *
 *
 * 3.int shmdt(const void *shmaddr);
 *功能是解除映射关系，将shmid_ds结构体中的shm_nattch计数器值减1
 *只是删除本进程对该共享内存的使用。
 *shmaddr: shmat的返回值。
 *也可以使用ipcrm -m shmid的形式删除共享内存，但如果有其他进程
 *在使用共享内存，则不会真正的删除共享内存，但会把共享内存的状态
 *置为dest,该动作是系统维护的。此时共享内存可以使用，当最后一个
 *进程结束或是与共享内存解除映射关系时，共享内存会自动删除。
 *
 * 4.int shmctl(int shmid, int cmd, struct shmid_ds *buf);
 *功能是删除共享内存，但不会立马删除。使shmid_ds结构体中的key变为0x00000000_
 *shmid:shmget的返回值
 *cmd:IPC_STAT / IPC_RMID / SHM_LOCK / SHM_UNLOCK
 *    IPC_STAT:得到共享内存的状态
 *    IPC_SET:改变共享内存的状态
 *    IPC_RMID:删除共享内存
 *buf:是一个结构体指针
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define IPC_KEY 0x12345678
#define SHM_SIZE 4096
int main() 
{ 
   int shmid = shmget(IPC_KEY, SHM_SIZE, IPC_CREAT | 0664);
   if(shmid < 0)
   {
      perror("shmget error\n");
      return -1;
   }
   
   void *shm_start = shmat(shmid, NULL, 0);
   if(shm_start == (void*) -1)//将-1强转为void*类型。
   {
      perror("shmat error\n");
      return -1;
   }
   
   int i = 0;
   while(1)
   {
      sprintf(shm_start, "%s-%d\n", "咚咚咚", i++);
      sleep(1);
   }
   shmdt(shm_start);
   shmctl(shmid, IPC_RMID, NULL);
   return 0;
}

/*
//读取共享内存
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define IPC_KEY 0x12345678
#define SHM_SIZE 4096
int main() 
{ 
   int shmid = shmget(IPC_KEY, SHM_SIZE, IPC_CREAT | 0664);
   if(shmid < 0)
   {
      perror("shmget error\n");
      return -1;
   }
   
   void *shm_start = shmat(shmid, NULL, 0);
   if(shm_start == (void*) -1)//将-1强转为void*类型。
   {
      perror("shmat error\n");
      return -1;
   }
   
   int i = 0;
   while(1)
   {
      printf("%s", (char*) shm_start);
      sleep(1);
   }
   shmdt(shm_start);
   shmctl(shmid, IPC_RMID, NULL);
   return 0;
}

*/
