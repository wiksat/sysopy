#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char* argv[]){
	if (argc != 2) {
		printf("Wrong arguments amount!\n");
		return 1;
	}
    char* str = argv[0];
    char *ptr;

    ptr = strchr(str, '/');
    if (ptr != NULL) {
        printf("%s: ",ptr+1);
    }
    char* path=argv[1];

    fflush(stdout);

	if (execl("/bin/ls", "ls", path, NULL) == -1) { //ostatni argument musi mieć wartość NULL
        printf("Wrror while execl");
		return 1;
	}

    return 0;



    
}