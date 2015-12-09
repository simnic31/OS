/*
 * test prog for lab 4
 */

#include <types.h>
#include <lib.h>
#include <test.h>

int bootytest (int nargs, char *argv[])
{
        for (int i = 0; i<nargs; i++)
                kprintf("\n%s", argv[i]);

        (void)nargs;
        (void)argv;

        //kprintf("You typed: %s\n", argv[1]);

        //kprintf("It got da booty. \n");

        return 0;
}






