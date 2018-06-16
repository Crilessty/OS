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

struct sort_info{
    int buf[100];
    int num;
};

void swap(int *a,int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void select_sort(int buf[],int num)
{
    int i,j,min;
    for(i = 0;i < num - 1;i ++)
    {
        min = i;
        for(j = i + 1;j < num;j ++)
            if(buf[j] < buf[min])
                min = j;
        if(min != i)
        {
            swap(buf + i,buf + min);
        }
    }
}
void *sort(void *arg)
{
    struct sort_info *info  = (struct sort_info *)arg; 
    select_sort(info->buf,info->num);
}

void merge_sort(int arr1[], int len1, int arr2[], int len2, int result[])
{
    int dst = 0;
    int ind1 = 0; 
    int ind2 = 0;
    while(ind1 < len1 && ind2 < len2)
    {
        if(arr1[ind1] < arr2[ind2])
        {
            result[dst++] = arr1[ind1++];
        }
        else
        {
            result[dst++] = arr2[ind2++];
        }
    }
    while(ind1 < len1)
    {
        result[dst++] = arr1[ind1++];
    }
    while(ind2 < len2)
    {
        result[dst++] = arr2[ind2++];
    }
}

int main(int argc,char **argv)
{
    int num,i;
    struct sort_info info;
    pthread_t pid;
    puts("input the number of array :");
    scanf("%d",&num);
    int *buf1 = (int *)malloc(sizeof(int) * (num/2 + 1));
    int * buf2 = (int *)malloc(sizeof(int) * num);
    puts("input the array :");
    for(i = 0;i < num/2;i ++)
        scanf("%d",&info.buf[i]);
    for(i = 0; i < num - num/2;i ++)
        scanf("%d",&buf1[i]);
    
    info.num = num/2;
    pthread_create(&pid,NULL,sort,(void *)&info);
    select_sort(buf1,num - num/2);
    pthread_join(pid,NULL); 
    merge_sort(info.buf,num/2,buf1,num - num/2,buf2);

    puts("after sort :");
    for(i = 0;i < num ;i ++)
        printf("%d ",buf2[i]);
    printf("\n");
}
