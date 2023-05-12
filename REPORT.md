# User-level Thread Library
## Queue API (Phase 1)
Our queue data structure follows a doubly linked list implementation because we 
needed to ensure O(1) operations for managing the threads in the queue. To 
accomplish this we defined two structs to populate our queue. The first struct 
is called "node," which contains the address of a thread and a pointer to the 
next node. The node structure is a substructure of our queue structure. The 
“queue” struct internally holds the current size of the queue and two nodes: 
the front (oldest) node and the rear (newest) node.

Each function in the queue handles error management at the beginning. If the 
inputted queue, data, or allocated space has a value of NULL, the function 
returns -1. A return value of 0 indicates success.

The following design allowed for executing the enqueue, dequeue, and length 
functions in O(1) time. Having access to the rear pointer the enqueued node is 
appended to the front of the most recent node, and the rear pointer is updated. 
Similar to the rear, when a dequeue operation was called we would update the 
front node to point to where the current front’s next pointed to 
(second oldest node). However, there are two edge cases if both functions are 
called when the queue is empty. If a node is enqueued in an empty queue then 
the rear and front should both point to it. If the last node is dequeued then 
the rear should be NULL to ensure the queue's reliability. Additionally, we 
used the queues internal counter to appropriately update the length each time 
the enqueue, dequeue, or delete functions were called.

An exception to the O(1) constraints were the delete and iterate functions, 
which required the queue to be iterated resulting in O(N) time. Deleting a 
node in the middle/end of the queue follows a similar concept to dequeuing. 
Deleting a node from the front results in O(1) time because we just need to 
simply point the front to where the deleted node pointed to (second oldest). 
The iterate function is relatively simple. It iterates through each node in 
the queue and passes the required arguments to the user-defined callback 
function. This function is designed to be resistant to deletion, meaning that 
if a node is deleted within the callback function, a segmentation fault won't 
occur because the function avoids calling dereferenced deleted nodes.

To ensure the proper functionality of our queue implementation, we designed
a comprehensive set of tests that cover key aspects of the queue operations.
We wanted to ensure the test functions handled edge cases appropriately, and
reliable and efficient queue operations. We started off by testing the
conditions that should return -1, to ensure reliability of the queue. This
included: passing in null values as parameters to the functions, ensuring the
length of the queue remained consistent, and testing all values that are
supposed to return -1. We also found it helpful to create simple and complex
test functions. This helped ensure that we did not miss any important test
cases that can lead to problems for the later phases of the project. 

## Uthread API (Phase 2)
We have implemented the uthread interface, which provides the uthread API for
managing threads. Internally, we utilize two global queues and a data
structure called the thread control block (TCB) to store information about
each thread. The TCB holds the thread's context (a set of registers), stack
pointer (points to the top of the stack), and its current state. A thread can
be in one of four states: READY, RUNNING, BLOCKED (for phase 3), or TERMINATED.

The main function of our uthread API is called uthread_run(). This function
can be divided into three sections: the idle thread, scheduler execution, and
deallocation of terminated threads. In the first section, we initialize the
initial thread, which runs in the background and manages thread resources.
Initially, the current thread points to the idle thread. Then, we call the
uthread_create() function to generate a new (parent) thread.

The second section focuses on iterating through the ready queue to check if it
is empty. If the queue is not empty, the scheduler continues to yield threads
in a round-robin fashion. In the last section, we focus on memory management
by deallocating threads that have been terminated, but only when the ready
queue is empty.

Once the user-level thread library has been initialized by calling
uthread_run(), users can create and manage threads. The uthread_create()
function is straightforward. Each time it is called, a new thread is initiated
with its corresponding TCB properties. Then, the uthread_ctx_init() function
is invoked to execute the thread's execution context, which is added to the
ready queue for scheduling.

Our uthread API enables thread switching, also known as context switching.
We utilize the uthread_ctx_switch() function from the private.h API to perform
the switches. First, we create two internal contexts: the currently running
thread is stored as the previous context, and the oldest node in the queue is
stored as the next context to be executed. During the switch, the previous
context's contents are stored, and the next context's TCB properties are
restored before being scheduled to run. In a special case where the currently
running thread has not finished executing, it is enqueued back in the ready
queue to await scheduling.

The uthread_exit() function is called when the bootstrap function in context.c
finishes executing the currently running thread or when a user wants to exit a
thread prematurely. We deallocate the thread's pointer to the top of the stack
to prevent it from resuming execution. Additionally, we set the thread's state
to terminated to indicate that its resources can be deallocated.


## Semaphore API (Phase 3)
Our Semaphore implementation was going to be the first real scheduling for our
created user thread library. We thought of our semaphore implementation similar 
to the lock and unlock functionality we learned in class. Semaphores are
responsible for specfic shared resources in a program. In our case the shared
reasource is the CPU/thread of execution or it is respoblie for which thread is
currently running.Therefore, we created our semaphore struct with only a count 
and a queue called blocked queue. The value of count would be limited to 0 or 1, 
making this implentation a binary semaphore implementation. The blocked queue is 
the queue that holds all threads waiting for the resource of the given semaphore, 
in this case it was the thread of execution since our multithread was meant to be 
ran on a single CPU. Our semaphore library consists of 4 functions create(), destroy(), 
down() and up().The create() function took an integer value as its arguement 
either being 0 or 1, representing the initial avaliablity to the semaphore's 
resources. Calling create_sem(0) meant that the semaphore's resource was busy 
therefore calling sem_down() again on the same semaphore would put that thread into 
the semaphore's blocked queue. If the function was called as create_sem(1) that made
the resources available to take by the next sem_down() call. Since we have a create()
function which creates and returns a pointer of struct semaphore as described, we also
need a destroy() function that will destroy or free that created semaphore, reducing
memory leaks and practing good memory management. Our created function intially checks 
if the semaphore we want to destroy is NULL or if the semaphore's blocked queue is not 
empty, if so return -1. If the blocked queue is not empty that means we are attempting 
to destroy a semaphore with threads waiting to be ran. If we did not include this we 
would lose that thread forever. The reason being is that in out implemention are blocked 
queues are limited to the semaphore only and once we remove them from the ready queue 
that exist in the uthread library we can no longer run them until they are put back. 
Looking back now this could be an issue if the uthread_run function ends and we still 
have threads in the blocked queue, it will not know since it is only reading the value 
of the ready queue until it is 0. One solution could be to also have a global blocked 
thread in the uthread library to ensure all threads go from ready to terminated without 
ever being lost. Our other two functions sem_up() and sem_down() are the most important 
as they are responsible for the lock and unlock behavior of the semaphore, allowing us to 
schedule execution of the threads. Starting with sem_down() which is also know as a 'P'
operation, which is meant to grab a resource from the semaphore. The sem_down() function took
a semaphore pointer as an argument. We then check the semaphore's count value, if the value
was 1 then we would change it to 0, indicating the semaphore's resource is now being used
making it unavaliable until it is released. The other sceninero in sem_down() is the case
if the given argument semaphore is 0, then we create a uthread_tcb pointer to retrieve
the current thread calling the semaphore, then we enqueued that thread to the semaphore's
blocked queue to hold and blocked the queue in the unthread library by calling uthread_block().
Block() located in the private.h library making it accessible by sem.c was implemented in
the unthread.c file. It simply changed the state of the thread to "BLOCKED" and called 
yield(). The yield function then dequened it from ready and rather to enqueue it back it was 
gone and gave the thread of execution to the next thread. Finally, we leave that semaphore's
count as 0. The sem_up() fucntion also know as a 'V' operation is the polar opposite of the 
sem_down() function. The function takes in a semaphore pointer as its argument then changes
that semaphores count value to 1. It then checks the size of the semaphore's blocked queue
if the size of the blocked queue is 0, then the resource is avaiable to take. If the 
blocked queue size is not 0 then that means we have a thread waiting for the resource. To 
release the thread we call a uthread_tcb pointer, dequeue the first thread from the blocked 
list as the uthread_tcb pointer then we call the uthread_unblock() function to unblock the thread.
Similary, as unthread_block the uthread_unblocked function was defined in private.h and located
in uthread.c making acessible by sem.c. The unblock function takes in a uthread_tcb pointer as the 
argument. It changes the threads state to READY and enqueues the thread back on the ready thread to
be allowed for execution. This is the complete implentation of our Semaphore schduling implemention.
