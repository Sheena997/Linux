/*
 *�����������
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
 *���Ϊwriteֱ�Ӵ�ӡ��3����֮��printffwritefprintf�Ŵ�Ӧ��
  ��Ϊwrite��ϵͳ���ýӿڴ�ӡ�ģ���ζ�ţ�ϵͳ���ýӿ����ļ�д�����ݵ�ʱ����ֱ��д�뵽�ļ���û��д����������
  �������Ǳ�׼��ӿ���Ļ�������ÿ���ļ����л���������ʵ���������ļ���ָ���С�
 *
*/