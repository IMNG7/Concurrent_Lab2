#ifndef LOCKS_H
#define LOCKS_H

using namespace std;

#include <atomic>
#include <iostream>


class Node
{
public:
	atomic<Node*> next;
	atomic<bool> wait;
	Node()
	{
		next = NULL;
		wait = false;
	}
} __attribute__((packed));
class MCS_Lock
{
	atomic<Node*> tail;
public:
	void lock(Node* myNode)
	{
		myNode->next.store(nullptr);
		myNode->wait.store(true);
		Node* oldTail = tail.exchange(myNode,memory_order_acq_rel);
		if(oldTail)
		{	//myNode->wait.store(true,memory_order_relaxed);
			oldTail->next.store(myNode,memory_order_release);
			while(myNode->wait.load(memory_order_acquire));
		}
	}
	void unlock(Node* myNode)
	{
		Node* succ = new Node();
		succ = myNode->next.load(memory_order_acquire);
		if(!succ)
		{
			auto expected = myNode;
			if(tail.compare_exchange_strong(expected,nullptr,memory_order_acq_rel))
				return;
			do
			{
				succ = myNode->next.load(memory_order_acquire);
			}while(succ=nullptr);
		}
		succ->wait.store(false,memory_order_release);
	}
};
class barrier_sense
{
	atomic<int> cnt;
	atomic<int> sense;
	int N;
public:
	void initialize_bar_values(int thread_num)
	{
		cnt=0; sense=0; N = thread_num-1;
	}
	void wait()
	{
		thread_local int my_sense = 0;
		if(my_sense == 0) 
			my_sense = 1;
		else
			my_sense = 0;
		int cnt_cpy = atomic_fetch_add(&cnt,1);
		if(cnt_cpy == N)
		{
			cnt.store(0);
			sense.store(my_sense);
		}
		else
		{
			while(sense.load()!=my_sense);
		}
	}
};

enum lock_type
{
	tas,
	ttas,
	ticket,
	pthread,
	mcs
};

void tas_lock();
void tas_unlock();
void ttas_lock();
void ttas_unlock();
void ticket_lock();
void ticket_unlock();
void pthread_lock();
void pthread_unlock();


#endif