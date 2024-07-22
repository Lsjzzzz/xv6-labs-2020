#include "kernel/types.h"
#include "user/user.h"

int primes(int *oldp)
{
    close(oldp[WR]);
    int prime, num;
    if (read(oldp[RD], &prime, sizeof(int)) != 0)
    {
        fprintf(1, "prime %d\n", prime);

        int newp[2];
        pipe(newp);

        if (fork() == 0)
        {
            primes(newp);
        }
        else
        {
            close(newp[RD]);

            while (read(oldp[RD], &num, sizeof(int)) != 0)
                if(num % prime != 0)
                    write(newp[WR], &num, sizeof(int));

            close(oldp[RD]);
            close(newp[WR]);

            wait((int *)0);
        }
    }
    else
    {
        close(oldp[RD]);
    }
    exit(0);

}
int main(int argc, char *argv)
{
    // 调用错误
    if (argc != 1)
    {
        fprintf(2, "Usage primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);
    
    if (fork() == 0)
        primes(p);
    else
    {
        close(p[RD]);
        
        for (int i = 2; i <= 35; i++) write(p[WR], &i, sizeof(int));

        close(p[WR]);

        wait((int *)0);
    }

    exit(0);
}