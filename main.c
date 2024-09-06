#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_RETURN, 124);

    disassembleChunk(&chunk, "test");

    printLineInfo(&chunk);

    freeChunk(&chunk);
}