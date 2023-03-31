#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void obslugaUSR1(int signum){
printf("Ja cie ale sygnal\n");
exit(0);
}


int main() {

signal(SIGUSR1, obslugaUSR1);
raise(SIGUSR1);
while(1)
sleep(100);


// raise(SIGUSR1);
return 0;
}