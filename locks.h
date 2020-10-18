#ifndef LOCKS_H
#define LOCKS_H

using namespace std;

#include <atomic>
#include <iostream>


void tas_lock();
void tas_unlock();

class Node
{
public:
	atomic<Node*> next;
	atomic<bool> wait;
};
class MCS_Lock
{
	atomic<Node*> tail;
public:
	void lock(Node* myNode)
	{
		Node* Null_ptr = NULL;
		myNode->next.store(Null_ptr);
		myNode->wait.store(true);
		Node* oldTail = tail.exchange(myNode);
		if(oldTail)
		{
			oldTail->next.store(myNode);
			while(myNode->wait.load());
		}
	}
	void unlock(Node* myNode)
	{
		Node* succ = myNode->next.load();
		if(!succ)
		{
			auto expected = myNode;
			Node* Null_ptr = NULL;
			if(atomic_compare_exchange_strong(&tail,&expected,Null_ptr))
				return;
			do
			{
				succ = myNode->next.load();
			}while(succ=nullptr);
		}
		succ->wait.store(false);
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
		{	cout<<"\n\rMy sense:"<<my_sense<<"\n\rsense:"<<sense<<"\n\r";
			while(sense.load()!=my_sense);
		}
	}
};

void tas_lock();
void tas_unlock();
void ttas_lock();
void ttas_unlock();
void ticket_lock();
void ticket_unlock();
#endif