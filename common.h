#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEBUG_TRACE_EXECUTION

typedef enum
{
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN
} OpCode;

#endif