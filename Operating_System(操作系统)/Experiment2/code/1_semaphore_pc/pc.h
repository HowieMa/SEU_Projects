#include<stdlib.h>
#include "buffer.h"
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
sem_t empty;
sem_t full;
sem_t mutex;

int p_count=1;
int c_count=1;
void *producer(void *param)
{
	buffer_item rands;
	while(1)
	{
		srand(time(0));
		sleep(rand()%7+1);
		rands=p_count;
		printf("prod:	%d\n",rands);
		p_count++;
		sem_wait(&empty);
		sem_wait(&mutex);
		insert_item(rands);
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(void *param)
{
	buffer_item rands;
	while(1)
	{
		srand(time(0)+1);
		sleep(rand()%9+1);
		rands=c_count;
		c_count++;
		sem_wait(&full);
		sem_wait(&mutex);
		printf("cons:	%d\n",rands);
		remove_item(&rands);
		sem_post(&mutex);
		sem_post(&empty);
	}
}
