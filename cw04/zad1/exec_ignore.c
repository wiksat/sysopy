#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
void raise_signal() {
    printf("Raising signal in exec\n");
    raise(SIGUSR1);
}
int main(int argc, char** argv){
    raise_signal();
}