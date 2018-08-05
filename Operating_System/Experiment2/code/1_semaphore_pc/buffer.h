typedef int buffer_item;
#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
int count=0;
int insert_item(buffer_item item){
	
	//找到第一个为空的位置
	int i=0;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		if(buffer[i]==0)
		{
			buffer[i]=item;
			count++;
			if(count==5)
				printf("prod:	FULL\n");
			return 0;
		}
	}
	
	return -1;
	
}

int remove_item(buffer_item *item){
	//找到rand所在位置
	int i=0;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		if(buffer[i]==*item)
		{
			buffer[i]=0;
			count--;
			if(count==0)
				printf("cons:	EMPTY\n");
			return 0;
		}
	}
	
	return -1;
}


