#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int licznik=0;
void obslugaUSR1(int signum){
    licznik++;
    printf("Otrzymano sygnał %d\n",licznik);
}


int main() {

    int pid = fork();
    if (pid ==0){
        signal(SIGUSR1, obslugaUSR1);
    }
    
    if (pid !=0){
        while(1)
        raise(SIGUSR1);
        sleep(100);
    }




    // raise(SIGUSR1);
return 0;
}