#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define STD_IN 0
#define MAXLINE 512 // max exec arguments 32

int main(int argc, char *argv[])
{
    char *cmd = argv[1]; // 命令

    char line[MAXLINE];     // 每行数据
    char *new_argv[MAXARG]; // 新的参数，用于 exec
    int n, args_index = 0;

    int i;
    for (i = 1; i < argc; i++)
    {
        new_argv[args_index++] = argv[i];
    }

    while ((n = read(STD_IN, line, MAXLINE)) > 0)
    {
        if (fork() == 0) // 子进程执行 exec
        {
            // 存储每个空格和\n间隔的字符串
            char *buf = (char *)malloc(sizeof(line));
            int index = 0;
            for (i = 0; i < n; i++)
            {
                if (line[i] == ' ' || line[i] == '\n')
                {   // 存储一个字符串
                    buf[index] = 0;
                    new_argv[args_index++] = buf;
                    index = 0;
                    buf = (char *)malloc(sizeof(line));
                }
                else
                {
                    buf[index++] = line[i];
                }
            }
            buf[index] = 0;
            new_argv[args_index] = 0;
            exec(cmd, new_argv);
        }
        else
        {
            wait((int *)0);
        }
    }

    exit(0);
}