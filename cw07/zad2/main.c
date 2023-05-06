#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

char* chair_sem_name = "/chair";
char* waiting_sem_name = "/waiting";
char* memory_sem_name = "/memory";
char* memory_name = "/times";

char time_msg[16];
int M, N, P, F;
int *service_times;
int shmColl;
int semColl;
int run = 1;

int* times;

void set_time() {
	struct tm time_struct;
	time_t my_time = time(NULL);
	time_struct = *localtime(&my_time);
	sprintf(time_msg, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
}


void generate_times_of_services(int times_of_services[], int F) {
	printf("Times of services:\n");
    for (int i=0; i<F; i++) {
		times_of_services[i] =1000 + random() % 4000;
		printf("id: %d, time: %fs\n", i+1, times_of_services[i]/1000.0);
    }
		printf("\n");
}

void regular_signal_handler(int signum) {
	run = 0;
}
void hairdresser_sig_handler(int signum) {
	run = 0;
	exit(0);
}

void client() {
	int last_client = 0;
	sem_t *wait_sem = sem_open(waiting_sem_name, 0);
	sem_t *mem_sem = sem_open(memory_sem_name, 0);
	shmColl = shm_open(memory_name, O_RDWR, 0666);
	while(run) {
		sleep(rand() % 4);
		int service_number = rand()%F;
		int time = service_times[service_number];
		set_time();
		printf("%s New client has arrived. Service number: %d, time: %fs\n", time_msg, service_number+1, time/1000.0);

		sem_wait(mem_sem);
		times = mmap(NULL, (P+1)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmColl, 0);
		if (times[last_client+1] == -1) {
			times[last_client+1] = time;
			last_client = (last_client + 1) % P;
			sem_post(wait_sem);

			set_time();
			printf("%s Client take place in the queue\n", time_msg);
		}
		else {
			set_time();
			printf("%s No empty place in queue, leaving..\n", time_msg);;
		}

		munmap(times, sizeof(int)*(P+1));
		sem_post(mem_sem);

	}
}
void hairdresser(int id) {
	sem_t *wait_sem = sem_open(waiting_sem_name, 0);
	sem_t *chair_sem = sem_open(chair_sem_name, 0);
	sem_t *mem_sem = sem_open(memory_sem_name, 0);
	shmColl = shm_open(memory_name, O_RDWR, 0666);
	set_time();
	printf("%s Hairdresser [%d] started working...\n", time_msg, id);
	while(run) {
		sem_wait(chair_sem);
		sem_wait(wait_sem);

		set_time();
		printf("%s Hairdresser [%d] has reserved chair, taking client..\n", time_msg, id);

		sem_wait(mem_sem);
		times = mmap(NULL, (P+1)*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmColl, 0);

		int time = times[times[0]+1];
		times[times[0]+1] = -1;
		times[0] = (times[0] + 1) % P;

		munmap(times,sizeof(int)*(P+1));
		sem_post(mem_sem);

		usleep(time);
		set_time();
		printf("%s Hairdresser [%d] finished\n", time_msg, id);
		sem_post(chair_sem);
	}
}


int main(int argc, char** argv) {
	if (argc < 5) {
		fprintf(stderr, "Error! Correct input pattern [M N P F]\n");
		exit(1);
	}

	M = atoi(argv[1]);
	N = atoi(argv[2]);
	P = atoi(argv[3]);
	F = atoi(argv[4]);
	if (M < 1 || N < 1 || P < 1 || F < 1) {
		fprintf(stderr, "All arguments have to be greater than 0\n");
		exit(1);
	}
	if (M < N) {
		fprintf(stderr, "M must be greater than N\n");
		exit(1);
	}
	signal(SIGINT, regular_signal_handler);
	setbuf(stdout, NULL);
	srand(time(NULL));

	service_times = malloc(F * sizeof(int));
	generate_times_of_services(service_times, F);

	sem_t *chair_sem = sem_open(chair_sem_name, O_CREAT, 0666, 0);
	sem_t *mem_sem = sem_open(memory_sem_name, O_CREAT, 0666, 0);
	sem_open(waiting_sem_name, O_CREAT, 0666, 0);
	for (int i = 0; i < N; i++) {
		sem_post(chair_sem);
	}
	sem_post(mem_sem);
	sem_close(chair_sem);

	int shmColl = shm_open(memory_name, O_CREAT | O_RDWR, 0666);
	ftruncate(shmColl, (P+1)*sizeof(int));
	times = (int*) mmap(NULL, sizeof(int)*(P+1), PROT_READ | PROT_WRITE, MAP_SHARED, shmColl, 0);
	memset(times, -1, (P+1)*sizeof(int));
	times[0] = 0;
	munmap(times, (P+1)*sizeof(int));

	for (int i = 0; i < M; i++) {
		if (fork() == 0) {
			signal(SIGINT, hairdresser_sig_handler);
			hairdresser(i);
			exit(0);
		}
	}
	client();
	while(wait(NULL) > 0);
	free(service_times);
	printf("Zakończono pracę alonu\n");
	exit(0);
}