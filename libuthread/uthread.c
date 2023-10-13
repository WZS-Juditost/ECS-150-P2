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

#define UTHREAD_STACK_SIZE 32768

queue_t ready_queue;
uthread_ctx_t idle;
unsigned by_block = 0;
bool preem_bool = false;

typedef struct uthread_tcb {
	uthread_ctx_t *context;
	uthread_func_t func;
	void *arg;
	void* stack;
}uthread_tcb;

uthread_tcb *curr;
uthread_tcb *prev;

struct uthread_tcb *uthread_current(void)
{
	if(by_block == 1) {
		by_block -= 1;
		return prev;
	}
	void* curr_thread;
	if (preem_bool == true) { preempt_disable(); }	
	queue_dequeue(ready_queue, &curr_thread);
	if (preem_bool == true) { preempt_enable(); }	
	uthread_tcb *current_thread = (uthread_tcb*) curr_thread;
	if(current_thread == NULL) { return NULL; }
	curr = curr_thread;
	return current_thread;
}

void uthread_yield(void)
{	
	if (queue_length(ready_queue) > 0){
		prev = curr;
		if (preem_bool == true) { preempt_disable(); }	
		queue_enqueue(ready_queue, prev);
		if (preem_bool == true) { preempt_enable(); }	
		curr = uthread_current();
		uthread_ctx_switch(prev->context, curr->context);
	} else {
		uthread_ctx_switch(prev->context, &idle);
	}
}

void uthread_exit(void)
{
	if(queue_length(ready_queue) == 0) {
		queue_destroy(ready_queue);
		setcontext(&idle);
	}
	curr = uthread_current();
	setcontext(curr->context);
}

int uthread_create(uthread_func_t func, void *arg)
{
	uthread_tcb *new_thread = (uthread_tcb *)malloc(sizeof(uthread_tcb));
	if(new_thread == NULL) { return -1; }
	new_thread->func = func;
	new_thread->arg = arg;
	new_thread->context = (uthread_ctx_t *) malloc(sizeof(uthread_ctx_t));
	if(new_thread->context == NULL) { return -1; }
	new_thread->stack = uthread_ctx_alloc_stack();
	if(uthread_ctx_init(new_thread->context, new_thread->stack, func, arg)==-1){
		return -1;
	}
	if (preem_bool == true) { preempt_disable(); }	
	queue_enqueue(ready_queue, new_thread);
	if (preem_bool == true) { preempt_enable(); }
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	preem_bool = preempt;
	ready_queue = queue_create();
	preempt_start(preempt);
	if (ready_queue == NULL) { return -1;}
	uthread_create(func, arg);
	curr = uthread_current();
	uthread_ctx_switch(&idle, curr->context);
	if(preempt == true) {
		preempt_stop();
	}
	return 0;
}

void uthread_block(void)
{
	by_block += 1;
	prev = curr;
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	if (preem_bool == true) { preempt_disable(); }	
	queue_enqueue(ready_queue, uthread);
	if (preem_bool == true) { preempt_enable(); }	
}

