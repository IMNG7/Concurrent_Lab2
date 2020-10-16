#ifndef LOCKS_H
#define LOCKS_H

void tas_lock();
void tas_unlock();
void ttas_lock();
void ttas_unlock();
void ticket_lock();
void ticket_unlock();
#endif