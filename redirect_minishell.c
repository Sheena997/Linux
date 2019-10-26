/*写一个简单的minishell程序实现重定向
 * 步骤：
 * 1、接收标准输入数据
 * 2、对输入进行解析，得到程序名称以及运行参数。（判断是否包含重定向符号>> 或者 >）,
 *    若包含，则认为需要输出重定向，这时候获取重定向符号后边的文件名，将重定向符号替换成'\0'
 *    >为清空重定向
 *    >>为追加重定向
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
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
   while(1)
   {
      
      //1、将原先要写入到的标准输出1中的数据，写入到指定文件中
      fflush(stdout);
      char buf[1024] = {0};
      //ls >> a.txt(该文件为要重定向到的文件)
      int redirect_flag = 0;
      char *ptr =  buf;
      char *file = NULL;
      while(*ptr != '\0')
      {
           if(*ptr == '>')
           {
              redirect_flag = 1;
              *ptr = '\0';
              ++ptr;
              if(*ptr == '>')
              {
                  redirect_flag = 2;
                  ++ptr;
              } 
              while(*ptr == ' ' && *ptr != '\0')
              {
                  ++ptr;
              }
              file = ptr;
              while(isspace(*ptr) && *ptr != '\0')
              {
                  ++ptr;
              }
              *ptr = '\0';
           }
           ++ptr;
      }


      //2、解析
      int argc = 0;
      char *argv[32] = {NULL};
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
          if(redirect_flag == 1)
          {
              int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0664);
              dup2(fd, 1);
          }
          else if(redirect_flag == 2)
          {
              int fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0664);
              dup2(fd, 1);
          }
          execvp(argv[0], argv);
          exit(0);
      }
    
      //5、等待子进程退出
      waitpid(-1, NULL, 0);
   }
   

   return 0;
}
