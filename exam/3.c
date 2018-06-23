#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *do_sum(void * arg)
{
	int value,sum = 0;
	void * re;
	value = (int)arg;
	for(int i = 0;i < 10;i ++)
	{
		sum += value;
		value++;
	}
	re = (void *)sum;
	return re;
	
}
int main()
{
	pthread_t *tid;
	int start,sum = 0;
	void * result;
	tid = (pthread_t *)malloc(sizeof(pthread_t)*10);
	for(int i = 0;i < 10;i ++)
	{
		start = 10 * i + 1;
		pthread_create(&tid[i],NULL,do_sum,(void *)start);
	}
	for(int i = 0;i < 10;i ++)
	{
		pthread_join(tid[i],&result);
		sum += (int)result;
	}	
	printf("%d\n",sum);
	return 0;
}

