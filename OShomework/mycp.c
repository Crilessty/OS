#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc,char **argv)
{
    int fd1,fd2,num;
    char buf[1024];
    fd1 = open(argv[1],O_CREAT|O_RDWR, 0666);
    fd2 = open(argv[2],O_CREAT|O_RDWR, 0666);

    while((num = read(fd1,buf,1024)) > 0)
        write(fd2,buf,num);
    close(fd1);
    close(fd2);
    return 0;

}
