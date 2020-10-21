/*
	FileName: locks.h 
	Description: Contains the function declaration and classes of all the locking functions and barriers that performs different locking and barrier mechanism required for assignment.
	Author: Nitik Gupta
	References Used: For MCS Lock: https://stackoverflow.com/questions/61944469/problems-with-mcs-lock-implementation
					 For Sense Reversal Barrier: https://en.wikipedia.org/wiki/Barrier_(computer_science) or professor's slide
					 For alignas : https://stackoverflow.com/questions/29824570/segfault-in-stdatomic-load
*/
#ifndef LOCKS_H
#define LOCKS_H

using namespace std;

#include <atomic>
#include <iostream>

// Class for creating a queue node
class alignas(16) Node
{
public:
	// Contains the address for next node
	atomic<Node*> next;
	// Tells the Node to wait 
	atomic<bool> wait;
	// Constructor for Node
	Node()
	{
		next = NULL;
		wait = false;
	}
};
// Class for creating the object for MCS Lock contains functions for locka and unlock
class MCS_Lock
{
	atomic<Node*> tail;
public:
	// Function for locking the primitive. Uses the nodes to determine if it needs to wait or acquire lock.
	void lock(Node* myNode)
	{
		// Intializes the node
		myNode->next.store(nullptr,memory_order_release);
		myNode->wait.store(true);
		// Gets the oldtail value
		Node* oldTail = tail.exchange(myNode,memory_order_seq_cst);
		if(oldTail)
		{	// if oldtail is null, wait for others to complete their execution and your turn to acquire lock
			oldTail->next.store(myNode,memory_order_seq_cst);
			while(myNode->wait.load(memory_order_seq_cst));
		}
	}
	// Fucntion for unlocking the primitive. Uses the node to unlock
	void unlock(Node* myNode)
	{
		Node* succ = new Node();
		// the successor defined for the next thread to acquire the lock.
		succ = myNode->next.load(memory_order_release);
		if(!succ)
		{
			auto expected = myNode;	// Object so that it doesn't change the original
			if(tail.compare_exchange_strong(expected,nullptr,memory_order_seq_cst))
				return;
			do
			{
				// Gets the next successor
				succ = myNode->next.load(memory_order_seq_cst);
			}while(succ==nullptr);
		}
		// Tells the successor not to wait.
		succ->wait.store(false,memory_order_seq_cst);
	}
};
class barrier_sense
{
	atomic<int> cnt;
	atomic<int> sense;
	int N;
public:
	// Initializes the barrier values
	void initialize_bar_values(int thread_num)
	{
		cnt=0; sense=0; N = thread_num-1;
	}
	// Function to wait for other threads to reach this point
	void wait()
	{	// local thread for the sense variable
		thread_local int my_sense = 0;
		if(my_sense == 0) 
			my_sense = 1;
		else
			my_sense = 0;
		int cnt_cpy = atomic_fetch_add(&cnt,1); // Increment the copy of counter
		if(cnt_cpy == N)	// Check if the counter reached the last thread
		{
			cnt.store(0);	// reset the counter
			sense.store(my_sense);
		}
		else
		{
			while(sense.load()!=my_sense); // Wait until the last thread reaches wait.
		}
	}
};
// Enum for defining the lock type
enum lock_type
{
	tas,
	ttas,
	ticket,
	pthread,
	mcs
};
/*
	Function Name: tas_lock
	Description: Uses Test and set lock for locking primitive
*/
void tas_lock();
/*
	Function Name: tas_unlock
	Description: Uses Test and Set unlock for locking primitive
*/
void tas_unlock();
/*
	Function Name: ttas_lock
	Description: Uses Test and test and set lock for locking primitive
*/
void ttas_lock();
/*
	Function Name: ttas_unlock
	Description: Uses est and test and set unlock for locking primitive
*/
void ttas_unlock();
/*
	Function Name: ticket_lock
	Description: Uses ticket lock for locking primitive
*/
void ticket_lock();
/*
	Function Name: ticket_unlock
	Description: Uses Ticket unlock for locking primitive
*/
void ticket_unlock();
/*
	Function Name: pthread_lock
	Description: Uses Pthread lock for locking primitive
*/
void pthread_lock();
/*
	Function Name: pthread_unlock
	Description: Uses Pthread unlock for locking primitive
*/
void pthread_unlock();


#endif