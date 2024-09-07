#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 500; i++)
    {
        writeConstant(&chunk, i, 1);
        //int constant = addConstant(&chunk, 1.2);
    }

    writeChunk(&chunk, OP_RETURN, 124);

    disassembleChunk(&chunk, "test");


    freeChunk(&chunk);
}