#include "common.h"
#include "chunk.h"
#include "memory.h"
#include <stdio.h>

void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;

    chunk->lines.data = NULL;
    chunk->lines.capacity = 0;
    chunk->lines.count = 0;

    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                                 oldCapacity, chunk->capacity);
    }

    LineInfo lines = chunk->lines;
    if (lines.count > 0 && lines.data[lines.count].line == line)
    {
        lines.data[lines.count].count++;
    }
    else
    {
        int oldCapacity = lines.capacity;
        lines.capacity = GROW_CAPACITY(oldCapacity);
        lines.data = GROW_ARRAY(Line, lines.data,
                                oldCapacity, lines.capacity);

        lines.data[lines.count].line = line;
        lines.data[lines.count].count = 1;
        lines.count++;

        printf("line info:\n");

        for (size_t i; i < lines.count; i++)
        {
            printf("entry: %zu %zu\n", lines.data[i].line, lines.data[i].count);
        }
    }
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(int, chunk->lines.data, chunk->capacity);
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    initChunk(chunk);
    freeValueArray(&chunk->constants);
}

int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk *chunk, int offset)
{
    LineInfo lines = chunk->lines;
    int acc = 0;
    int i = 0;

    for (; i < lines.count; i++)
    {
        acc += lines.data[i].count;
        if (acc > offset)
        {
            break;
        }
    }
    printf("\nindex: %i\n", (int)lines.count);

    return (int)lines.data[i].line;
}