#include "kernel/types.h"
#include "user/user.h"

void primes(int fd) __attribute__((noreturn));

void primes(int fd)
{
    int prime;
    if (read(fd, &prime, sizeof(int)) == 0)
    {
        close(fd);
        exit(1);
    }
    printf("prime %d\n", prime);

    int p_right[2];
    pipe(p_right);
    int pid = fork();
    if(pid == 0)
    {
        close(fd);
        close(p_right[1]);
        primes(p_right[0]);
        exit(0);
    }
    else if (pid > 0)
    {
        close(p_right[0]);
        int num;
        while (read(fd, &num, sizeof(int)) != 0)
        {
            if(num % prime != 0)
                write(p_right[1], &num, sizeof(int));
        }
        close(p_right[1]);
        close(fd);
        wait(0);
        exit(0);
    }
    else
    {
        printf("Fork failed\n");
        exit(1);
    }
}

int main()
{
    int p[2];
    pipe(p);
    int pid = fork();

    if(pid == 0)
    {
        close(p[1]);
        primes(p[0]);
        exit(0);
    }
    else if (pid > 0)
    {
        close(p[0]);
        for(int i = 2; i <= 280; ++i)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    else
    {
        printf("Fork failed\n");
        exit(1);
    }
}