#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"

memoryStruct create(int size){
    char** data = calloc(size,sizeof(char*));
    struct memoryStruct newF={data,0,size};
    return newF;
}
// dla testów
// int main(){
//     printf("Hello World\n");
//     memoryStruct memory=create(12);
//     count(&memory,"text.txt");
//     printf("%s",show(&memory,0));
//     return 0;
// }
void count(memoryStruct* memory, char* file){
    if (memory->currentSize<memory->maxSize)
    {
        //przygotowanie komendy oraz wykonanie
        size_t commandSize = strlen("wc  > /tmp/") +2* strlen(file) + 1;
        char* command = malloc(commandSize);
        snprintf(command,commandSize, "wc %s > /tmp/tmpFile", file);
        system(command);
        free(command);

        //przygotowanie nazwy pliku
        char* tmpFilePath = calloc(15, sizeof(char));
        strcpy(tmpFilePath, "/tmp/tmpFile");

        //pobranie zawartości pliku
        FILE* tmp_file = fopen(tmpFilePath, "r");
        if (tmp_file == NULL) {
            printf("Cannot open tmp file!\n");
        }
        //szukanie długości pliku
        fseek(tmp_file, 0, SEEK_END);
        long tmp_file_size = ftell(tmp_file);
        fseek(tmp_file, 0, SEEK_SET);

        //zapis zawartości do struktury
        if (tmp_file_size>2)
        {
            memory->data[memory->currentSize] = calloc(tmp_file_size,sizeof(char));
            fgets(memory->data[memory->currentSize], tmp_file_size, tmp_file);

            memory->currentSize++;
        }

    //porządkowanie
    fclose(tmp_file);
    remove(tmpFilePath);
    free(tmpFilePath);
    printf("Counted!\n");
    }
    else{
        printf("Cannot count - no space in memory!\n");
    }

}


char* show(memoryStruct *memory, int index) {
    if(index<0 || index >= memory->currentSize){
        printf("Error: Index out of range!!!");
        return NULL;
    }
    return memory->data[index];
}

void removeBlock(memoryStruct *memory, int index) {
    if(index<0 || index >= memory->currentSize){
        printf("Error: Index out of range!!!");
        return;
    }
    free(memory->data[index]);
    for(int i = index; i<memory->currentSize-1; i++){
        memory->data[i] = memory->data[i+1];
    }
    
    memory->data[memory->currentSize-1]=NULL;
    memory->currentSize--;
    printf("Index %d has been deleted!\n", index);
}

void freeBlocks(memoryStruct* memory){
    for(int i=0; i<memory->currentSize; i++){
        free(memory->data[i]);
    }
    memory->currentSize = 0;
    free(memory->data);
    printf("All has been free!\n");
}