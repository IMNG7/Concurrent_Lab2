#include <iostream>
#include <mutex>
#include "locks.h"

int cnt = 0;
int num_iterations=20;
int thread_num=4;
mutex mtx;
pthread_barrier_t bar;
MCS_Lock Lock_MCS;
barrier_sense bar_sen;

void* thread_main(void* args)
{	int my_tid = *((size_t*)args);
	Node New_node;
	for(int i=0;i<num_iterations+thread_num;i++)
	{
		if(i%thread_num == my_tid)
		{
			//mtx.lock();
			//tas_lock();
			//ttas_lock();
			//ticket_lock();
			Lock_MCS.lock(&New_node);
			cnt++;
			Lock_MCS.unlock(&New_node);
			//ticket_unlock();
			//ttas_unlock();
			//tas_unlock();
			//mtx.unlock();
		}
		bar_sen.wait();
	}
}

int main()
{
	pthread_t threads[thread_num];
	ssize_t* argt = new ssize_t[thread_num+1];
	int ret;
	pthread_barrier_init(&bar, NULL, thread_num);
	bar_sen.initialize_bar_values(thread_num);
	cout<<"\n\rDoing quicksort";
	int i;
	for(i=0;i<thread_num;i++)
	{
		argt[i]=i;
		//Creates threads for quick sort
		ret = pthread_create(&threads[i],NULL,&thread_main,&argt[i]);
		if(ret)
		{
			cout<<"ERROR WHILE CREATION";
			exit(-1);
		}
		cout<<"\n\rThreads "<<i<<" Created";
	}
	for(i=0;i<thread_num;i++)
	{
		ret = pthread_join(threads[i],NULL);
		if(ret)
		{
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		cout<<"\n\rThreads "<<i<<" Joined";
	}
	cout<<"\n\rConter Value:"<<cnt<<"\n\r";
	return 0;
}
