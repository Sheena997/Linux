/*
 *认识系统调用接口
 *
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
   int fd = -1;

   umask(0); 
   fd = open("./test.txt", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0777);
   //open(char *filename, int flag, mode_t mode)
   if(fd < 0)
   {
       perror("open error");
       return -1;
   }

   char ptr[1024] = "初学好难啊！！！\n";
   int ret = write(fd, ptr, strlen(ptr));
   //ssize_t write(int fd, void *buf, size_t count)
   //fd:open返回的文件描述符
   //成功时返回写入数据的字节数
   if(ret < 0)
   {
      perror("write error");
      close(fd);
      //int close(int fd);
      //成功时返回0，失败返回-1，读写之后都要关闭文件。
      return -1;
   }
  
   lseek(fd, 0, SEEK_SET);
   //off_t lseek(int fd, off_t offset, int whence)
   //offsetsh:偏移量
   //whence:有三种，SEEK_SET:文件开始位置，SEEK_END:文件结束位置，SEEK_CUR:文件当前位置
   //在读写文件时，写入文件后，需要用lseek将文件指针指向文件开始位置，否则会从文件最后位置开始读。

   char buf[1024] = {0};
   ret = read(fd, buf, 1023);
   //ssize_t read(int fd, void *buf, size_t count)
   //成功时返回实际读取数据的字节数,返回0则表示读取到来了文件末尾
   if(ret < 0)
   {
      perror("read error");
      close(fd);
      return -1;
   }
   else if(ret == 0)
   {
      perror("at the end of file");
      close(fd);
      return -1;
   }
   printf("buf:[%s]-%d\n", buf, ret);
   close(fd);
   return 0;
}
