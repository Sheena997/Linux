/*дһ���򵥵�minishell����ʵ���ض���
 * ���裺
 * 1�����ձ�׼��������
 * 2����������н������õ����������Լ����в��������ж��Ƿ�����ض������>> ���� >��,
 *    ������������Ϊ��Ҫ����ض�����ʱ���ȡ�ض�����ź�ߵ��ļ��������ض�������滻��'\0'
 *    >Ϊ����ض���
 *    >>Ϊ׷���ض���
 * 3�������ӽ��̣��滻�Լ��Ļ��������������shell���˳��ˣ����ǲ�����ģ�
 * 4�����ӽ��̳����滻��ָ�������Ӧ�ĳ���
 * 5�������̵ȴ��ӽ����˳���������Ϊ��ʬ���̣���
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
      
      //1����ԭ��Ҫд�뵽�ı�׼���1�е����ݣ�д�뵽ָ���ļ���
      fflush(stdout);
      char buf[1024] = {0};
      //ls >> a.txt(���ļ�ΪҪ�ض��򵽵��ļ�)
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


      //2������
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


      //3��4�������ӽ���,�����г����滻
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
    
      //5���ȴ��ӽ����˳�
      waitpid(-1, NULL, 0);
   }
   

   return 0;
}
