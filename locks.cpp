#include <iostream>
#include <atomic>
#include "locks.h"

using namespace std;

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