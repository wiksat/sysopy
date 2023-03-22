#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	if (argc != 2) {
		printf("Wrong arguments amount!\n");
		return 1;
	}
    int how_many=atoi(argv[1]);
    for (int i = 0; i < how_many; i++)
    {
        pid_t pid = fork();
        if (pid==0)
        {
        printf("Parent PID: %d, own PID: %d\n",getppid(), getpid());
        exit(0);
        }
        
    }
	for (int i = 0; i < how_many; i++) {
        wait(NULL);
	}
	printf("Number of forks: %d\n", how_many);
    
}