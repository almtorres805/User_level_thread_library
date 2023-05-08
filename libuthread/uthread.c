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

struct uthread_tcb {
	/* TODO Phase 2 */
  uthread_ctx_t *context; // From private.h that stores thread's execution context
  void *sp; // Stack pointer
  enum{
    IDLE,
    RUNNING,
    READY,
    EXITED
  }state;
};

struct uthread_tcb *uthread_current;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
  return uthread_current;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
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

  // Create a new thread
  uthread_ctx_init(new_t->context, new_t->sp , func, arg);

  // Add thread to the queue
  return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  // Create an idle thread
  // Initialize the tcb of the idle thread
  // Initialize the queues
  // Create a thread
  // while loop: if queue_length == 0 then break out
  // Destroy queue
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}
