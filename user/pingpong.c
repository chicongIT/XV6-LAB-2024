#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    int pid = fork();
    char buf;

    if(pid == 0)
    {
        close(p1[1]); 
        close(p2[0]);

        read(p1[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &buf, 1);

        close(p1[0]); 
        close(p2[1]);
    }
    else if (pid > 0)
    {
        close(p1[0]); 
        close(p2[1]);

        write(p1[1], &buf, 1);
        wait(0);

        read(p2[0], &buf, 1); 
        printf("%d: received pong\n", getpid());

        close(p1[1]); 
        close(p2[0]);
    } 
    else
    {
        printf("Fork failed\n");
        exit(1);
    }
    exit(0);
}