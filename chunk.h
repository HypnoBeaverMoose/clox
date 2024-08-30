#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"
#include "common.h"

typedef struct
{
    size_t capacity;
    size_t count;
    uint8_t *code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

#endif