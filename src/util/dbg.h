
#ifndef H_UTIL_DBG
#define H_UTIL_DBG

#include <stdio.h>
#include <sys/timeb.h>

#define UNIMPL printf("Unimplemented: %s -> %s(...)\n", __FILE__, __FUNCTION__);

#define DBG(...) { struct timeb t; ftime(&t); printf("dbg %d:%d.%d %s(%d) ", t.time / 60 % 60, t.time % 60, t.millitm, __FUNCTION__, __LINE__); printf(__VA_ARGS__); printf("\n"); }


#endif
