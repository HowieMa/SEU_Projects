#include<iostream>
#include<stdlib.h>
#include "pc.h"
#include<pthread.h>
#include<assert.h>
using namespace std;
int main()
{
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);
	pthread_t p1,p2;
	int rc;
	printf("main:begin\n");
	rc=pthread_create(&p1,NULL,producer,NULL);
	assert(rc==0);
	rc=pthread_create(&p2,NULL,consumer,NULL);
	assert(rc==0);
	rc=pthread_join(p1,NULL);
	assert(rc==0);
	rc=pthread_join(p2,NULL);
	assert(rc==0);
	printf("main:end\n");
	return 0;
	
}
