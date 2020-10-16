/*
	FileName: mergesort.h
	Description:Contains the function declarations required to perform mergesort operation on the given numbers.
	Author: Nitik Gupta

*/
#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>
#include <iostream>

using namespace std;

//pthread_barrier_t bar;
// extern pthread_barrier_t bar;
/*
	Function Name: mergesort
	Description: Initial recursive function to split the vector for sorting
*/
void mergesort(vector<int> &nums,int left,int right);
/*
	Function Name: merge
	Description: Combines the sorted vectors from their indices
*/
void merge(vector<int> &nums,int left,int middle,int right);
/*
	Function Name: mergesort_thread
	Description: Initial recursive function to split the vector for sorting for single thread
*/
void* mergesort_thread(void* args);
/*
	Function Name: final_merge_sorted
	Description: Merges different part of sorting done by threads in one array
*/
void final_merge_sorted(vector<int> &nums,int num_thread,int agg);
/*
	Function Name: BAR1_init
	Description: Initializes Barrier
*/
void BAR1_init();

#endif