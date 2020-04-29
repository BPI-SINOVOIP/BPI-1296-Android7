/* 
 * Copyright (C) 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *
 * Author(s): Mauro Bisson
 */

#include <stdio.h>	/* printf */
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>	/* ETIMEDOUT */
#include <signal.h>
#include <string.h>	/* memcpy */
#include "timerlib.h"


/* time to wait the init of ticker thread in timer_init() function */
#define INIT_WAIT_TIME	5

/* for conversion purpose */
#define NANO_PER_MICRO	1000
#define MICRO_PER_SEC	1000000

/* max number of microsecond in a timeval struct */
#define MAX_USEC	999999

/* microseconds in a millisecond */
#define ALMOST_NOW	1000

#define TIMER_KIND	ITIMER_REAL
#define SIG_TO_WAIT	SIGALRM

/* 
 * evals true if struct timeval t1 defines a time strictly before t2;
 * false otherwise.
 */
#define TV_LESS_THAN(t1,t2)	(((t1).tv_sec < (t2).tv_sec) ||\
				(((t1).tv_sec == (t2).tv_sec) &&\
				 ((t1).tv_usec < (t2).tv_usec)))

/* 
 * Assign struct timeval tgt the time interval between the absolute
 * times t1 and t2. 
 * IT IS ASSUMED THAT t1 > t2, use TV_LESS_THAN macro to test it.
 */
#define TV_MINUS(t1,t2,tgt)	if ((t1).tv_usec >= (t2).tv_usec) {\
					(tgt).tv_sec = (t1).tv_sec -\
						       (t2).tv_sec;\
					(tgt).tv_usec = (t1).tv_usec -\
       						        (t2).tv_usec;\
				}\
				else {\
					(tgt).tv_sec = (t1).tv_sec -\
						       (t2).tv_sec -1;\
					(tgt).tv_usec = (t1).tv_usec +\
						(MAX_USEC - (t2).tv_usec);\
				}

typedef struct timer tl_timer_t;

static void timer_free(tl_timer_t * /*t*/);
static void timer_dequeue(tl_timer_t * /*t*/);
#if 0
static void timer_start(struct timeval * /*abs_to*/);
#endif
static void *cronometer(void * /*arg*/);
			
struct timer {

	struct timeval	timeout;
	void(*handler)(void *);
	void 		*handler_arg;
	int 		id;
	tl_timer_t 	*next;
	tl_timer_t 	*prev;
};

static struct {

	tl_timer_t 	*first;
	tl_timer_t 	*last;
	pthread_mutex_t mutex;
	pthread_cond_t	cond;
	pthread_t	ticker;
	int		cur_id;
} timerq;

static void timer_free(tl_timer_t *t) {

	if (t == NULL) return;
	free(t);
	return;
}

static void timer_dequeue(tl_timer_t *t) {

	if (t == NULL) return;

	if (t->prev == NULL)
		/* t is the first of the queue */
		timerq.first = t->next;
	else
		t->prev->next = t->next;

	if (t->next == NULL)
		/* t is the last of the queue */
		timerq.last = t->prev;
	else
		t->next->prev = t->prev;

	timer_free(t);
	return;
}

/* 
 * Set a timer with setititmer syscall to expire on abs_to time.
 * The parameter abs_to is an absolute time as those returned with
 * gettimeofday (since epoch).
 * If abs_to refers to a time in the past the timer will be set to 
 * expire in 1 millisecond.
 * This function has to be called in a critical section.
 */
#if 0
static void timer_start(struct timeval *abs_to) {

	struct itimerval	relative = {{0,0},{0,0}};
	struct timeval		abs_now;
	int			rv;

	if (abs_to == NULL) return;

	/* absolute to relative time */
	gettimeofday(&abs_now, NULL);
	if (TV_LESS_THAN(abs_now, *abs_to)) {
		/* ok, timeout is in the future */
		TV_MINUS(*abs_to,abs_now,relative.it_value);
	}
	else {
		/* 
		 * ouch, timeout is in the past! Let's set it
		 * to a very near future value.
		 */
		relative.it_value.tv_sec = 0;
		relative.it_value.tv_usec = ALMOST_NOW;
	}

//printf("timer_start: %d, %d\n", time(NULL), relative.it_value.tv_sec);
		
	rv = setitimer(TIMER_KIND, &relative, NULL);
	if (rv == -1) {
		/* This should never happen but I dont trust myself */
		perror("setitimer");
		exit(1);
	}

	return;
}
#endif

static void *cronometer(void *arg)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIG_TO_WAIT);

	/* 
	 * Set this thread to be cancelled only in the cancellation
	 * points (pthread_cond_wait() and sigwait()).
	 */
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	/* signal creator thread of succesful init */
	pthread_mutex_lock(&timerq.mutex);
	pthread_cond_signal(&timerq.cond);
	/* printf("\tTicker thread: inizializzaione compleata\n"); */
	pthread_mutex_unlock(&timerq.mutex);


	while (1) {
		struct timeval		abs_now;
		gettimeofday(&abs_now, NULL);
		
		pthread_mutex_lock(&timerq.mutex);
		if (timerq.first == NULL) {
			// empty timer queue => wait
//printf("(%d)timer sleep: %u\n", getpid(), (unsigned int)abs_now.tv_sec);
			pthread_cond_wait(&timerq.cond, &timerq.mutex);
//gettimeofday(&abs_now, NULL);
//printf("(%d)timer wakeup: %u\n", getpid(), (unsigned int)abs_now.tv_sec);

			pthread_mutex_unlock(&timerq.mutex);
		} else if (TV_LESS_THAN(abs_now, timerq.first->timeout)) {
			// first element has not timed out => wait
			struct timespec	wakeup_timing;
			wakeup_timing.tv_sec = timerq.first->timeout.tv_sec;
			wakeup_timing.tv_nsec = timerq.first->timeout.tv_usec * NANO_PER_MICRO;
//printf("(%d)timer sleep: %u\n", getpid(), (unsigned int)abs_now.tv_sec);
			pthread_cond_timedwait(&timerq.cond, &timerq.mutex, &wakeup_timing);
//gettimeofday(&abs_now, NULL);
//printf("(%d)timer wakeup: %u\n", getpid(), (unsigned int)abs_now.tv_sec);
			pthread_mutex_unlock(&timerq.mutex);
		} else {
			void (*hdl)(void *);
			void *hdl_arg;

			// dequeue first timed-out elements

			/*
			 * we cannot be cancelled while freeing memory and executing
			 * an handler.
			 */
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			hdl = timerq.first->handler;
			hdl_arg = timerq.first->handler_arg;
			timer_dequeue(timerq.first);
			/* 
			 * unlock the mutex before executing an handler to avoid 
			 * possible deadlock inside it.
			 */
			pthread_mutex_unlock(&timerq.mutex);
//CWLog("_handler_begin");
			hdl(hdl_arg);
//CWLog("_handler_end");

			/* reset cancel state (cancel type has not  been changed) */
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		}
	}
	return NULL;
}

/*
 * Initialize the timerq struct, spwan the ticker thread,
 * wait its initialization and return.
 */
int timer_init() {

	int rv;
	struct timespec	ts;
	struct timeval	tv;

	rv = pthread_mutex_init(&timerq.mutex, NULL);
	if (rv != 0) return 0;

	rv = pthread_cond_init(&timerq.cond, NULL);
	if (rv != 0) {
		
		pthread_mutex_destroy(&timerq.mutex);
		return 0;
	}

	timerq.first = timerq.last = NULL;
	timerq.cur_id = 0;

	pthread_mutex_lock(&timerq.mutex);

	rv = pthread_create(&timerq.ticker, NULL, cronometer, NULL);
	if (rv != 0) {
		
		pthread_mutex_unlock(&timerq.mutex);
		pthread_mutex_destroy(&timerq.mutex);
		pthread_cond_destroy(&timerq.cond);
		return 0;
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + INIT_WAIT_TIME;
	ts.tv_nsec = tv.tv_usec * NANO_PER_MICRO;

	rv = pthread_cond_timedwait(&timerq.cond, &timerq.mutex, &ts);
	if (rv != 0) {

		if (rv == ETIMEDOUT) {

			pthread_cancel(timerq.ticker);
			pthread_join(timerq.ticker, NULL);
		}
		pthread_mutex_destroy(&timerq.mutex);
		pthread_cond_destroy(&timerq.cond);
		return 0;
	}
	
 	/*
         * BUG.TRL02
         * Here we have to detach the thread in oder to avoid memory leakage.
         *
         * 16/10/2009 - Donato Capitella
         */
        pthread_detach(timerq.ticker);

	pthread_mutex_unlock(&timerq.mutex);
	
	return 1;
}

void timer_destroy() {

	tl_timer_t	*t;
	
	pthread_cancel(timerq.ticker);
	pthread_join(timerq.ticker, NULL);

	t = timerq.last;
	while (t != NULL) {

		timerq.last = t->prev;
		timer_free(t);
		t = timerq.last;
	}

	pthread_mutex_destroy(&timerq.mutex);
	pthread_cond_destroy(&timerq.cond);
	
	return;
}

static void __timer_print_nolock() {

	tl_timer_t *t;
	int i;

	for(t = timerq.first, i = 0; t != NULL; t = t->next, i++) {
#if 1
		printf("Timer %d: id=<%d>, expire=<%u,%u>\n",
			i, t->id, (unsigned int)t->timeout.tv_sec, (unsigned int)t->timeout.tv_usec);
#else	
		CWLog("Timer %d: id=<%d>, expire=<%u,%u>\n",
			i, t->id, (unsigned int)t->timeout.tv_sec, (unsigned int)t->timeout.tv_usec);
#endif
	}
}

void timer_print() 
{
	pthread_mutex_lock(&timerq.mutex);
	__timer_print_nolock();
	pthread_mutex_unlock(&timerq.mutex);
	return;
}

int timer_add(long sec, long usec, void(*hndlr)(void *), void *hndlr_arg) {

	struct timeval	new;
	tl_timer_t	*tmp = NULL;
	tl_timer_t	*app = NULL;
	int		id = 0;

	if (hndlr == NULL) return -1;

	/* ensure timeout is in the future */	
	if ((sec < 0) || (usec < 0) || ((sec == 0) && (usec ==0))) return -1;


	app = (tl_timer_t *)malloc(sizeof(tl_timer_t));
	if ( app == NULL ) {
		return -1;
	}

	/* relative to absolute time for timer */
	gettimeofday(&new, NULL);

//printf("(%d)1. new.tv_sec=%d\n", getpid(), new.tv_sec);
	/* add 10^6 microsecond units to seconds */
	new.tv_sec += sec + (new.tv_usec + usec) / MICRO_PER_SEC;
	/* keep microseconds inside allowed range */
	new.tv_usec = (new.tv_usec + usec) % MICRO_PER_SEC;
//printf("(%d)2. new.tv_sec=%d\n", getpid(), new.tv_sec);

	memcpy(&app->timeout, &new, sizeof(struct timeval));
	app->handler = hndlr;
	app->handler_arg = hndlr_arg;

	pthread_mutex_lock(&timerq.mutex);
	if (timerq.cur_id < 0) timerq.cur_id = 1; // Babylon: currently we don't use minus values and 0. althogh 0 is valid.
	id = timerq.cur_id++;
	app->id = id;

	if (timerq.first == NULL) {
		/* timer queue empty */
		timerq.first = app;
		timerq.last = app;
		app->prev = NULL;
		app->next = NULL;
		pthread_cond_signal(&timerq.cond);
//printf("(%d)timer_add condition 1\n", getpid());
//__timer_print_nolock();
		pthread_mutex_unlock(&timerq.mutex);
		return id;
	}
	
	/* there is at least a timer in the queue */
	tmp = timerq.first;
	
	/* find the first timer that expires before app */
	while (tmp != NULL) {

		if (TV_LESS_THAN(app->timeout, tmp->timeout)) 
			break;
		tmp = tmp->next;
	}

	if (tmp == NULL) {
		/* app is the longest timer */
		app->prev = timerq.last;
		app->next = NULL;
		timerq.last->next = app;
		timerq.last = app;
//printf("(%d)timer_add condition 2\n", getpid());
//__timer_print_nolock();
		pthread_mutex_unlock(&timerq.mutex);
		return id;
	}
	
	if (tmp->prev == NULL) {
		/* app is the shoprtest timer */
		app->prev = NULL;
		app->next = tmp;
		tmp->prev = app;
		timerq.first = app;
		
		pthread_cond_signal(&timerq.cond);
//printf("(%d)timer_add condition 3\n", getpid());
//__timer_print_nolock();

	}
	else {
		app->prev = tmp->prev;
		app->next = tmp;
		tmp->prev->next = app;
		tmp->prev = app;
//printf("(%d)timer_add condition 4\n", getpid());
//__timer_print_nolock();

	}
	pthread_mutex_unlock(&timerq.mutex);
	return id;
}

void timer_rem(int id, void(* free_arg)(void *)) {

	tl_timer_t	*t;

	pthread_mutex_lock(&timerq.mutex);
	t = timerq.first;

	/* look for timer id */
	while (t != NULL) {

		if (t->id == id) break;
		t = t->next;
	}	
	
	if (t == NULL) {
		/* timer id is not in queue (maybe empty) */
		pthread_mutex_unlock(&timerq.mutex);
		return;
	}
	
	/* timer id is in the queue and has not been activated */
	if (free_arg) free_arg(t->handler_arg);
	timer_dequeue(t);

	pthread_mutex_unlock(&timerq.mutex);
	return;
}
	

