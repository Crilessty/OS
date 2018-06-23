#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_cond_t wait_T1;
pthread_cond_t wait_T2;
pthread_cond_t wait_T3;
pthread_mutex_t mutex;

int t1 = 0,t2 = 0,t3 = 0;

void *T1_entry(void *arg)
{
	pthread_mutex_lock(&mutex);
	sleep(2);
	puts("T1");
	t1 = 1;
	pthread_cond_broadcast(&wait_T1);
	pthread_mutex_unlock(&mutex);
}
void *T2_entry(void *arg)
{
	pthread_mutex_lock(&mutex);
	while(!t1)
		pthread_cond_wait(&wait_T1,&mutex);
	pthread_mutex_unlock(&mutex);
    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts("T2");
	t2 = 1;
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&wait_T2);
	pthread_mutex_unlock(&mutex);
}

void *T3_entry(void *arg)
{
	pthread_mutex_lock(&mutex);
	while(!t1)
		pthread_cond_wait(&wait_T1,&mutex);
	pthread_mutex_unlock(&mutex);
    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts("T3");
	t3 = 1;
	 pthread_mutex_lock(&mutex);
	pthread_cond_signal(&wait_T3);
	pthread_mutex_unlock(&mutex);
}

void *T4_entry(void *arg)
{
	pthread_mutex_lock(&mutex);
	while(!t2 || !t3)
		pthread_cond_wait(&wait_T2,&mutex);
	pthread_mutex_unlock(&mutex);
    puts("T4");
}


int main()
{
	pthread_t T[4];
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&wait_T1,NULL);
	pthread_cond_init(&wait_T2,NULL);
	pthread_cond_init(&wait_T3,NULL);
	pthread_create(&T[0],NULL,T1_entry,NULL);
	pthread_create(&T[1],NULL,T2_entry,NULL);
	pthread_create(&T[2],NULL,T3_entry,NULL);
	pthread_create(&T[3],NULL,T4_entry,NULL);
	for(int i = 0;i < 4;i ++)
		pthread_join(T[i],NULL);
	return 0;
}






