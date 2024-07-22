#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv)
{
    // 调用错误
    if (argc != 1)
    {
        fprintf(2, "Usage pingpong\n");
        exit(1);
    }

    // 两个管道
    int p_FaterToSon[2];
    int p_SonToFater[2];

    pipe(p_FaterToSon);
    pipe(p_SonToFater);

    char *c = "";
    if (fork() == 0) // 子进程
    {
        int pid = getpid();
        close(p_FaterToSon[WR]); // 父到子的   写入关闭 , 只打开读取
        close(p_SonToFater[RD]); // 子到父的   读取关闭 , 只打开写入

        read(p_FaterToSon[RD], c, 1); // 等到 father 关闭写入断 才停止
        close(p_FaterToSon[RD]);

        //2  子进程收到一个字节后 先打印“`<pid>: received ping`”
        fprintf(1, "%d: received ping\n", pid);

        //3  发送给父进程一个字节
        write(p_SonToFater[WR], c, 1);
        close(p_SonToFater[WR]);
    }
    else
    {
        int pid = getpid();
        //1  父进程应该先向子进程发送一个字节
        close(p_FaterToSon[RD]);
        close(p_SonToFater[WR]);

        write(p_FaterToSon[WR], c, 1);
        close(p_FaterToSon[WR]);

        //4 读取子进程发送的一个字节 然后打印
        read(p_SonToFater[RD], c, 1);
        close(p_SonToFater[RD]);
        
        fprintf(1, "%d: received pong\n", pid);
    }

    exit(0);
}