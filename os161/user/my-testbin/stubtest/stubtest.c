/*
 * An example program.
 */
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>

int
main()
{
	int status;

	printf("Beginning stubtest\n");
	
	pid_t id = getpid();
	printf("pid is: %d\n", id);
	pid_t wid = waitpid(getpid(), &status, 0);
	printf("waitpid: %d\n", wid);

   int pid = fork();
	printf("fork pid: %d\n", pid);


	_exit(0);
	//return 0; /* avoid compiler warnings */
}
