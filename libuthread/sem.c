#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.c"

typedef struct semaphore {
	size_t count;
	queue_t block_queue;
}semaphore;

sem_t sem_create(size_t count)
{
	semaphore *new_sem = (semaphore *)malloc(sizeof(semaphore));
	if(new_sem == NULL) { return NULL; }
	new_sem->count = count;
	new_sem->block_queue = queue_create();
	return new_sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL) { return -1; }
	queue_destroy(sem->block_queue);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if (sem == NULL) { return -1; }
	if(sem->count == 0) {
		uthread_block();
		uthread_tcb *to_block = uthread_current();
		uthread_tcb *to_run = uthread_current();
		queue_enqueue(sem->block_queue, to_block);
		uthread_ctx_switch(to_block->context, to_run->context);
	}
	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{
	if (sem == NULL) { return -1; }
	sem->count++;
	if(sem->count > 0) {
		if(queue_length(sem->block_queue) > 0) {
			void *to_unblock; 
			queue_dequeue(sem->block_queue, &to_unblock);
			uthread_tcb *waken_thread = (uthread_tcb*) to_unblock;
			uthread_unblock(waken_thread);
		}
	}
	return 0;
}
