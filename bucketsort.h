/*
	FileName: bucketsort.h 
	Description: Contains the function declarations required to perform bucketsort operation on the given numbers.
	Author: Nitik Gupta
	References Used: For Bucket Sort Algorithm:
					 https://www.geeksforgeeks.org/bucket-sort-2/
*/
#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <vector>
//#include <algorithm>

using namespace std;

/*
	Function Name: bucketsort_thread
	Description: Initial recursive function to split the vector for sorting for single thread
*/
void* bucketsort_thread(void* args);
/*
	Function Name: bucketsort
	Description: Sorting function to initialize and start the bucket sort with threading
*/
void bucketsort(pthread_t *threads);

#endif