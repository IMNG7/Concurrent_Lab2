#include <iostream>
#include <atomic>
#include "locks.h"

using namespace std;


// void MCS_Lock::lock(Node* myNode)
// {	
// 	Node* oldTail = tail.load();
// 	myNode->next.store(NULL);
// 	while(!atomic_compare_exchange_strong(&tail,&oldTail,myNode))
// 	{
// 		oldTail = tail.load();
// 	}
// 	if(oldTail != NULL)
// 	{
// 		myNode->wait.store(true);
// 		oldTail->next.store(myNode);
// 		while(myNode->wait.load());
// 	}
// }
// void MCS_Lock::unlock(Node* myNode)
// {
// 	Node* Null_ptr = NULL;
// 	//Null_ptr.store(NULL);
// 	if(atomic_compare_exchange_strong(&tail,&myNode,Null_ptr))
// 	{

// 	}
// 	else
// 	{
// 		while(myNode->next.load() == NULL);
// 		Node* temp;
// 		temp->wait = false;
// 		myNode->next.store(temp);
// 	}
// }


atomic<bool> lock_flag = ATOMIC_FLAG_INIT;
atomic<int> next_num(0);
atomic<int> now_serving(0);

void tas_lock()
{
	while(lock_flag.exchange(true));
}
void tas_unlock()
{
	lock_flag.store(false);
}

void ttas_lock()
{
	do
	{
		while(lock_flag);
	}while(lock_flag.exchange(true));
}
void ttas_unlock()
{
	lock_flag.store(false);
}

void ticket_lock()
{
	int my_num = atomic_fetch_add(&next_num,1);
	while(now_serving.load()!=my_num);
}
void ticket_unlock()
{
	atomic_fetch_add(&now_serving,1);
}