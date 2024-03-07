#include "kernel/types.h"
#include "user/user.h"

#define READ 0
#define WRITE 1


/*
 * @brief 读取左邻居管道数字的第一个数字
 * @param fisrt 第一个数字存入的位置
 * @return 正确返回 0，否则 -1
*/
int lpipe_first_data(int lpip[2], int *fisrt)
{
    if (read(lpip[READ], fisrt, sizeof(int)) == sizeof(int))
    {
        printf("prime %d\n", *fisrt);  // 输出第一个数字
        return 0;
    }
    return -1;
}

/*
 * @brief 将左管道剩余无法被 first 整除的数字传入右管道
 * @param lpip 左管道
 * @param rpip 右管道
 * @param first 首个数字
*/
void transmit_data(int lpip[2], int rpip[2], int first)
{
    int data;
    while (read(lpip[READ], &data, sizeof(int)) == sizeof(int))
    {
        if (data % first)
        {   // 无法整除传入右管道
            write(rpip[WRITE], &data, sizeof(int));
        }
    }

    // 数据传输完，关闭左管道的读取和右管道的写入
    close(lpip[READ]);
    close(rpip[WRITE]);
}


/*
 * @brief 寻找素数
 * @param lpip 左管道
*/
void prime(int lpip[2])
{
    close(lpip[WRITE]);   // 当前需要从左管道读取数据，所以需要关闭写入
    
    int first;
    // 首先得到左管道的首个数字
    if (lpipe_first_data(lpip, &first) == 0)
    {
        int rpip[2];
        pipe(rpip);

        transmit_data(lpip, rpip, first);  // 传输数据到右管道

        if (fork() == 0)
        {
            prime(rpip);   // 子进程递归调用
        }
        else
        {
            close(rpip[READ]);  // 数据在 transmit_data 传输完了，关闭右管道写数据
            wait(0);    // 等待子进程结束
        }
    }
    exit(0);
}


int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    // 先写入管道
    for (int i = 2; i <= 35; ++i)
    {
        write(p[WRITE], &i, sizeof(i));
    }

    if (fork() == 0)
    {
        prime(p);    // 子进程执行管道数字的转移
    }
    else
    {
        close(p[WRITE]);  // 关闭读写
        close(p[READ]);
        wait(0);
    }

    exit(0);
}