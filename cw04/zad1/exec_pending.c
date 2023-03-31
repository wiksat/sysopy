#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
int main(int argc, char ** argv){
    sigset_t pending_set;
    sigemptyset(&pending_set);
    sigpending(&pending_set);
    if (sigismember(&pending_set, SIGUSR1)) {
        printf("Signal is pending in exec\n");
    }
    else {
        printf("Signal is not pending in exec\n");
    }
}