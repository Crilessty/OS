#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LEN 1000
int start = 0;
int str_operation(const char *str,char **buf)
{
    int count,num,i;
    i = count = num = 0;

    while(*str == ' ')
        str++;

    while(str[i] != '\n')
    {
        if(str[i] == ' ')
        {
            i++;
            continue;
        }
        for(;str[i] != ' ' && str[i] != '\n';i++)
            count++;
        buf[num] = (char*)malloc(sizeof(char)*20);
        strncpy(buf[num++],str+i-count,count);
        count = 0;
        if(str[i] == '\n')
            break;
    }
    buf[num] = NULL;
    return num;
}
void do_command(char **buf,int n)
{
    int i,fd_in,fd_out;
    int fildes[2],fd[2];
    int pid;
    
    for(i = 0;i < n  - 2;i ++)
    {
        if(buf[i][0] == '<')
        {
            if(strlen(buf[i]) == 1)
            {
                fd_in = open(buf[i + 1],O_CREAT|O_RDWR, 0777); 
                for(i;i < n - 1;i ++)
                    buf[i] = buf[i + 2];
            }
            else    
            {
                fd_in = open(buf[i] + 1,O_CREAT|O_RDWR, 0777);
                for(i;i < n ;i ++)
                    buf[i] = buf[i + 1];
            }
            dup2(fd_in,0);
            break;
        }
    }
    
    //TODO PIPE
    
    for(i = 0;i < n && buf[i] != NULL;i ++)
    {
        if(strcmp(buf[i],"|") == 0)
        {   
            pipe(fildes);
            pid = fork();
            if(pid < 0)
                puts("err");
            if(pid == 0)
            {
                //printf("start :%d,i : %d\n",start,i);
                //dup2(fildes[0],0);
                dup2(fildes[1],1);
                close(fildes[0]);
                close(fildes[1]);
                buf[i] = NULL;
                
                execvp(buf[start],buf + start);
            }
            wait(NULL);
            start =  i + 1;
            //printf("%d\n",start);
            dup2(fildes[0],0);
            close(fildes[0]);
            close(fildes[1]);
        }
    }
    //wait(NULL);
    for(i = 0;i < n;i ++)
    {
        if(buf[i][0] == '>')
        {
            if(strlen(buf[i]) == 1)
            {
                fd_out = open(buf[i + 1],O_CREAT|O_RDWR, 0777); 
                for(i;i < n - 1;i ++)
                    buf[i] = buf[i + 2];
            }
            else    
            {
                fd_out = open(buf[i] + 1,O_CREAT|O_RDWR, 0777);
                for(i;i < n ;i ++)
                    buf[i] = buf[i + 1];
            }
            dup2(fd_out,1);
            break;
        }
    }
    
    execvp(buf[start],buf + start);
    
    
    //dup2(1,fd_out);
    close(fd_out);
    
}
int mysys(char * str)
{
    int i,flag,newfd;
    char **buf = (char**)malloc(sizeof(char *)*20);
    int n = str_operation(str,buf);
    pid_t pid;
    int status;
    if(str == NULL)
        return 1;
    pid = fork();
    if(pid < 0)
        status = -1;
    else if(pid == 0)
    {
        do_command(buf,n);
        exit(123);
    }
    else
    {
        wait(&status);
    }
    return status;
}



//内置指令返回0，外置命令返回1
int bulidin_command(char *str)
{
    char **buf = (char**)malloc(sizeof(char *)*20);
    int n = str_operation(str,buf);
    if(!buf[0])
        return 0;
    if(!strcmp(buf[0],"exit"))
    {
        exit(0);
    }
    if(!strcmp(buf[0],"cd"))
    {
        if(chdir(buf[1]) == -1)
            printf("-myshell: cd: %s: No such file or directory\n",buf[1]);
        return 0;
    }
    if(!strcmp(buf[0],"pwd"))
    {
        char buf[100];
        printf("%s\n",getcwd(buf,100));
        return 0;
    }
    return 1;
}

int main(int argc,char **argv)
{
    int fd,n ;
    char cmdstr[MAX_LEN];


    while(1)
    {
        printf("shell>");
        fflush(stdout);
        memset(cmdstr,0,MAX_LEN * sizeof(char));
        fd = read(0,cmdstr,MAX_LEN);

        if(n = bulidin_command(cmdstr))
            mysys(cmdstr);

    }
   return 0;
}

