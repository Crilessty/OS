#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int mysys(const char * str)
{
    pid_t pid;
    int status;
    if(str == NULL)
        return 1;
    pid = fork();
    if(pid < 0)
        status = -1;
    else if(pid == 0)
    {
        execl("/bin/sh","sh","-c",str,NULL);
        exit(123);
    }
    else
    {
        wait(&status);
    }
    return status;
}

int main()
{ 
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
    return 0;
}
