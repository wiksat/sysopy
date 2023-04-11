#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
double lower_bound = 0.0;
double upper_bound = 1.0;
double interval_width;
int n;
double current_pos = 0.0;


int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(1);
	}
    clock_t start, end;
	start = clock();

	interval_width = strtod(argv[1], NULL);
	n = atoi(argv[2]);

	if (n*interval_width > 1) {
		n = 1 / interval_width;
	}

	double n_space = 1.0 / n;

	mkfifo("/tmp/my_fifo", 0666);


    double area;
char beginA[1024];
char endA[1024];
	for (size_t i = 0; i < n; i++)
	{
        if(0 == fork()){
			snprintf(beginA, 1024, "%lf", i*n_space);
			snprintf(endA, 1024, "%lf", (i+1)*n_space);
			execl("./integral", "inetgral", beginA, endA, argv[1], NULL);
            exit(0);
        }
	}

    FILE* fifo = fopen("/tmp/my_fifo", "r");
    double area2 = 0;
    double res = 0;
    for(int i = 0;i <n;i++){
        wait(NULL);
        fread(&area2,sizeof(double),1,fifo);
        res += area2;
    }
    fclose(fifo);
    remove("/tmp/my_fifo");

	end = clock();
	double time = ((double) (end - start)) / CLOCKS_PER_SEC;


	printf(">> RESULT <<\n");
	printf("sum = %lf\nn = %d\ndx = %lf\n", res, n, interval_width);
	printf("TIME USED: %fs\n", time);
	printf("-------------\n");
	exit(0);
}