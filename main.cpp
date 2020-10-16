/*
	FileName: main.cpp
	Description:Contains the main function where the command line arguments are captured using the getopt_long function. 
				These arguments are then processed for their respective functionality.
	Author: Nitik Gupta
	References Used: for command line arguments:
					 https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/  
					 https://stackoverflow.com/questions/30141000/c-how-to-pass-command-line-argument-to-read-txt-file/30141127 
					 https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/ 
					 for getopt_long:
					 https://linux.die.net/man/3/
					 https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <pthread.h>
#include "util.h"
#include "mergesort.h"
#include "quicksort.h"
#include "bucketsort.h"
#include <stdlib.h>
#include <unistd.h>

using namespace std;
vector<int> UnsortedArray = {0};
int thread_num=5;
int offset=0;
struct timespec start= (struct timespec){0}, end_time= (struct timespec){0};
int main(int argc, char *args[])									 
{	int c;
	string input_file,output_file,algorithm;
	while(1)
	{
		int count = 0;
		//Structure used in getopt_long to capture the command line arguments.
		static struct option long_options[] = {						
			{"name",	no_argument,		0,	'n'},
			{"output",	required_argument,	0,	'o'},
			{"alg", 	required_argument,	0,	'a'},
			{"threads", required_argument,	0,	't'},
			{0,			0,					0,	0}
		};
		// Used to capture the command line arguments
		c= getopt_long(argc,args,"no:t:a:",long_options,&count);		
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
			case 'a' :	
						// Saves the algorithm name in the variable
						algorithm = optarg;							
						break;
			case 't' :	// Saves number of threads to be used
						thread_num = atoi(optarg);
						break;
			default	:	
						break;
		}
	}
	// The input file doesn't have a command variable, so it will be an 
	// extra variable that needs to be saved.
	if(optind < argc)
	{
		while(optind < argc)										
		{	
			input_file = args[optind++];
		}
	}
	// convert the input_file name into string and save into filename
	if(input_file.empty())
	{
		// If file Not Given
		cout<<"\n\r No Input File Given, Exiting. \n\r";
		return 0;
	}
	string filename(input_file);
	// Takes the values inside the file and stores them in a vector 
	UnsortedArray = ConvertToVector(filename);
	//cout<<"\n\rSIZE GIVEN:"<<UnsortedArray.size()<<"\n\r";
	pthread_t threads[thread_num];
	ssize_t* argt = new ssize_t[thread_num+1];
	int ret;
	offset = UnsortedArray.size() % thread_num;
	//pthread_barrier_init(&bar, NULL, thread_num);
	if(algorithm == "merge")
	{	BAR1_init();
		//Sending the Unsorted Array to mergesort function.
		cout<<"\n\rDoing mergesort";
		for(int i=0;i<thread_num;i++)
		{
			argt[i]=i;
			//Creates threads for merge sort
			ret = pthread_create(&threads[i],NULL,&mergesort_thread,&argt[i]);
			if(ret)
			{
				cout<<"ERROR WHILE CREATION";
				exit(-1);
			}
			cout<<"\n\rThreads "<<i<<" Created";
		}
		for(int i=0;i<thread_num;i++)
		{
			//Joining the threads
			ret = pthread_join(threads[i],NULL);
			if(ret)
			{
				printf("ERROR; pthread_join: %d\n", ret);
				exit(-1);
			}
			cout<<"\n\rThreads "<<i<<" Joined";
		}
		final_merge_sorted(UnsortedArray,thread_num,1);
	}
	else if(algorithm == "quick")
	{
		BAR2_init();
		//Sending the Unsorted Array to mergesort function.
		cout<<"\n\rDoing quicksort";
		for(int i=0;i<thread_num;i++)
		{
			argt[i]=i;
			//Creates threads for quick sort
			ret = pthread_create(&threads[i],NULL,&quicksort_thread,&argt[i]);
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
		//final_merge_sorted(UnsortedArray,thread_num,1);
		final_quick_sorted(UnsortedArray,thread_num,1);
	}
	else if(algorithm == "bucket")
	{	
		cout<<"\n\rDoing bucketsort";
		//Sending the threads reference to BucketSort function.
		bucketsort(threads);
	}
	else
	{
		//Since there was not algorithm provided, will do the mergesort
		// by default
		cout<<"Wrong Choice,Doing Merge Sort By Default without threads";
		mergesort(UnsortedArray,0,UnsortedArray.size()-1);
	}
	//If output_file is provided, then send the Array to output_file
	if(!(output_file.empty())) add_to_file(UnsortedArray,output_file);
	//If output_file is not provided, then send the Array to console
	else
	{
		cout<<"\n\r Output File Not Given Printing the Sorted Array:";
		printIntVector(UnsortedArray);
	}
	//Calculates the time elapsed in completing the thread
	unsigned long long elapsed_ns;
	elapsed_ns = (end_time.tv_sec-start.tv_sec)*1000000000 + (end_time.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	return 0;
}
