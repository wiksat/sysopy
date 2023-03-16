#ifndef LIBRARY_H
#define LIBRARY_H

typedef struct memoryStruct
{
    char** data;
    int currentSize;
    int maxSize;
} memoryStruct;

memoryStruct create(int size);

void count(memoryStruct* memory, char* file);

char* show(memoryStruct *memory, int index) ;

void removeBlock(memoryStruct *memory, int index);

void freeBlocks(memoryStruct* memory);

#endif
