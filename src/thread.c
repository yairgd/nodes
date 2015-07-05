/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/02/13 20:02:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "thread.h"
// http://stackoverflow.com/questions/4544234/calling-pthread-cond-signal-without-locking-mutex

void thread_start(thread_t *thread,thread_func_t thread_func,void *thread_prm) {
	thread->suspended = 1;
	pthread_attr_init(&thread->attr);
	pthread_cond_init(&thread->m_ResumeCond, NULL);
	pthread_mutex_init(&thread->mutex, NULL);
	pthread_create(&thread->tid, NULL, thread_func, thread_prm );
}


void thread_manage_suspend(thread_t * thread) {
	pthread_mutex_lock(&thread->mutex);
	if (thread->suspended) {
		pthread_cond_wait(&thread->m_ResumeCond, &thread->mutex);
	}
	pthread_mutex_unlock(&thread->mutex);

}

void thread_resume(thread_t * thread){

	/* The shared state 'suspended' must be updated with the mutex held. */
	pthread_mutex_lock(&thread->mutex);
//	while (	thread->suspended == 0);
	if (thread->suspended) {
		thread->suspended = 0;	
		pthread_cond_signal(&thread->m_ResumeCond);
	}
	pthread_mutex_unlock(&thread->mutex);
	
	
}
void thread_suspend(thread_t * thread)
{
pthread_mutex_lock(&thread->mutex);
		thread->suspended = 1;
	pthread_mutex_unlock(&thread->mutex);
		
}
