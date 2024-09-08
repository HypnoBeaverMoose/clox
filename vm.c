#include "vm.h"
#include "common.h"
#include "debug.h"
#include <stdio.h>

VM vm;

static void resetStack()
{
    vm.stackTop = vm.stack;
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

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
        case OP_RETURN:
            printValue(pop());
            return INTERPRET_OK;
        }
    }
#undef READ_CONSTANT
#undef READ_BYTE
}

void initVM()
{
    resetStack();
}

void freeVM()
{
}

InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = chunk->code;
    return run();
}

void push(Value value)
{
    // TODO: see if this works. It should work
    *(vm.stackTop++) = value;
}

Value pop()
{
    return *(--vm.stackTop);
}