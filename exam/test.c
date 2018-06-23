#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
        int pid;
        int fd[2];
        char buf[1024];
        pipe(fd);
        pid = fork();
        if(pid == 0)
        {
                dup2(fd[1],1);
                close(fd[0]);
                close(fd[1]);
                execlp("echo","echo","hello world",NULL);
                exit(0);
        }
        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        read(0,buf,sizeof(buf));
        printf("%s",buf);
        return 0;
}

