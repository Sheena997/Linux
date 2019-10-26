/*
 *exec家族 程序替换接口
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
   //execl("/bin/ls", "ls", "-l", "-a", NULL);//第一种
   char *argv[32] = {NULL};
   argv[0] = "ls";
   argv[1] = "-a";
   argv[2] = NULL;
   /*if(execvp("ls", argv < 0))//带p和不带p的区别：在于第一个参数，可执行文件的路径名。
   {
       perror("execv error");
       return -1;
   }*/
   if(execve("./env", argv, NULL) < 0)
//带e和不带e的区别：新程序的环境变量是否由自己来赋值，带e是自己设置的环境变量，不带e是原来的环境变量，子程序的环境变量实际上是父进程给的。
   {
       perror("exeve error");
       return -1;
   }  
   printf("上句代码是将printf函数的功能替换成ls -l -a的功能！");
   return 0;
}
