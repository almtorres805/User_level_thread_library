#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

 

struct semaphore {
	/* TODO Phase 3 */
	int count;
	queue_t blocked_q;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	
	//init semaphore pointer, allocated memory for the sempahore type
	sem_t sem_thread = malloc(sizeof(struct semaphore)); 
	if(sem_thread == NULL){
		return NULL;
	}

	//create blocked queue
	sem_thread->blocked_q = queue_create();

	//intialize count to given count
	sem_thread->count = count;

	return sem_thread;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	//check if threads still blocked on queue
	if (sem == NULL || queue_length(sem->blocked_q) != 0 ){
		//there are still threads blocked in queue or sem NULL
		return -1;
	}

	//destroy its interal blocked queue
	queue_destroy(sem->blocked_q);

	//free semaphore pointer
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	
	if(sem == NULL){
		return -1;
	}

	//dont need lock until P4
	//check count 
	if(sem->count == 0){
		//since 0 then we lock thread
		//get current tcb
		struct uthread_tcb *current_t = uthread_current();

		//add current thread to the semaphore blocked queue
		queue_enqueue(sem->blocked_q,current_t);

		//Block
		uthread_block(); //this will remove the current thread from the ready queue
	}
	//count was 1 
	sem->count = 0;
	//unlock here

	return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */

	if(sem == NULL){
		return -1;
	}

	//set semaphore count to 1
	sem->count = 1;
	//check blocked_q if there are any threads waiting
	//lock
	if (queue_length(sem->blocked_q) > 0){

		//top of block_q thread
		struct uthread_tcb *blocked_t;

		//get the current thread from blocked queue
		queue_dequeue(sem->blocked_q,(void**)&blocked_t);
		
		//unlock thread
		uthread_unblock(blocked_t);
	}
	//unlock here
	return 0;
}

