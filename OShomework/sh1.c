#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 1000

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
            system(cmdstr);

    }
   return 0;
}
