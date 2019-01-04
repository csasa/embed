// pthread demo
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void* chicken_thread(void *arg);	//mid thread declare
void* eagle_thread(void *arg);//term thread declare

int main(){
	pthread_t chicken_tid, eagle_tid;	//store thread ID
	if(pthread_create(&chicken_tid,NULL,chicken_thread,NULL)){
		perror("Create mid thread error!");
		return 0;
		}

	if(pthread_create(&eagle_tid,NULL,eagle_thread,(void*)&chicken_tid)){
		perror("Create term thread error!");
		return 0;
		}
		
	if(pthread_join(chicken_tid,NULL)){
		perror("Wait mid thread error!");
		return 0;
		}
		
	if(pthread_join(eagle_tid,NULL)){
		perror("Wait term thread error!");
		return 0;
		}			
		return 0;
	}
	
void* chicken_thread(void *arg){
	int times=0;
	printf("chicken thread created! chicken is borned!\n");
	while(1){
		printf("Chicken:I'm Running (%d s) You can't catch me!\n",times);
		sleep(1);
		times++;
		}
	}
	
void* eagle_thread(void *arg){
	pthread_t *tid;
	int i;
	printf("Eagle thread created! I'm fly in the sky!\n");
	for(i=0;i<20;i++){
		sleep(1);	
		printf("Eagle:I'm searching,Don't let me see u (%d s)\n",i);
		}

	if(NULL!=arg){
		tid=(pthread_t *)arg;
		pthread_cancel(*tid);
		printf("Eagle:I've found it,Kill!Game finished!\n");
		}
	}
