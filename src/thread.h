/*
 * =====================================================================================
 *
 *       Filename:  thread.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/13 00:26:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#pragma once
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

typedef void * ( *thread_func_t)(void *);
typedef struct {
	pthread_t tid;
	pthread_mutex_t mutex;
	pthread_cond_t m_ResumeCond;
	pthread_attr_t attr;
	    char suspended;
} thread_t;

void thread_start(thread_t *thread,thread_func_t thread_func ,void *thread_prm ) ;
void thread_manage_suspend(thread_t *thread) ;
void thread_resume(thread_t *thread);
void thread_signal_handle(int sig);
void thread_suspend(thread_t * thread);




