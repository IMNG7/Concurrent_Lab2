/*
	FileName: counter.cpp
	Description: Contains the function declarations required to perform counter micro benchmarking.
	Author: Nitik Gupta
	References Used: For function Pointers: 
					https://www.cprogramming.com/tutorial/function-pointers.html
*/
#ifndef COUNTER_H
#define COUNTER_H
/*
	Function Name: thread_main
	Description: Initial recursive function for the micro benchmark
*/
void* thread_main(void* args);

#endif