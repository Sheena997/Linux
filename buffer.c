/*
 *缓冲区的体会
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{

   printf("printf");
   fwrite("fwrite", 6, 1, stdout);
   fprintf(stdout, "fprintf");
   write(1, "write", 5);
   sleep(3);
   return 0;
}

/*
 *结果为write直接打印，3秒钟之后printffwritefprintf才答应。
  因为write是系统调用接口打印的，意味着，系统调用接口向文件写入数据的时候是直接写入到文件，没有写到缓冲区。
  缓冲区是标准库接口里的缓冲区，每个文件都有缓冲区，其实缓冲区在文件流指针中。
 *
*/