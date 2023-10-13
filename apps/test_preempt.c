/*
 * Preempt test
 *
 * Test the synchronization of three threads, by having them print messages in
 * a certain order.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>

int times = 10000000;

static void thread2(void *arg)
{
	(void)arg;

	while(1) {
        times --;
	    printf("I'm thead2, times = %d\n", times);
	}
}

static void thread1(void *arg)
{
	(void)arg;

	uthread_create(thread2, NULL);
    
	while(1) {
        times --;
	    printf("I'm thead1, times = %d\n", times);
	}
}

int main(void)
{

	uthread_run(true, thread1, NULL);

	return 0;
}
