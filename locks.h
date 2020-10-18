#ifndef LOCKS_H
#define LOCKS_H

using namespace std;

#include <atomic>

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

void tas_lock();
void tas_unlock();
void ttas_lock();
void ttas_unlock();
void ticket_lock();
void ticket_unlock();
#endif