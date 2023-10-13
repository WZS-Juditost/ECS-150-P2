#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sa, sa_old;
struct itimerval it, it_old;

void preempt_disable(void)
{
	sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
}

void preempt_enable(void)
{
	sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL);
}

static void timer_handler (void)
{
	uthread_yield();
}

void preempt_start(bool preempt)
{
	if (preempt == true){
		sa.sa_handler = (__sighandler_t)&timer_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sigaddset(&sa.sa_mask, SIGVTALRM);
		sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL);
		sigaction(SIGVTALRM, &sa, &sa_old);
	
		//100HZ == 1/100. 10^-6 * 10000 = 0.01
		it.it_interval.tv_sec = 0;
		it.it_interval.tv_usec = 1000000 / HZ;
		it.it_value.tv_sec = 0;
		it.it_value.tv_usec = 1000000 / HZ;
		setitimer(ITIMER_VIRTUAL, &it, &it_old);
	}
}

void preempt_stop(void)
{
	sigaction(SIGVTALRM, &sa_old, NULL);
	setitimer(ITIMER_VIRTUAL, &it_old, NULL);
}