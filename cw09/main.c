#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdbool.h>

#define NUM_ELVES 10
#define NUM_REINDEERS 9
#define MAX_ELVES_WAITING 3
#define MAX_REINDEERS_WAITING 9

int elf_q[MAX_ELVES_WAITING];
int last_elf = -1;
int no_reindeers = 0;
int no_elves = 0;
int presents = 0;
bool running = 1;

char time_msg[16];

void set_time() {
	struct tm time_struct;
	time_t my_time = time(NULL);
	time_struct = *localtime(&my_time);
	sprintf(time_msg, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
}
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_work_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeers_cond = PTHREAD_COND_INITIALIZER;

typedef struct {
	int id;
} IdStruct;


void* elf(void* arg) {
	IdStruct* indexOf = (IdStruct*)arg;
    int id = indexOf->id;
    free(indexOf);

    while (1) {
		sleep(2 + (rand() % 4));
        pthread_mutex_lock(&mutex);
        no_elves++;

        set_time();
        if (no_elves < MAX_ELVES_WAITING) {
            last_elf += 1;
            elf_q[last_elf] = id;
            printf("%s Elf: czeka %d elfów na Mikołaja, ID: %d\n", time_msg, last_elf + 1, id);
            pthread_cond_wait(&elves_cond, &mutex);
        } else if (no_elves == MAX_ELVES_WAITING) {
        	printf("%s Elf: czeka %d elfów na Mikołaja, ID: %d\n", time_msg, last_elf + 1, id);
            printf("%s Elf: wybudzam Mikołaja, ID: %d\n", time_msg, id);
            pthread_cond_signal(&santa_work_cond);
        } else {
            printf("%s Elf: samodzielnie rozwiązuje problem, ID: %d\n", time_msg, id);
        }

        pthread_mutex_unlock(&mutex);
    }
}

void* reindeer(void* arg) {
    IdStruct* indexOf = (IdStruct*)arg;
    int id = indexOf->id;
    free(indexOf);

    while (1) {
     	sleep(5 + (rand() % 6));

        pthread_mutex_lock(&mutex);

        no_reindeers += 1;
        set_time();
        printf("%s Renifer: czeka %d reniferów na Mikołaja\n", time_msg, no_reindeers);
        if (no_reindeers == MAX_REINDEERS_WAITING) {
            printf("%s Renifer: wybudzam Mikołaja, ID: %d\n", time_msg, id);
            pthread_cond_signal(&santa_work_cond);
        }

        pthread_mutex_unlock(&mutex);
    }
}

void* santa(void* args) {
     while (1) {
        pthread_mutex_lock(&mutex);

        if (no_elves >= MAX_ELVES_WAITING) {
        	set_time();
            printf("%s Mikołaj: rozwiązuje problemy elfów %d %d %d\n", time_msg, elf_q[0], elf_q[1], elf_q[2]);
            sleep(1 + (rand() % 2));
            last_elf = -1;
            no_elves = 0;
            pthread_cond_broadcast(&elves_cond);
        }

        if (no_reindeers == MAX_REINDEERS_WAITING) {
        	set_time();
            printf("%s Mikołaj: dostarczam zabawki\n", time_msg);
            presents++;
            sleep(2 + (rand() % 3));
            no_reindeers = 0;
            pthread_cond_broadcast(&reindeers_cond);
        }

        set_time();
        printf("%s Mikołaj: zasypiam\n", time_msg);

        if (presents == 3) {
        	running = 0;
        }

        pthread_cond_wait(&santa_work_cond, &mutex);

        set_time();
        printf("%s Mikołaj: budzę się\n", time_msg);

        pthread_mutex_unlock(&mutex);
    }
}

int main() {
	srand(time(NULL));
	pthread_t santa_thread, elves_thread[NUM_ELVES], reindeers_thread[NUM_REINDEERS];

	pthread_create(&santa_thread, NULL, santa, NULL);

	for (int i = 0; i < NUM_REINDEERS; i++) {
		IdStruct* indexOf = malloc(sizeof(IdStruct));
		indexOf->id = i + 1;
	    pthread_create(&reindeers_thread[i], NULL, reindeer, indexOf);
	}

    for (int i = 0; i < NUM_ELVES; i++) {
		IdStruct* indexOf = malloc(sizeof(IdStruct));
		indexOf->id = i + 1;
	    pthread_create(&elves_thread[i], NULL, elf, indexOf);
	}
    while (running)
	;

	pthread_cancel(santa_thread);

	for (int i = 0; i < NUM_ELVES; i++) {
		pthread_cancel(elves_thread[i]);
	}

	for (int i = 0; i < NUM_REINDEERS; i++) {
		pthread_cancel(reindeers_thread[i]);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&santa_work_cond);
	pthread_cond_destroy(&elves_cond);
	pthread_cond_destroy(&reindeers_cond);


	exit(0);
}