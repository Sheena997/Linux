/*
 *一个简单的进度条程序
 *操作符‘\n’ 针对标准输出时，除了换行之外还有刷新缓冲区的作用。
 *‘\r’：回车符，让光标移动到起始位置。
 *
 */


#include <stdio.h>
#include <unistd.h>

int main()
{
    char buf[11] = {0};
    int i = 0;
    for(; i < 10; ++i)
    {
        buf[i] = '-';
        printf("%s\r", buf);
        fflush(stdout);
        sleep(1);
    }
    return 0; 
}

