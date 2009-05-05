
#ifndef H_UTIL_DBG
#define H_UTIL_DBG

#include <stdio.h>

#define UNIMPL printf("Unimplemented: %s -> %s(...)\n", __FILE__, __FUNCTION__);

#define DBG(str) printf("Dbg: %s -> %s: %s\n", __FILE__, __FUNCTION__, str)


#endif
