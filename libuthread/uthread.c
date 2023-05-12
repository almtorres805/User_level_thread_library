#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

// Global Queues to manage threads
queue_t ready_q;
queue_t terminated_q;

struct uthread_tcb {
	/* TODO Phase 2 */
  uthread_ctx_t *context; // From private.h that stores thread's execution context
  void *sp; // Stack pointer
  enum{
    RUNNING,
    READY,
    TERMINATED,
    BLOCKED
  }state;
};

struct uthread_tcb *curr_t;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
  return curr_t;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
  /*Running Thread*/
  struct uthread_tcb *next_t;
  queue_dequeue(ready_q,(void**)&next_t); 

  //check to see if current there are not any threads to yield to
  // if(queue_length(ready_q) == 0){
  //   //cannot yeild since there are no threads left
  //   printf("No threads remaining");
  //   queue_enqueue(ready_q,prev_t); //add back till it terminates its function

  // }
  
  /*This is the thread waiting to run*/
  // struct uthread_tcb *next_t;
  // queue_dequeue(ready_q,(void**)&next_t); 

  uthread_ctx_t *prev = uthread_current()->context;
  uthread_ctx_t *next = next_t->context;
  
  /*Checking State of Current Thread*/
  // If current state is in RUNNING state, schedule for execution later
  if (uthread_current()->state == RUNNING){
    uthread_current()->state = READY;
    queue_enqueue(ready_q, uthread_current());
  }

  //If current state BLOCKED it will be enqueue to semaphore's blocked_q, utilzed by uthread_block(), 
  if(uthread_current()->state == BLOCKED){;}

  /*Assign new thread*/
  curr_t = next_t;  
  next_t->state = RUNNING;
  
  uthread_ctx_switch(prev, next);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */

  uthread_ctx_destroy_stack(curr_t->sp);
  curr_t->state = TERMINATED;
  queue_enqueue(terminated_q, curr_t);

  uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  // Create TCB for new thread
  struct uthread_tcb *new_t = malloc(sizeof(new_t));
  if (new_t == NULL){
    return -1;
  }
  
  // Allocate memory for the context of the thread
  new_t->context = malloc(sizeof(uthread_ctx_t));
  if(new_t->context == NULL){
    return -1;
  }

  // Initialize TCB
  new_t->state = READY;
  new_t->sp = uthread_ctx_alloc_stack();
  if(new_t->sp == NULL){
    return -1;
  }

  // Create a new thread
  uthread_ctx_init(new_t->context, new_t->sp , func, arg);

  //if its the first thread make current 
  if(queue_length(ready_q) == 0) {
    curr_t = new_t;
  }
  
  // Add thread to the queue
  queue_enqueue(ready_q, new_t);

  return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  // Create an idle thread
  struct uthread_tcb *idle_t = malloc(sizeof(idle_t));
  if (idle_t == NULL){
    free(idle_t);
    return -1;
  }
  
  if(preempt == false){;}
  
  idle_t->context = malloc(sizeof(uthread_ctx_t));
  if (idle_t->context == NULL) {
    free(idle_t);
    return -1;
  } 

  
  idle_t->state = RUNNING;
  curr_t = idle_t;

  // Initialize the queues
  ready_q = queue_create();
  terminated_q = queue_create();
  // Create a thread
  uthread_create(func, arg);
  //want to make this thread my current thread
  // while loop: if queue_length == 0 then break out
  while (queue_length(ready_q) != 0){
    //switch threads
    uthread_yield();
  }

  curr_t = idle_t;
  idle_t->state = TERMINATED;
  queue_enqueue(terminated_q, curr_t);
  //curr_t->state = TERMINATED;

  //queue_enqueue(terminated_q, idle_t);
  // All threads have been executed, now free the memory
  void *terminated_t;
  while (queue_length(terminated_q) != 0){
    queue_dequeue(terminated_q, (void**)&terminated_t);
    free(terminated_t);
  }

  // Destroy queue
  queue_destroy(ready_q);
  queue_destroy(terminated_q);
  return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
  //Reason to block : tried to sem_down() but count was 0
  
  //change state to BLOCKED
  curr_t->state = BLOCKED;

  //removed thread from running queue and context switch
  //Modied yield function to check for BLOCKED state  
  uthread_yield();

}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */

  //change state to READY
  uthread->state = READY;

  //add to running queue
  queue_enqueue(ready_q,uthread);

}
