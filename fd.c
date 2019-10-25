/*
 *文件描述符和重定向的简单认识 
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
   //int dup2(int oldfd, int newfd);
   //将newfd重定向到oldfd所指向的文件
   //若newfd本身已有打开文件，重定向时则关闭已打开文件
   
   int fd = open("./tmp.txt", O_RDWR);

   dup2(fd, 1);//将1号描述符重定向到fd中，将1号描述符下标中的内容，改变成fd下标中的内容。
   //1号原本指向标准输出文件，当dup2语句使用过之后，1号指向tmp.txt，关闭标准输出文件。
   printf("%d\n", fd);  
   fflush(stdout);
   close(fd);
   return 0;
}
