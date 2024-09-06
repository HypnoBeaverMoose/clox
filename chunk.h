#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"
#include "common.h"

typedef struct
{
    size_t line;
    size_t count;
} Line;

typedef struct
{
    size_t capacity;
    size_t count;
    Line *data;
} LineInfo;

typedef struct
{
    size_t capacity;
    size_t count;
    uint8_t *code;
    ValueArray constants;
    LineInfo lines;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

int getLine(Chunk* chunk, int offset);

#endif