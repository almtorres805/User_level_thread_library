#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore
{
	int count;
	queue_t blocked_q;
};

sem_t sem_create(size_t count)
{
	// Init semaphore pointer, allocated memory for the sempahore type
	sem_t sem_thread = malloc(sizeof(struct semaphore)); 
	if(sem_thread == NULL){
		return NULL;
	}

	// Create blocked queue
	sem_thread->blocked_q = queue_create();

	// Intialize count to given count
	sem_thread->count = count;

	return sem_thread;
}

int sem_destroy(sem_t sem)
{
	// Check if threads still blocked on queue
	if (sem == NULL || queue_length(sem->blocked_q) != 0 ){
		return -1;
	}

	queue_destroy(sem->blocked_q);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}

	// Check count and lock thread if count is 0
	if(sem->count == 0){
		
    struct uthread_tcb *current_t = uthread_current();
		queue_enqueue(sem->blocked_q,current_t);
		uthread_block(); // This will remove the current thread from the ready queue
	}

	// Count was 1 
	sem->count = 0;
	return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}

	sem->count = 1;

	// Check blocked_q if there are any threads waiting
	if (queue_length(sem->blocked_q) > 0){
		struct uthread_tcb *blocked_t;
		
    // Get the current thread from blocked queue
		queue_dequeue(sem->blocked_q,(void**)&blocked_t);
		uthread_unblock(blocked_t);
	}
	return 0;
}

