#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
	int pid1,pid2;
	int fd1[2],fd2[2];
	int value1,value2,sum = 0;
	pipe(fd1);
	pipe(fd2);
	pid1 = fork();
	pid2 = fork();
	if(pid1 == 0)
	{
		for(int i = 1;i <= 50;i ++)
		{	
			sum += i;
		}
		dup2(fd1[1],1);
		close(fd1[0]);
		close(fd1[1]);
		write(1,&sum,sizeof(int));
	}
	if(pid2 == 0)
	{
		for(int i = 51;i <= 100;i ++)
		{
			sum += i;
		}
		dup2(fd2[1],1);
		write(1,&sum,sizeof(int));
	}
	read(fd1[0],&value1,sizeof(int));
	read(fd2[0],&value2,sizeof(int));
	printf("%d\n",value1+value2);
	return 0;	
}

