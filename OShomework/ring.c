#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <math.h>

#define THREAD_NUM 24

int count = 0;
void *send(void *arg)
{
    int n = (int)arg;
    printf(" T%-3d ->  T%-3d:  %d\n",count + 1,(count + 1)%24+1,n);
    n++;
    return (void *)n;
}

int main()
{
    int a;
    void *N;
    pthread_t *tid;
    tid = (pthread_t *)malloc(sizeof(pthread_t) * THREAD_NUM);
    a = 1;
    pthread_create(&tid[0],NULL,send,(void *)a);
    for(int i = 1; i < THREAD_NUM;i ++)
    {
        pthread_join(tid[i - 1],&N);
        pthread_create(&tid[i],NULL,send,N);
        count++;
    }
    pthread_join(tid[23],NULL);
    return 0;
}
