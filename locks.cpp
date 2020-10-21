/*
	FileName: locks.cpp
	Description:Contains the function definitions required to perform bucketsort operation on the given numbers.
	Author: Nitik Gupta
	References Used: For TAS Algorithm: https://en.wikipedia.org/wiki/Test-and-set
					For Fetch and increment : https://en.cppreference.com/w/cpp/atomic/atomic_fetch_add
					For atomic: https://en.cppreference.com/w/cpp/atomic/atomic
*/
#include <iostream>
#include <atomic>
#include "locks.h"

using namespace std;

// Intializing the lock flag for TAS and TTAS
atomic<bool> lock_flag = ATOMIC_FLAG_INIT;
// Used for ticket lock
atomic<int> next_num(0);
atomic<int> now_serving(0); // Used for ticket lock
/*
	Function Name: tas_lock
	Description: Uses Test and set lock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void tas_lock()
{
// Wait until the lock flag is true
	while(lock_flag.exchange(true));
}
/*
	Function Name: tas_unlock
	Description: Uses Test and Set unlock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void tas_unlock()
{
// To unlock store false in lock flag
	lock_flag.store(false);
}
/*
	Function Name: ttas_lock
	Description: Uses Test and test and set lock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void ttas_lock()
{
// First test if lock flag is false(nobody has acquired it)
	do
	{
		while(lock_flag);
// If nobody has acquired it, acquire it for yourself
	}while(lock_flag.exchange(true));
}
/*
	Function Name: ttas_unlock
	Description: Uses est and test and set unlock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void ttas_unlock()
{
// To unlock store false in lock flag
	lock_flag.store(false);
}
/*
	Function Name: ticket_lock
	Description: Uses ticket lock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void ticket_lock()
{	//Increment the my_num to get the thread waiting number
	int my_num = atomic_fetch_add(&next_num,1);
	// Wait until its thread's turn
	while(now_serving.load()!=my_num);
}
/*
	Function Name: ticket_unlock
	Description: Uses Ticket unlock for locking primitive
	Inputs: None
	Returns: Nothing.
*/
void ticket_unlock()
{
	// Increment now serving to give the lock to next thread
	atomic_fetch_add(&now_serving,1);
}