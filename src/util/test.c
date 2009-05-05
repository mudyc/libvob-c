// (c): Matti J. Katila

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static void the_assert(char *condition, char *file, 
		       char *func, int line, bool do_fart)
{
	if (do_fart) {
		printf("%s failed at: %s:%s:%d", condition, file, func, line);
		exit(1);
	}
}

void assert_true_(char *file, char *func, int line, bool val)
{
	the_assert("Assert_true", file, func, line, !val);
}

void assert_false_(char *file, char *func, int line, bool val)
{
	the_assert("Assert_false", file, func, line, val);
}


