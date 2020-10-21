/*
	FileName: bucket.cpp
	Description:Contains the function definitions required to perform bucketsort operation on the given numbers.
	Author: Nitik Gupta
	References Used: For Bucket Sort Algorithm:
					 https://www.geeksforgeeks.org/bucket-sort-2/
*/
#include <iostream>
#include <vector>
#include <mutex>
#include <pthread.h>
#include "bucketsort.h"
#include "locks.h"
#include <bits/stdc++.h>

using namespace std;
extern vector<int> UnsortedArray;
extern int thread_num;
extern int offset;
mutex mtx;
map<int,int> bucket;
extern struct timespec start, end_time;
MCS_Lock Lock_MCS;	// Object for MCS_Lock class
pthread_barrier_t bar; // Object for Pthread Barrier
barrier_sense bar_sen; // Object for Barrier sense class
extern lock_type lock_name;	// Gets lock name for the Lock to be used
extern string bar_name;	// Gets barrier name for the Barrier to be used
// Function pointer for lock functions
static void (*lock_array[])() = {tas_lock,ttas_lock,ticket_lock,pthread_lock};
// Function pointer for unlock functions
static void (*unlock_array[])() = {tas_unlock,ttas_unlock,ticket_unlock,pthread_unlock};

/*
	Function Name: bucketsort_thread
	Description: Initial recursive function to split the vector for sorting for single thread
	Inputs: args - thread number
	Returns: Nothing.
*/
void* bucketsort_thread(void* args)
{

	// Taking the thread number
	size_t thread_part = *((size_t*)args);
	
	int size = UnsortedArray.size();
	// Calculating the value of left to send for sorting
	int left =thread_part * (size/thread_num);
	// Calculating the value of right to send for sorting
	int right=((thread_part+1) * (size/thread_num)) -1;
	// In the last thread, checking if there are number left and adding
	// to right
	Node New_node;
	if (thread_part == thread_num - 1) 
	{
        right += offset;
    }
    // Depending on name given, use the barrier
    if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
	if(thread_part==0)
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
	// Depending on name given, use the barrier
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
    // Inserting the value in individual bucket maps
	for(int i=left;i<=right;i++)
	{	
		// Depending on name given, use the lock
		if(lock_name < mcs)
			(*lock_array[lock_name])();
		else if(lock_name == mcs)
			Lock_MCS.lock(&New_node);
		//Insert the value inside the ordered map
		bucket.insert(pair<int,int>(UnsortedArray[i],i));
		// Depending on name given, use the lock
		if(lock_name < mcs)
			(*unlock_array[lock_name])();
		else if(lock_name == mcs)
			Lock_MCS.unlock(&New_node);
		
	}
	// cout<<"wait3";
	if(bar_name == "sense")
		bar_sen.wait();
	else if(bar_name == "pthread")
		pthread_barrier_wait(&bar);
	if(thread_part==0)
	{
		clock_gettime(CLOCK_MONOTONIC,&end_time);
	}
}
/*
	Function Name: bucketsort
	Description: Sorting function to initialize and start the bucket sort with threading
	Inputs: threads- thread array
	Returns: Nothing.
*/
void bucketsort(pthread_t *threads)
{
	ssize_t* argt = new ssize_t[thread_num+1];
	int ret;
	int size = UnsortedArray.size();
	if(bar_name == "sense")
		bar_sen.initialize_bar_values(thread_num);	//Initializes the Sense Reversal barrier
	else if(bar_name == "pthread")
		BAR3_init();	//Initializes the Pthread barrier
	for(int i=0;i<thread_num;i++)
	{	
		argt[i]=i;
		ret = pthread_create(&threads[i],NULL,&bucketsort_thread,&argt[i]);
		if(ret)
		{
			cout<<"ERROR WHILE CREATION";
			exit(-1);
		}
		cout<<"\n\rThreads "<<i<<" Created";
	}
	for(int i=0;i<thread_num;i++)
	{
		ret = pthread_join(threads[i],NULL);
		if(ret)
		{
			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);
		}
		cout<<"\n\rThreads "<<i<<" Joined";
	}
	int index = 0;
	// Storing the value in the array from the sorted buckets
	for(auto i=bucket.begin();i!=bucket.end();i++)
	{	
		UnsortedArray[index++] = i->first; 
	}
	delete argt;
}
/*
	Function Name: BAR3_init
	Description: Initializes Barrier
	Inputs: None
	Returns: Nothing.
*/
void BAR3_init()
{
	pthread_barrier_init(&bar, NULL, thread_num);
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