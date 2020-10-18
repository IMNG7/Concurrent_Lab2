#include <iostream>
#include <mutex>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "locks.h"

int cnt = 0;
int num_iterations=20;
int thread_num=4;
mutex mtx;
pthread_barrier_t bar;
MCS_Lock Lock_MCS;
barrier_sense bar_sen;
lock_type lock_name;
string bar_name;
struct timespec start, end_time;
void (*lock_array[])() = {tas_lock,ttas_lock,ticket_lock,pthread_lock};
void (*unlock_array[])() = {tas_unlock,ttas_unlock,ticket_unlock,pthread_unlock};

void* thread_main(void* args)
{	int thread_part = *((size_t*)args);
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
	if(thread_part==0)
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
	
	Node New_node;
	// New_node.next = NULL;
	// New_node.wait = false;
	for(int i=0;i<num_iterations+thread_num;i++)
	{
		if(i%thread_num == thread_part)
		{
			if(lock_name < mcs)
				(*lock_array[lock_name])();
			else if(lock_name == mcs)
				Lock_MCS.lock(&New_node);
			cnt++;
			if(lock_name < mcs)
				(*unlock_array[lock_name])();
			else if(lock_name == mcs)
				Lock_MCS.unlock(&New_node);
		}
	}
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
	if(thread_part==0)
	{
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
}

int main(int argc, char *args[])
{
	int c;
	string output_file,lock;
	while(1)
	{
		int count = 0;
		//Structure used in getopt_long to capture the command line arguments.
		static struct option long_options[] = {						
			{"name",	no_argument,		0,	'n'},
			{"output",	required_argument,	0,	'o'},
			{"lock", 	required_argument,	0,	'l'},
			{"threads", required_argument,	0,	't'},
			{"iterations", required_argument,	0,	'i'},
			{"bar", required_argument,	0,	'b'},
			{0,			0,					0,	0}
		};
		// Used to capture the command line arguments
		c= getopt_long(argc,args,"not:li:b:",long_options,&count);			
		if(c==-1)	break;
		switch(c)
		{
			case 'n' :	
						// Full name of Developer
						cout<<"\n\r Name:NITIK SATISH GUPTA:";
						return 0;	
						break;
			case 'o' : 	
						//Saves the output file name in the variable
						output_file = optarg;						
						break;
			case 'l' :	
						// Saves the algorithm name in the variable
						lock = optarg;
						if(lock == "tas")
						lock_name = tas;
						else if(lock == "ttas")
						lock_name = ttas;
						else if(lock == "ticket")
						lock_name = ticket;
						else if(lock == "pthread")
						lock_name = pthread;
						else if(lock == "mcs")
						lock_name = mcs;							
						break;
			case 'b' :	
						// Saves the algorithm name in the variable
						bar_name = optarg;							
						break;
			case 't' :	// Saves number of threads to be used
						thread_num = atoi(optarg);
						break;
			case 'i' :	// Saves number of threads to be used
						num_iterations = atoi(optarg);
						break;

			default	:	
						break;
		}
	}
	pthread_t threads[thread_num];
	ssize_t* argt = new ssize_t[thread_num+1];
	int ret;
	if(bar_name == "sense")
		bar_sen.initialize_bar_values(thread_num);
	else if(bar_name == "pthread")
		pthread_barrier_init(&bar, NULL, thread_num);
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
	unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	return 0;
}
void pthread_lock()
{
	mtx.lock();
}
void pthread_unlock()
{
	mtx.unlock();
}