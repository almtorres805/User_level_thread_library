# User_level_thread_library
This project for the ECS150 Operating Systems class involves the development of a User-level Thread Library, consisting of three main phases: Queue API, Uthread API, and Semaphore API.

## Phase 1: Queue API
In this phase, the project aims to create a queue data structure that supports user-level thread management. The queue uses a doubly linked list implementation to ensure O(1) operations for managing threads.

Functions in the queue perform error checks and return -1 for NULL inputs, with a return value of 0 indicating success. Key operations like enqueue, dequeue, and length are designed to run in O(1) time. Special cases are handled, such as enqueuing into an empty queue and dequeuing the last node. The delete and iterate functions require O(N) time as they involve iterating through the queue.

To ensure functionality, a comprehensive set of tests is designed, covering various scenarios, including edge cases.

## Phase 2: Uthread API
In this phase, the project implements the uthread interface for managing threads. It utilizes global queues and a thread control block (TCB) to store thread information. Threads can be in four states: READY, RUNNING, BLOCKED (for Phase 3), or TERMINATED.

The uthread_run() function initializes the initial thread, handles scheduling through a round-robin mechanism, and deallocates terminated threads when the ready queue is empty. Users can create and manage threads using the uthread_create() function, which initializes TCB properties and executes thread contexts.

Context switching, or thread switching, is enabled through the uthread_ctx_switch() function, allowing threads to yield the CPU. When threads exit, resources are deallocated using the uthread_exit() function.

## Phase 3: Semaphore API
The Semaphore API introduces scheduling to the user-level thread library. Semaphores manage shared resources, in this case, the CPU/thread of execution. A binary semaphore is implemented with a count and a blocked queue for waiting threads.

The sem_down() function, or 'P' operation, claims a resource from the semaphore, changing its count to 0 if available. The sem_up() function, or 'V' operation, releases a resource, changing the count to 1 and unblocking waiting threads.

Overall, the project aims to create a robust user-level thread library with efficient thread management, context switching, and semaphore-based resource control. Potential issues related to blocked threads are noted, suggesting the need for global blocked thread management in future improvements.
