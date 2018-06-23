#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc,char *argv[])
{
    int fd,num;
    char buf[1024];
    fd = open(argv[1],O_RDONLY);
	if(fd < 0)
		puts("can 't open the file !");
    while((num = read(fd,buf,1023)) > 0)
        write(1,buf,num);
	close(fd);
    return 0;

}
