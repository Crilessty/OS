#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <math.h>

#define COM_MAX_NUM 200000
#define THREAD_NUM 24

double sum = 0 ;

void * compute(void *arg)
{
    int n = (int)arg;
    for(int i = 0 ;i< COM_MAX_NUM;i++)
    {
        sum += pow(-1,n+1)/(2*n - 1);
        n += THREAD_NUM;
    }
}



int main()
{
    pthread_t *pid;
    pid = (pthread_t *)malloc(sizeof(pthread_t) * THREAD_NUM);

    for(int i = 0;i < THREAD_NUM;i ++)
    {
        pthread_create(&pid[i],NULL,compute,(void *)(i+1));
    }
    for(int i = 0;i < THREAD_NUM;i ++)
        pthread_join(pid[i],NULL);

    printf("%.8f\n",sum*4);
}
