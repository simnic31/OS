
/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  9

static struct semaphore *tsem = NULL;
static struct semaphore *tsem2 = NULL;

static
void
init_sem(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadfun: sem_create failed\n");
		}
	}

	if (tsem2==NULL) {
		tsem2 = sem_create("tsem2", 1);
		if (tsem2 == NULL) {
			panic("threadfun: sem_create failed\n");
		}
	}
		
}

static int counter;


int threadfun(int type)
{	
	(void)type;
	init_sem();
	counter = 0;
	char name[16];
	int i, result;

	for (i=0; i<12; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
				     threadUse2, NULL, i);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}


	for (i=0; i<12; i++) {
		P(tsem);
	}
	
	kprintf("%d", counter);
	counter = counter -119999;
	counter = counter + '0';

	return 0;
}

void
threadUse(void *junk, unsigned long num)
{
	int ch = '0' + num;
	for(int i; i < 10000; i++){
	counter++;
	(void)junk;
	(void) ch;
	}
	V(tsem);

}


void
threadUse2(void *junk, unsigned long num)
{
	int ch = '0' + num;
	//(void)ch;
	//(void)junk;

	for (int i; i<10000; i++){
		P(tsem2);
		counter++;
		(void)junk;
		(void)ch;
		V(tsem2);
	}
	V(tsem);
}


int test1(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	init_sem();
	kprintf("Starting thread test...\n");
	threadfun(1);
	kprintf("\nTest finished.\n");

	return 0;
}

int test2(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	 init_sem();
	 kprintf("Starting thread test...\n");
	 threadfun(2);
	 kprintf("\nTest finished.\n");

	 return 0;
}

int test3(int nargs, char **args)
{
	(void) nargs;
	(void) args;

	init_sem();
	kprintf("Starting thread test...\n");
	threadfun(3);
	kprintf("\nTest finished.\n");

	return 0;
}

