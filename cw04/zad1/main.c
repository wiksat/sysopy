#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void raise_signal() {
    printf("(PID: %d) Raising signal\n", getpid());
    raise(SIGUSR1);
}
void raise_signalExec() {
    printf("Raising signal before exec\n");
    raise(SIGUSR1);
}
void create_child(){
    if(0 == fork()){
        raise_signal();
    }
}

void signal_handler(int signum){
     printf("(PID: %d) Received signal: %d\n", getpid(), signum);
}

int main(int argc, char** argv){
    if (argc != 2) {
        fprintf(stderr, "Wrong arguments number!\n");
        exit(EXIT_FAILURE);
    }
setbuf(stdout, NULL);

    if (strcmp(argv[1],"ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
        raise_signal();
        create_child();
    }
    else if (strcmp(argv[1],"handler") == 0){
        signal(SIGUSR1, signal_handler);
        raise_signal();
        create_child();
    }
    else if (strcmp(argv[1],"mask") == 0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise_signal();
        create_child();
        
        sigset_t waiting_mask;
        sigemptyset(&waiting_mask);
        sigpending(&waiting_mask);
        printf("(PID: %d) Signal blocked: %d\n",getpid(),sigismember(&waiting_mask,SIGUSR1));
    }
    else if (strcmp(argv[1],"pending") == 0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        raise_signal();
        fork();
        sigset_t pending_set;
        sigemptyset(&pending_set);
        sigpending(&pending_set);
        if (sigismember(&pending_set, SIGUSR1)) {
            printf("(PID: %d) Signal is pending\n", getpid());
        }
        else {
            printf("(PID: %d) Signal is not pending\n", getpid());
        }
    }

    else if(strcmp(argv[1],"execIgnore")==0){

        signal(SIGUSR1, SIG_IGN);
        raise_signalExec();
        if (0 == fork()){
            execl("./exec_ignore","./exec_ignore",NULL);
        }
    }
    else if(strcmp(argv[1],"execMask")==0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);

        raise_signalExec();
        
        execl("./exec_mask","./exec_mask",NULL);
        

    }
    else if(strcmp(argv[1],"execPending")==0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);

        raise_signalExec();

        sigset_t pending_set;
        sigemptyset(&pending_set);
        sigpending(&pending_set);
        if (sigismember(&pending_set, SIGUSR1)) {
            printf("Signal is pending before exec\n");
        }
        else {
            printf("Signal is not pending before exec\n");
        }
        execl("./exec_pending","./exec_pending",NULL);
        }
    else{
        printf("Wrong argument\n");
    }

}