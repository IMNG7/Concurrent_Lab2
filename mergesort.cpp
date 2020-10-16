/*
	FileName: mergesort.cpp
	Description:Contains the function definitions required to perform mergesort operation on the given numbers.
	Author: Nitik Gupta
	References Used: For Merge Sort Algorithm:
					 https://www.geeksforgeeks.org/merge-sort/ 
*/
#include "mergesort.h"
#include "pthread.h"


extern vector<int> UnsortedArray;
extern int thread_num;
extern int offset;
pthread_barrier_t bar1;
extern struct timespec start, end_time;

/*
	Function Name: mergesort
	Description: Initial recursive function to split the vector for sorting
	Inputs: nums- vector to be printed.
			left - Left index of vector to be split
			Right - Right index of the smal vector to be split.
	Returns: Nothing.
*/
void mergesort(vector<int> &nums,int left,int right)
{
	
		// Calculates the middle value of the array given
		int middle = left+((right-left)/2);
		if(left<right)
		{
		//Splits the array into two parts and further given to split
		//until there is only one element left in each.
		mergesort(nums,left,middle);
		mergesort(nums,middle+1,right);
		// After splitting each, the are given to merge back after 
		// sorting
		merge(nums,left,middle,right);
	}
}
/*
	Function Name: mergesort_thread
	Description: Initial recursive function to split the vector for sorting for single thread
	Inputs: args - thread number
	Returns: Nothing.
*/
void* mergesort_thread(void* args)
{
		size_t thread_part = *((size_t*)args);
		int size = UnsortedArray.size()-1;
		int left =thread_part * (size/thread_num);
		// cout<<"\n\r"<<left<<"\n\r";
		int right=((thread_part+1) * (size/thread_num)) -1;
		// cout<<"\n\r"<<right<<"\n\r";
		if (thread_part == thread_num - 1) {
        right += offset;
    	}
		// Calculates the middle value of the array given
		int middle = left+((right-left)/2);
		pthread_barrier_wait(&bar1);
		if(thread_part==0)
		{
			clock_gettime(CLOCK_MONOTONIC,&start);
		}
		pthread_barrier_wait(&bar1);
		if(left<right)
		{
		//Splits the array into two parts and further given to split
		//until there is only one element left in each.
		mergesort(UnsortedArray,left,right);
		mergesort(UnsortedArray,left+1,right);
		// After splitting each, the are given to merge back after 
		// sorting
		// pthread_barrier_wait(&bar);
		merge(UnsortedArray,left,middle,right);
		}
		pthread_barrier_wait(&bar1);
		if(thread_part==0)
		{
			clock_gettime(CLOCK_MONOTONIC,&end_time);
		}
}
/*
	Function Name: merge
	Description: Combines the sorted vectors from their indices
	Inputs: nums- vector to be printed.
			left - Left index of the splitted vector
			middle - the middle portion of the splited vector.
			Right - Right index of the splitted vector.
	Returns: Nothing.
*/
void merge(vector<int> &nums,int left,int middle,int right)
{	
	//Size of the left side array
	int leftsize = middle - left + 1;
	//Size of the Right side array
	int rightsize = right - middle;
	//Temp arrays to save the numbers
	int* lefttemp = new int[leftsize];
	int* righttemp = new int[rightsize];
	int i,j,k;
	// Save the arrays from the left and right to temp.
	for(i=0;i<leftsize;i++)
	{
		lefttemp[i] = nums[i+left];
	}
	for(i=0;i<rightsize;i++)
	{
		righttemp[i] = nums[middle+i+1];
	}
	i=0;j=0;k=left;
	while(i<leftsize && j<rightsize)
	{
	// compare the values in two arrays and kept on adding to the final
	// array until one of them exhausts.
		if(lefttemp[i]<=righttemp[j])
		{
			nums[k]=lefttemp[i];
			i++;
		}
		else
		{
			nums[k]=righttemp[j];
			j++;
		}
		k++;
	}
	// Store the remaining value inside the array.
	while(i<leftsize)
	{
		nums[k]=lefttemp[i];
		i++;
		k++;
	}
	while(j<rightsize)
	{
		nums[k]=righttemp[j];
		j++;
		k++;
	}
}
/*
	Function Name: final_merge_sorted
	Description: Merges different part of sorting done by threads in one array
	Inputs: nums- vector to be printed.
			num_thread - thread number .
			agg - Aggregate, no of times this function is called.
	Returns: Nothing.
*/
void final_merge_sorted(vector<int> &nums,int num_thread,int agg)
{
	int size = UnsortedArray.size();
	for(int i=0;i<num_thread;i+=2)
	{	
		int left = i*(size/thread_num)*agg;
		int right = ((i+2)*(size/thread_num)*agg)- 1;
		int middle = left + ((size/thread_num)*agg) - 1;
		if(right >= size)
		{
			right = size - 1;
		}
		merge(nums,left,middle,right);
	}
	if(num_thread/2 >= 1)
	{
		final_merge_sorted(nums,num_thread/2,agg*2);
	}
}
/*
	Function Name: BAR1_init
	Description: Initializes Barrier
	Inputs: None
	Returns: Nothing.
*/
void BAR1_init()
{
	pthread_barrier_init(&bar1, NULL, thread_num);
}