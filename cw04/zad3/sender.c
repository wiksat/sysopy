#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int sig_response;

void handler(int signum) {
    sig_response = 1;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Wrong arguments number!\n");
        exit(EXIT_FAILURE);
    }

    int state;
    int catcher_pid = atoi(argv[1]);

    for (int i = 2; i < argc; i++) {
        sig_response = 0;

        state = atoi(argv[i]);

        signal(SIGUSR1, handler);

        sigval_t sig_val = {state};
        sigqueue(catcher_pid, SIGUSR1, sig_val);
        printf("Signal sent. State: %d\n", state);

        while(!sig_response);
        printf("Catcher received signal ;)\n");
    }
}