#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> 
#include <string.h> 

/**
**Author: Mengling Ding
**Date:10/11/2020
**This program is to solve the producer and consumer problem by semaphore.
** 
*/ 

sem_t empty;
sem_t mutex;
sem_t full;
sem_t empty1;
sem_t mutex1;
sem_t full1;


int milk_buffer[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int cheese_buffer[4] = {0, 0, 0, 0};
int write_idx = 0;
int read_idx = 0;
int write1_idx = 0;
int read1_idx = 0;
const int N = 9;
const int M = 4;

//milk thread 
void *thread_milk(void* arg){
	int i;
	int *temp = (int*)arg;
	for(i = 1; i<= temp[1]; i++){
		sem_wait(&empty);
		sem_wait(&mutex);
		//critical section
		milk_buffer[write_idx] = temp[0];
		write_idx = write_idx + 1;
		if (write_idx == N){
			write_idx = write_idx % N;
		}
		//printf ("create milk:%d\n",temp[0]);
		sem_post(&mutex);
		sem_post(&full);
	}	
}

//cheese thread
void *thread_cheese(void* arg){
	int i;
	int temp1, temp2, temp3;
	int *temp = (int*)arg; 

	for (i = 1; i <= temp[1]; i++){
	//cheese is the consumer role with milk
    	sem_wait(&full);
    	sem_wait(&full);
    	sem_wait(&full);
     	sem_wait(&mutex);
     	//critical section
     	//first ID of milk
     	temp1 = milk_buffer[read_idx];
     	milk_buffer[read_idx] = 0;
	    read_idx = read_idx + 1;
	    if(read_idx == N){
	      read_idx = read_idx % N;
	     }
	    //second ID of milk 
	    temp2 = milk_buffer[read_idx];
	    milk_buffer[read_idx] = 0;
	    read_idx = read_idx + 1;
	    if(read_idx == N){
	      read_idx = read_idx % N;
	    }
	    //third ID of milk
	    temp3 = milk_buffer[read_idx];
	    milk_buffer[read_idx] = 0;
	    read_idx = read_idx + 1;
	    if(read_idx == N){
	      read_idx = read_idx % N;
	    }
	    //print out cheese ID by milk ID
	    //printf ("create cheese:%d\n",(temp1*1000+temp2*100+temp3*10+temp[0]));
	    sem_post(&mutex);
	    sem_post(&empty);
	    sem_post(&empty);
	    sem_post(&empty);
	   
	//cheese is the producer role with burger
	   sem_wait(&empty1);
	   sem_wait(&mutex1);
	   //critical section
	   //write in cheese buffer ID
	   cheese_buffer[write1_idx] = temp1*1000+temp2*100+temp3*10+temp[0];
       write1_idx = write1_idx + 1;
	   if (write1_idx == M){
		write1_idx = write1_idx % M;
	   } 
	   sem_post(&mutex1);
       sem_post(&full1);
	}
}

//burger thread
void *thread_burger(void* arg){
    int temp1, temp2;
    int i;
    int temp = *(int*)arg;
	for(i = 1; i <= temp; i++ ){
	sem_wait(&full1);
	sem_wait(&full1);
    sem_wait(&mutex1);
    //critical section
    //first 4 digit number of burger ID
    temp1 = cheese_buffer[read1_idx];
    cheese_buffer[read1_idx] = 0;
	read1_idx = read1_idx + 1;
	if(read1_idx == M){
	   read1_idx = read1_idx % M;
	   }
	//second 4 digit number of burger ID
	temp2 = cheese_buffer[read1_idx];
	cheese_buffer[read1_idx] = 0;
	read1_idx = read1_idx + 1;
	if(read1_idx == M){
	   read1_idx = read1_idx % M;
	   }
	//print out the ID of burger
	printf ("create burger:%d\n",(temp1*10000+temp2));
	sem_post(&mutex1);
	sem_post(&empty1);
	sem_post(&empty1);
	}
}

int main(){
    int num_burger;
	printf("How many burgers do you want?"); 
	scanf("%d", &num_burger);
	while(num_burger<=0){
	    while (getchar() != '\n');
	    printf("The input number should be greater than 0, please enter again.");
        num_burger = scanf("%d",&num_burger);
	}
	int num_cheese = num_burger * 2;
	int num_milk = num_burger * 6;
	
	sem_init(&mutex, 0, 1); //only one thread executes
	sem_init(&full, 0, 0); //no items for cunsumer to consume
	sem_init(&empty, 0, N); //All buffers is empty for producers to produce
	sem_init(&mutex1, 0, 1); //only one thread executes
	sem_init(&full1, 0, 0); //no items for cunsumer to consume
	sem_init(&empty1, 0, M); //All buffers is empty for producers to produce
	
	int milk1[2] = {1, num_cheese};
	int milk2[2] = {2, num_cheese};
	int milk3[2] = {3, num_cheese};
	int cheese1[2] = {4, num_burger};
	int cheese2[2] = {5, num_burger};
	
	//declaration of threads
	pthread_t t_milk1, t_milk2, t_milk3;
	pthread_t t_cheese1, t_cheese2;
	pthread_t t_burger;
	
	//create threads
	pthread_create(&t_milk1, NULL, thread_milk, (void*)milk1);
	pthread_create(&t_milk2, NULL, thread_milk, (void*)milk2);
	pthread_create(&t_milk3, NULL, thread_milk, (void*)milk3);
	pthread_create(&t_cheese1, NULL, thread_cheese, (void*)cheese1);
	pthread_create(&t_cheese2, NULL, thread_cheese, (void*)cheese2);
	pthread_create(&t_burger, NULL, thread_burger, (void*)&num_burger);
	
	sleep(1);
	
	//execute threads
	pthread_join(t_milk1, NULL);
	pthread_join(t_milk2, NULL);
	pthread_join(t_milk3, NULL);
	pthread_join(t_cheese1, NULL);
	pthread_join(t_cheese2, NULL);
	pthread_join(t_burger, NULL);
	
	//destroy the semaphore
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&mutex1);
	sem_destroy(&full1);
	sem_destroy(&empty1);

	return 0;
} 

