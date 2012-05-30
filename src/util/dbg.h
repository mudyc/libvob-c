
#ifndef H_UTIL_DBG
#define H_UTIL_DBG

#include <stdio.h>

#define UNIMPL printf("Unimplemented: %s -> %s(...)\n", __FILE__, __FUNCTION__);

#define DBG(...) { printf("dbg %s(%d) ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); printf("\n"); }


#endif
