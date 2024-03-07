#include "kernel/types.h"
#include "user/user.h"

#define READ 0       // 管道读入端
#define WRITE 1        // 管道写入端

int main(int argc, char* argv)
{
    char buf = ' ';

    // p1 用于父进程--->子进程，
    // p2 用于子进程--->父进程
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    int pid = fork();
    
    if(pid < 0)
    {
        fprintf(2, "fork() error!\n");
        close(p1[READ]);
        close(p1[WRITE]);
        close(p2[READ]);
        close(p2[WRITE]);
        exit(1);
    }
    else if (pid == 0)
    {
        close(p1[WRITE]);
        close(p2[READ]);
        if (read(p1[READ], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "child read() error!\n");
            exit(1);
        }
        else
        {
            fprintf(1, "%d: received ping\n", getpid());
        }

        if (write(p2[WRITE], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "child write() error!\n");
            exit(1);
        }

    }
    else
    {
        close(p1[READ]);
        close(p2[WRITE]);

        if (write(p1[WRITE], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent write() error!\n");
            exit(1);
        }

        if (read(p2[READ], &buf, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent read() error!\n");
            exit(1);
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }

        close(p1[WRITE]);
        close(p2[READ]);
    }

    exit(0);
}