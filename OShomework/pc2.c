#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <math.h>

#define CAPACITY 4
#define ITEM_COUNT 8

int buffer1[CAPACITY],buffer2[CAPACITY];
int in,co,out;

typedef struct {
    int value;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
}sema_t;

sema_t mutex1_sema;
sema_t mutex2_sema;
sema_t empty_buffer1_sema;
sema_t full_buffer1_sema;
sema_t empty_buffer2_sema;
sema_t full_buffer2_sema;



void put_item(int item)
{
    buffer1[in] = item;
    in = (in + 1) % CAPACITY;
}
int get_item()
{
    int item = buffer2[out];
    out = (out + 1) % CAPACITY;
    return item;
}
void com_item()
{
    int item;
    item = buffer1[co];
    buffer2[co] = item - 32;
    co = (co + 1) % CAPACITY;
} 

void sema_init(sema_t *sema,int value)
{
    sema->value = value;
    pthread_cond_init(&sema->cond,NULL);
    pthread_mutex_init(&sema->mutex,NULL);
}

void sema_wait(sema_t *sema)
{

    pthread_mutex_lock(&sema->mutex);
    sema->value--;
    //printf("%d ",sema->value);
    while(sema->value < 0)
        pthread_cond_wait(&sema->cond,&sema->mutex);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

void * produce(void *arg)
{
    for(int i = 0;i < ITEM_COUNT;i ++)
    { 
        sema_wait(&empty_buffer1_sema);
        sema_wait(&mutex1_sema);
        put_item('a' + i);
        //printf("produce : %c \n",'a' + i);
        sema_signal(&mutex1_sema);
        sema_signal(&full_buffer1_sema);
    }   
    return NULL;
}
void * compute(void *arg)
{
    int i;
    for(i = 0;i < ITEM_COUNT;i ++)
    {
        sema_wait(&full_buffer1_sema);
        sema_wait(&empty_buffer2_sema);

        sema_wait(&mutex1_sema);
        sema_wait(&mutex2_sema);
        com_item();
        //puts("compute : Uppercase");
        sema_signal(&mutex1_sema);
        sema_signal(&mutex2_sema);
        sema_signal(&full_buffer2_sema);
        sema_signal(&empty_buffer1_sema);
    }
    return NULL;
}
void * consume(void * arg)
{
    int i ,item;
    for(i = 0;i < ITEM_COUNT;i ++ )
    {
        sema_wait(&full_buffer2_sema);
        sema_wait(&mutex2_sema);
        item = get_item();
        printf("%c  ",item);

        sema_signal(&mutex2_sema);
        sema_signal(&empty_buffer2_sema); 
    }
    return NULL;
} 

int main()
{
    pthread_t pid1,pid2,pid3;
    sema_init(&mutex1_sema,1);
    sema_init(&mutex2_sema,1);
    sema_init(&empty_buffer1_sema,CAPACITY );
    sema_init(&full_buffer1_sema,0);
    sema_init(&empty_buffer2_sema,CAPACITY );
    sema_init(&full_buffer2_sema,0);

    pthread_create(&pid1,NULL,produce,NULL);
    pthread_create(&pid2,NULL,compute,NULL);
    pthread_create(&pid3,NULL,consume,NULL);

    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    pthread_join(pid3,NULL);
    printf("\n");
    return 0;
}
