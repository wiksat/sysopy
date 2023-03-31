#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int call_id;
int call_depth;


void info_handler(int signum, siginfo_t* info, void* context) {
    printf("Signal number: %d\n", info->si_signo);
    printf("PID: %d\n", info->si_pid);
    printf("UID: %d\n", info->si_uid);
    printf("Addr: %p\n", info->si_addr);
    printf("POSIX timer ID: %d\n",  info->si_timerid);
    printf("Exit value / signal: %d\n\n",info->si_status);
}

void depth_handler(int sinnum, siginfo_t* info, void* context) {
    printf("Start - call_id: %d, call_depth: %d\n", call_id, call_depth);
    call_id++;
    call_depth++;
    if (call_id < 5) {
        raise(SIGUSR1);
    }
    call_depth--;
    printf("End - call_id: %d, call_depth: %d\n", call_id-1, call_depth);
}

int main(int argc, char** argv) {
    if (argc != 1) {
        fprintf(stderr, "Wrong arguments amount!\n");
        exit(1);
    }

    struct sigaction action;

    // siginfo
    printf(">> SIGINFO flag:\n");
    printf("PARENT:\n");

    sigemptyset(&action.sa_mask);
    action.sa_sigaction = info_handler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);
    call_id = 0;
    call_depth = 0;

    raise(SIGUSR1);
    printf("CHILD:\n");
    if (fork() == 0) {
        raise(SIGUSR1);
        exit(0);
    }
    else {
        wait(NULL);
    }
    printf("\n");


    // restart
    printf(">> RESTART flag:\n");
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = depth_handler;
    action.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &action, NULL);
    call_id = 0;
    call_depth = 0;

    raise(SIGUSR1);
    printf("\n");

    // nodefer
    printf(">> NODEFER flag:\n");
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = depth_handler;
    action.sa_flags = SA_NODEFER;
    sigaction(SIGUSR1, &action, NULL);
    call_id = 0;
    call_depth = 0;

    raise(SIGUSR1);

}