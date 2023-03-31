#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
void raise_signal() {
    printf("Raising signal in exec\n");
    raise(SIGUSR1);
}
int main(int argc, char ** argv){
    raise_signal();
    sigset_t waiting_mask;
    sigemptyset(&waiting_mask);
    sigpending(&waiting_mask);
    printf("EXEC Signal blocked: %d\n",sigismember(&waiting_mask,SIGUSR1));
}