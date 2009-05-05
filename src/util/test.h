
#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include <stdbool.h>

void assert_true_(char *file, char *func, int line, bool val);
#define assert_true(EXP) assert_true_(__FILE__, (char*)__FUNCTION__, __LINE__, (EXP))

void assert_false_(char *file, char *func, int line, bool val);
#define assert_false(EXP) assert_false_(__FILE__, (char*)__FUNCTION__, __LINE__, (EXP))


#endif
