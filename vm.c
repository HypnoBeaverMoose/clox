#include "vm.h"
#include "memory.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "object.h"
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
VM vm;

static bool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate()
{
    Value valA = pop();
    Value valB = pop();

    ObjString* b = AS_STRING(valA);
    ObjString* a = AS_STRING(valB);

    int length = a->length + b->length;
    char *result = ALLOCATE(char, length + 1);

    memcpy(result, GET_CSTRING(a), a->length);
    memcpy(result + a->length, GET_CSTRING(b), b->length);

    result[length] = '\0';

    ObjString *string = takeString(result, length);

    push(OBJ_VAL(string));
}

static void resetStack()
{
    vm.stackTop = vm.stack;
}

static void runtimeError(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputc('\n', stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = getLine(vm.chunk, instruction);
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, OP)                         \
    do                                                   \
    {                                                    \
        if (!(IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))) \
        {                                                \
            runtimeError("Operands must be numbers.");   \
            return INTERPRET_RUNTIME_ERROR;              \
        }                                                \
        double b = AS_NUMBER(pop());                     \
        double a = AS_NUMBER(pop());                     \
        push(valueType(a OP b));                         \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
                               (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_NIL:
            push(NIL_VAL);
            break;
        case OP_TRUE:
            push(BOOL_VAL(true));
            break;
        case OP_FALSE:
            push(BOOL_VAL(false));
            break;
        case OP_EQUAL:
            push(BOOL_VAL(valuesEqual(pop(), pop())));
            break;
        case OP_NOT:
            push(BOOL_VAL(isFalsey(pop())));
            break;
        case OP_ADD:
            if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
            {
                concatenate();
            }
            else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))
            {
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                push(NUMBER_VAL(a + b));
            }
            else
            {
                runtimeError("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        case OP_SUBTRACT:
            BINARY_OP(NUMBER_VAL, -);
            break;
        case OP_MULTIPLY:
            BINARY_OP(NUMBER_VAL, *);
            break;
        case OP_DIVIDE:
            BINARY_OP(NUMBER_VAL, /);
            break;
        case OP_GREATER:
            BINARY_OP(BOOL_VAL, >);
            break;
        case OP_LESS:
            BINARY_OP(BOOL_VAL, <);
            break;
        case OP_NEGATE:
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_RETURN:
            printValue(pop());
            return INTERPRET_OK;
        }
    }
#undef READ_CONSTANT
#undef READ_BYTE
#undef BINARY_OP
}

void initVM()
{
    resetStack();
    vm.objects = NULL;
}

void freeVM()
{
}

InterpretResult interpret(const char *source)
{
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk))
    {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = chunk.code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}

void push(Value value)
{
    *(vm.stackTop++) = value;
}

Value pop()
{
    return *(--vm.stackTop);
}

Value peek(int distance)
{
    return vm.stackTop[-1 - distance];
}