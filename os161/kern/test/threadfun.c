
/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  12

static struct semaphore *tsem = NULL;
static struct lock* tlock = NULL;


static
void
init_items(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadfun: sem_create failed\n");
		}
	}

	if (tlock == NULL) {
		tlock = lock_create("tlock");
		if (tlock == NULL){
			panic("threadfun: lock_create failed\n");
		}
	}

	
		
}

static int counter;


int threadfun(int type)
{	
	(void)type;
	init_items();
	counter = 0;
	char name[16];
	int i, result;

	for (i=0; i<12; i++) {
		if(type == 1){
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
				     unsafeCounter, NULL, i);
	}

	if(type==2){
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
				lockCounter, NULL, i);
	} 

	if(type==3){
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
				spinLockCounter, NULL, i);
	}

		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}


	for (i=0; i<12; i++) {
		P(tsem);
	}
//	cleanitems();
	kprintf("%d", counter);
	counter = counter -9;
	counter = counter + '0';

	return 0;
}


void
unsafeCounter(void *junk, unsigned long num)
{
	int ch = '0' + num;
	for(int i=0; i < 10000; i++){
	counter++;
	(void)junk;
	(void) ch;
	}
	V(tsem);

}


void
lockCounter(void *junk, unsigned long num)
{
	int ch = '0' + num;
	//(void)ch;
	//(void)junk;

	for (int i=0; i<10000; i++){
		//P(tsem2);
		lock_acquire(tlock);
		counter++;
		lock_release(tlock);
		(void)junk;
		(void)ch;
		//V(tsem2);
		
	}
	V(tsem);
	thread_exit();
}

void
spinLockCounter(void *junk, unsigned long num)
{
	int ch = '0' + num;

	for (int i=0; i<10000; i++){
		spinlock_acquire(&tlock->lk_spinlock);
		counter++;
		spinlock_release(&tlock->lk_spinlock);
		(void)ch;
		(void)junk;
	}
	V(tsem);
}



int test1(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	init_items();
	kprintf("Starting unsafe thread test...\n");
	threadfun(1);
	kprintf("\nTest finished.\n");
//	sem_destroy(tsem);

	return 0;
}

int test2(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	 init_items();
	 kprintf("Starting locked thread test...\n");
	 threadfun(2);
	 kprintf("\nTest finished.\n");
//	 lock_destroy(tlock);

	 return 0;
}

int test3(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	init_items();
	kprintf("Starting spinlock thread test...\n");
	threadfun(3);
	kprintf("\nTest finished.\n");
	//lock_destroy(tlock);
	//lock_destroy(&tlock->lk_spinlock);

	return 0;
}

