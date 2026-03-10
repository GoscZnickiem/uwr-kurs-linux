#ifndef BUILTINS
#define BUILTINS

#include <stddef.h>

typedef int (*builtin_func_t)(char**);

extern const char* const builtin_str[];
extern const builtin_func_t builtin_func[];

size_t builtin_func_count();

#endif // !BUILTINS
