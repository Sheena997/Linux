/*写一个简单的minishell程序
 * 步骤：
 * 1、一直等待标准输入
 * 2、对输入进行解析，得到程序名称以及运行参数。
 * 3、创建子进程（替换自己的话则运行完命令后shell就退出了，这是不合理的）
 * 4、将子进程程序替换成指定命令对应的程序。
 * 5、父进程等待子进程退出（否则会成为僵尸进程）。
 *
 *
 * */


#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
   while(1)
   {
      //1、等待标准输入
      printf("[sheena@localhost]$ ");
      fflush(stdout);
      char buf[1024] = {0};
      fgets(buf, 1023, stdin);//从标准输入获取用户敲击的命令
      buf[strlen(buf) - 1] = '\0';
      printf("cmd:[%s]\n", buf);


      //2、解析
      int argc = 0;
      char *argv[32] = {NULL};
      char *ptr = buf;
      while(*ptr != '\0')
      {
         if(!isspace(*ptr))
         { 
             argv[argc] = ptr;
             ++argc;
             while(!isspace(*ptr) && *ptr != '\0')
             { 
                ++ptr;
             }
             *ptr = '\0';
         }
         ++ptr;  
      }
      argv[argc] = NULL;
      int i;
      for(i = 0; i < argc; ++i)
      {
          printf("[%s]\n", argv[i]);
      }


      //3、4、创建子进程,并进行程序替换
      int pid = fork();
      if(pid == 0)
      {
          execvp(argv[0], argv);
          exit(0);
      }
    
      //5、等待子进程退出
      waitpid(-1, NULL, 0);
   }
   

   return 0;
}

