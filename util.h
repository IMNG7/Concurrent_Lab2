/*
	FileName: util.cpp
	Description: Contains the function declarations of all the miscellaneous utility functions that performed inside the main function like print, write to file etc.
	Author: Nitik Gupta
*/
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <bits/stdc++.h>

using namespace std;

/*
	Function Name: printIntVector
	Description: Print the vector Provided
*/
void printIntVector(vector<int>);
/*
	Function Name: ConvertToVector
	Description: Takes the file and its values and stores it in a vector
*/
vector<int> ConvertToVector(const string&);
/*
	Function Name: add_to_file
	Description: Write the vector values into the file
*/
void add_to_file(vector<int> &nums,string filename);
int get_range(vector<int> &nums);

#endif