/*
	FileName: util.cpp
	Description: Contains the function definitions of all the miscellaneous utility functions that performed inside the main function like print, write to file etc.
	Author: Nitik Gupta
	References Used: for file handling C++:
					 https://www.geeksforgeeks.org/file-handling-c-classes/ 
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include "util.h"

/*
	Function Name: add_to_file
	Description: Write the vector values into the file
	Inputs: nums - The vector containing the sorted values.
			filename- File to be written into.
	Returns: Nothing
*/
void add_to_file(vector<int> &nums,string filename)
{
	cout<<"\n\r Writing to file";
	fstream file;
	//Opens the file to write the values to
	file.open(filename,ios::out);
	if(file.is_open())
	for(int i=0;i<nums.size();i++)
	{
		//Insert the values in file
		file<<nums[i]<<endl;
	}
}
/*
	Function Name: ConvertToVector
	Description: Takes the file and its values and stores it in a vector
	Inputs: filename- File to be taken from.
	Returns: Vector- The vector where values are written.
*/
vector<int> ConvertToVector(const string& filename)
{
	ifstream file(filename);
	// Opens the file to read the values from
	if(file.is_open())
	{
		vector<string> nums;
		vector<int> num;
		string line;
		//Take the values till end of line is achieved
		while(getline(file,line))
		{
			nums.push_back(line);
		}
		// Check file if it is empty. if empty, exit program.
		if(nums.empty())
		{
			cout<<"\n\r Nothing in File. EXITING";
			exit(0);
		}
		// Store the string values in vector
		for(int i=0;i<nums.size();i++)
		{
			stringstream s(nums[i]);
			int x=0;
			s >> x;
			num.push_back(x);
		}
		return num;
	}
	else
	{
		cout<<"\n\r FILE NOT FOUND. EXITING \n\r";
		exit(0);
	}
}
/*
	Function Name: printIntVector
	Description: Print the vector Provided
	Inputs: nums- vector to be printed.
	Returns: Nothing.
*/
void printIntVector(vector<int> nums)
{
	for(int i=0;i<nums.size();i++)
	{
		cout<<"\n\r"<<nums[i];
	}
}
int get_range(vector<int> &nums)
{
	int min=INT_MAX,max=INT_MIN;
	int size = nums.size();
	for(int i=0;i<size;i++)
	{
		if(min>nums[i]) min = nums[i];
		else if(max<nums[i]) max = nums[i];
	}
	int range = max - min;
	return range;
}