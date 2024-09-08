#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main()
{
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 1.2, 123);

    writeChunk(&chunk, OP_RETURN, 124);

    //disassembleChunk(&chunk, "test");

    interpret(&chunk);
    freeChunk(&chunk);

    freeVM();
}