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
#include "quicksort.h"
#include "locks.h"
#include <bits/stdc++.h>

using namespace std;
extern vector<int> UnsortedArray;
extern int thread_num;
extern int offset;
mutex mtx;
map<int,int> bucket;
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
	if (thread_part == thread_num - 1) 
	{
        right += offset;
    }
    // Inserting the value in individual bucket maps
	for(int i=left;i<=right;i++)
	{	//mtx.lock();
		//tas_lock();
		//ttas_lock();
		ticket_lock();
		bucket.insert(pair<int,int>(UnsortedArray[i],i));
		ticket_unlock();
		//ttas_unlock();
		//tas_unlock();
		//mtx.unlock();
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
