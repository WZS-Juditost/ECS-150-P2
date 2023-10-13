# User-level thread library

## Summary
`User-level thread library` is a program that run multiple threads on a main
function. we using the semaphore to control the resource of the threads.

## Implementation

Implementation of `User-level thread library` follows 3 steps.

1. Create a thread with the given arguments. 1st arguments is boolean variable
that manage preemption. 2nd argument is function that we want to run in the
thread. 3rd argument is the arguments that we want to pass to the function.
2. Excute the thread with the given function and arguments. If we have
multiple threads, then we will put other waiting thread into a ready queue. We
will excute the thread until we finished the task or yielded by the function in
the thread.
3. Terminate the thread if we finished the task. If we finished all thread
task, then destroyed ready queue and go back to Idle.

### Code structure

The code is divided into two sections. In the subfolder `apps`, there are 
various test apps. The tester `queue_tester_example.c` only focuses on the 
queue implementation. While the testers prefixed with `sem_` test the accuracy 
of semaphore, those prefixed with `uthread_` tests the thread library. The 
files in `libuthread` constitutes the entire User-level thread library. 
`queue.c` is the file for queue and all the API documentation it needs are 
contained in `queue.h`. `uthread.c` and `uthread.h` are files for thread 
managements. `sem.c` and `sem.h` contains the semaphore implementations and 
the interface of the semaphore API. `context.c` and `preempt.c` holds 
functions that will be called by uthread and semaphore. `private.h` contains 
the interface of `context.c`, `preempt.c`, and part of the interface of 
`uthread.c` and `sem.c`.

### Makefile

Makefile is used to generate a static library archive named `libuthread/
libuthread.a.` by combining all the files inside `libuthread`. The makefile 
inside `apps` will covert the test files to executable file and call the 
makefile inside `libuthread` to generate the static library.

### Queue

Queue is the data structure that perform operation in First In First Out
(FIFO) order. For our queue implementations, we used the singly linked list.
we have two sturct `queue` and `q_node`. `queue` is the sturct that store
pointers to the front of the queue and the rear of the queue. `queue` also
have variable called size so that we can easily track the size of the queue.
`q_node` is the sturct that have data and pointer that pointing the next node
in the queue. we initialized the queue with the `queue_create` and destroyed
the queue with the `queue_destroy` function. And also, we have the
`queue_enqueue` and `queue_dequeue` functions that put the arguments into back
of the queue and pop the data from front of the queue. Since we used the
singly linked list, we can't access the queue node directly. Thus, we have
`queue_iterate` functions so that we can easily run the function go through
each queue node.

### Uthread

Uthread.c and uthread.h are part of the uthread library. They take care of 
most of the thread management. There are five global variables. `ready_queue` 
is a FIFO queue stores all threads that are ready to run. `idle` is the 
context reserved for main/idle thread. `by_block` is a integer variable that 
indicates if we want the current running thread(by_block = 1) or the first 
thread in the ready queue(by_block = 0). `curr` and `prev` are a pointers 
points to current running thread and previous thread. `uthread_tcb` is a 
struct simulates thread control block(TCB). `context` is a ucontext_t pointer 
that points to the context of the thread. `func` is a pointer points to the 
function to be executed by the thread. `agrs` is a void pointer that points 
the arguments to be passed to the thread. `stack` is a pointer points to the 
top of a valid stack. `uthread_current` returns the first thread in the ready 
queue when by_block is equal to 0. If by_block = 1, means some function is 
asking for the current running thread, then it decrements by_block and return 
prev. `uthread_yield` set prev = curr, enqueue it to the ready queue, get the 
first thread in the ready queue, and swtich to that thread. `uthread_exit` 
check if the ready queue is empty. It will either return to idle thread when 
the queue is empty or switch to the first thread in the ready queue. 
`uthread_create` defines all variables in uthread_tcb and make the context. 
After create that thread, it enqueues that thread to the end of ready queue. 
`uthread_run` is only called by the process' original execution thread. It 
becomes the main/idle thread and create the first thread to be executed. It 
returns when all threads finish their execution. `uthread_block` is 
responsible to increment by_block and set prev equals curr. `uthread_unblock` 
unblocks the thread passed in by enqueue it to the ready queue.

### Semaphore

Semaphores are a mechanism to manage how many threads can access the same 
resources at once. It will utilize the blocking API functions that are defined 
in `private.h`. The struct `semaphore` contains two variable. `count` is the 
semaphore count. `block queue` is a queue that stores blocked threads. 
`sem_create` allocates memory for a new semaphore and initialize the internal 
count. `sem_destroy` destories the block queue and frees the memory of a 
semaphore. `sem_down` if a thread is trying to take an unavailable semaphore, 
this function will call `uthread_block`, get the current running thread and 
the first thread in the ready queue. Then it also takes the responsibility to 
enqueue the current running thread into block queue and starts executing other 
thread. `sem_up` increments internal count, unblocks the first thread in the 
block queue, and passed that queue `into uthread_block`

### Preemption

### Internally function
Uthread and semaphore uses some internal functions to help them schedule 
threads and allocate resources. `uthread_ctx_switch` swtich between two 
execution contexts. Used to schedule threads. `uthread_ctx_alloc_stack` 
allocates stack memory. `uthread_ctx_destroy_stack` deallocates stack memory. 
`uthread_ctx_init` initializes a thread's context. Used to create new threads.

Copyright 2022, Young Cheol Ko, ZeSheng Wang