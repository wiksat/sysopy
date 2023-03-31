#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int requests_counter = 0;
int catcher_state = 0;
int done=1;

void handler (int signum, siginfo_t* info, void* context) {
	int pid = info->si_pid;
	catcher_state = info->si_status;

	if (catcher_state < 1 || catcher_state > 5) {
		fprintf(stderr, "There is no such mode!\n");
		done = 1;
	}
	else {
		requests_counter++;
		done = 0;
	}

	kill(pid, SIGUSR1); //wysłanie sygnału zwrotnego bez komunikatu
}

void print_numbers() {
	for (int i = 0; i < 100; i++) {
		printf("%d\n", i+1);
	}
	done = 1;
}

void print_time() {
	time_t now = time(NULL);
  	char* timeStr = ctime(&now);
  	printf("Current time: %s", timeStr);
	done = 1;
}

void print_requests_amount() {
	printf("Requests count: %d\n", requests_counter);
	done = 1;
}

int main(int argc, char** argv) {
	setbuf(stdout, NULL);

	struct sigaction action;
	sigemptyset(&action.sa_mask);
	action.sa_sigaction = handler;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);
	printf("PID: %d\n", getpid());

	while(1) {

		if (done){
			pause();
			done=0;
		}

		if (catcher_state == 1) {
			print_numbers();
		}
		else if (catcher_state == 2) {
			print_time();
		}
		else if (catcher_state == 3) {
			print_requests_amount();
		}
		else if (catcher_state == 4) {
			print_time();
			sleep(1); //?
			done = 0;
		}
		else if (catcher_state == 5) {
			break;
		}
		fflush(NULL);
	}
	printf("Catcher has ended working successfully!\n");
	exit(0);
}