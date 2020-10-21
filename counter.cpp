/*
	FileName: counter.cpp
	Description: Contains the function definitions and the main function required to perform counter micro benchmarking.
	Author: Nitik Gupta
	References Used: For function Pointers: 
					https://www.cprogramming.com/tutorial/function-pointers.html
*/
#include <iostream>
#include <vector>
#include <mutex>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "locks.h"
#include "util.h"

int cnt = 0; // Counter for benchmark
int num_iterations=20;	// Number of iterations the benchmarks would run
int thread_num=4; // Number of threads
mutex mtx;
pthread_barrier_t bar;

MCS_Lock Lock_MCS; // Object for MCS_Lock class

barrier_sense bar_sen; // Object for Barrier sense class

lock_type lock_name; // String for storing the lock name given

string bar_name; // String for storing the barrier name given

bool Lock_Used = false,Bar_Used = false; // Used to get what is given in arguments
struct timespec start, end_time;
// Function pointer for lock functions
void (*lock_array[])() = {tas_lock,ttas_lock,ticket_lock,pthread_lock};
// Function pointer for unlock functions
void (*unlock_array[])() = {tas_unlock,ttas_unlock,ticket_unlock,pthread_unlock};

/*
	Function Name: thread_main
	Description: Initial recursive function for the micro benchmark
	Inputs: args - thread number
	Returns: Nothing.
*/
void* thread_main(void* args)
{	int thread_part = *((size_t*)args);
	pthread_barrier_wait(&bar);
	if(thread_part==0)
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	pthread_barrier_wait(&bar);
	// Object for a node to be given for MCS Lock
	Node New_node;
	for(int i=0;i<num_iterations+thread_num;i++)
	{
		if(i%thread_num == thread_part)
		{
// If lock is defined, then look for which lock to implement
if(Lock_Used)
{
			if(lock_name < mcs)
				(*lock_array[lock_name])();
			else if(lock_name == mcs)
				Lock_MCS.lock(&New_node);
}
			cnt++;	//Increment the counter
if(Lock_Used)
{
			if(lock_name < mcs)
				(*unlock_array[lock_name])();
			else if(lock_name == mcs)
				Lock_MCS.unlock(&New_node);
}
		}
	}
// If barrier is defined, then look for which barrier to implement
if(Bar_Used)
{
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
}
else if(Lock_Used)
// Barrier for the timing
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
		c= getopt_long(argc,args,"no:t:li:b:",long_options,&count);			
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
						Lock_Used = true;
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
						Bar_Used = true;
						cout<<"\n\r Bar given";
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
if(Lock_Used)
cout<<"\n\r Lock Initialized \n\r";
if(Bar_Used)
cout<<"\n\r Bar Initialized \n\r"; 
if(Lock_Used && Bar_Used)
{
// If both lock and barrier are given, exit the program.
cout<<"\n\r Cant do Lock and Bar Together \n\r Exiting";
return 0; 
}
if(!Lock_Used && !Bar_Used)
{
// If nothing is given, use barrier by default.
	cout<<"\n\r Not Given both Lock and Bar \n\r Using Pthread Lock by default";
	lock_name = pthread;
}
//Pthread barrier is used by default for the timing, so will initialize everytime
	pthread_barrier_init(&bar, NULL, thread_num);
	if(bar_name == "sense")
		bar_sen.initialize_bar_values(thread_num);
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
	std::vector<int> vector_cnt;
	vector_cnt.push_back(cnt);
	// If output file given, then write to output file
	if(!(output_file.empty())) add_to_file(vector_cnt,output_file);
	// If output file is not given,then write to terminal
	else cout<<"\n\rConter Value:"<<cnt<<"\n\r";
	unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	return 0;
}
/*
	Function Name: pthread_lock
	Description: Uses Pthread lock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void pthread_lock()
{
	mtx.lock();
}
/*
	Function Name: pthread_unlock
	Description: Uses Pthread unlock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void pthread_unlock()
{
	mtx.unlock();
}