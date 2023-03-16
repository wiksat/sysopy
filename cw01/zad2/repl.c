#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>

#ifndef DLL
#include "library.h"
#endif

#ifdef DLL
#include <dlfcn.h>

typedef struct memoryStruct
{
    char** data;
    int currentSize;
    int maxSize;
} memoryStruct;
#endif

int main(){
    #ifdef DLL
	void *handle = dlopen("../zad1/liblibrary.so", RTLD_LAZY);
	if (handle == NULL) {
		printf("Cannot open library!\n");
		return -1;
	}
	struct memoryStruct (*create)(int) = dlsym(handle, "create");
	void (*count)(struct memoryStruct*, char*) = dlsym(handle, "count");
	char* (*show)(struct memoryStruct*, int) = dlsym(handle, "show");
	void (*removeBlock)(struct memoryStruct*, int) = dlsym(handle, "removeBlock");
	void (*freeBlocks)(struct memoryStruct*) = dlsym(handle, "freeBlocks");
	#endif

    int is_initiated = 0;
	char *cmd;
    char *arg;
    memoryStruct memory;
    char line[100];

    struct timespec startTime, endTime;
	clockid_t clk_id = CLOCK_REALTIME;
	double realTime, userTime, systemTime;

    while (1)
    {
		printf("> ");
		if (fgets(line, sizeof(line), stdin) == NULL) {
			continue;
		}
        if (strcmp(line, "\n") == 0) {
            continue;
		}
        cmd = strtok(line, " \n");
		arg = strtok(NULL, " \n"); //null kontynuje prawe z poprzednim słowem

        if (strcmp(cmd, "init") == 0) {
			if (is_initiated) {
				printf("The array has already been initialized.\n");
				continue;
			}

			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &startTime);
			memory = create(atoi(arg)); //konwertuje stringa na inta
			clock_gettime(clk_id, &endTime);
            printf("The array has been initialized with %s indexes.\n",arg);
			is_initiated= 1;
		}
        else if (strcmp(cmd, "count") == 0 && is_initiated) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &startTime);
			count(&memory, arg);
			clock_gettime(clk_id, &endTime);
		}
        else if (strcmp(cmd, "show") == 0 && is_initiated) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &startTime);
            if (show(&memory, atoi(arg))!=NULL)
            {
			    printf("At %s index: %s\n", arg, show(&memory, atoi(arg)));
            }else{
                printf("\n");
            }
            
			clock_gettime(clk_id, &endTime);
		}
		else if (strcmp(cmd, "delete") == 0 && is_initiated) {
			if (arg == NULL) {
				printf("Not enougth arguments.\n");
				continue;
			}

			clock_gettime(clk_id, &startTime);
			removeBlock(&memory, atoi(arg));
			clock_gettime(clk_id, &endTime);
		}
        else if (strcmp(cmd, "exit") == 0) {
            printf("Program has ended successfully.\n");
            break;
		}
        else if (strcmp(cmd, "destroy") == 0 && is_initiated) {
			clock_gettime(clk_id, &startTime);
			freeBlocks(&memory);
			clock_gettime(clk_id, &endTime);

			is_initiated = 0;
		}
		else {
            if (!is_initiated)
            {
                printf("It is required to initialize array first!\n");
                continue;
            }
            
			printf("Unknown command!\n");
			continue;
		}

        realTime = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0; //czas w sekundach plus w nanosekundach podzielony przez 10^9
		struct tms buffer;
		times(&buffer); //pobiera informacje o czasie dla bieżącego procesu
		userTime = (double) buffer.tms_utime / CLOCKS_PER_SEC; //dzielimy przez CLOCKS_PER_SEC aby przekonwertować czas na sekundy
		systemTime = (double) buffer.tms_stime / CLOCKS_PER_SEC;

        printf("REAL TIME: %f s\n", realTime);
		printf("USER TIME: %f s\n", userTime);
		printf("SYSTEM TIME: %f s\n", systemTime); 
    }
#ifdef DLL
dlclose(handle);    //zamknięcie biblioteki dynamicznej
#endif
}