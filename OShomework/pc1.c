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

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;

int buffer1_empty()
{
    return in == co;
}
int buffer1_full()
{
    return (in + 1) % CAPACITY == co;
}
int buffer2_empty()
{
    return co == out;
}
int buffer2_full()
{
    return (co + 1) % CAPACITY == out;
}

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

void * produce(void *arg)
{
    for(int i = 0;i < ITEM_COUNT;i ++)
    {
        pthread_mutex_lock(&mutex1);
        while(buffer1_full())
        {
            pthread_cond_wait(&wait_empty_buffer1,&mutex1);
        }
            
        put_item('a' + i);
        //printf("produce : %c\n",'a'+i);

        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }   
    return NULL;
}
void * compute(void *arg)
{
    int i;
    for(i = 0;i < ITEM_COUNT;i ++)
    {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);
        while(buffer1_empty())
        {
            pthread_cond_wait(&wait_full_buffer1,&mutex1);
            //puts("xx");
        }
            
        while(buffer2_full())
            pthread_cond_wait(&wait_empty_buffer2,&mutex2);
        com_item();
        //puts("compute : Uppercase");
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}
void * consume(void * arg)
{
    int i ,item;
    for(i = 0;i < ITEM_COUNT;i ++ )
    {
        pthread_mutex_lock(&mutex2);
        while(buffer2_empty())
            pthread_cond_wait(&wait_full_buffer2,&mutex2);
        item = get_item();
        printf("%c  ",item);

        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
} 

int main()
{
    pthread_t pid1,pid2,pid3;
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    pthread_cond_init(&wait_empty_buffer1,NULL);
    pthread_cond_init(&wait_full_buffer1,NULL);
    pthread_cond_init(&wait_empty_buffer2,NULL);
    pthread_cond_init(&wait_full_buffer2,NULL);

    pthread_create(&pid1,NULL,produce,NULL);
    pthread_create(&pid2,NULL,compute,NULL);
    pthread_create(&pid3,NULL,consume,NULL);

    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    pthread_join(pid3,NULL);
    printf("\n");
    return 0;
}
