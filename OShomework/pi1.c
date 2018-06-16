#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <math.h>

#define COM_MAX_NUM 200000

double pnum = 0 , mnum = 0;

void * compute(void *arg)
{
    int i;
    for(i = COM_MAX_NUM + 1;i <= 2 * COM_MAX_NUM;i++)
    {
        pnum += pow(-1,i+1)/(2*i - 1);
    }
}

void master()
{
    for(int i = 1;i < COM_MAX_NUM;i++)
        mnum += pow(-1,i+1)/(2*i - 1);
}

int main()
{
    pthread_t tid;
    tid = pthread_create(&tid,NULL,compute,NULL);
    master();
    pthread_join(tid,NULL);
    printf("%0.8f\n",4*(mnum + pnum));
}
