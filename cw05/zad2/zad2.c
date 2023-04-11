#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

double lower_bound = 0.0;
double upper_bound = 1.0;
double interval_width;
int n;
double current_pos = 0.0;


double fun(float x) {
	return 4/(x*x+1);
}

double calculate_integral(double a, double b, double width) {

	while (current_pos < a) {
		current_pos += width;
	}

	double sum = 0.0;
	while (current_pos < b) {
		sum += fun(current_pos) * width;
		current_pos += width;
	}

	return sum;
}

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
    int fd[n][2];
    double area;

	for (size_t i = 0; i < n; i++)
	{
		pipe(fd[i]);
        if(0 == fork()){
            close(fd[i][0]);
            area = calculate_integral(i*n_space, (i+1)*n_space, interval_width);
            write(fd[i][1],&area,sizeof(double));
            exit(0);
        }
        close(fd[i][1]);
	}

	while(wait(NULL) > 0);
    double area2 = 0;
    double res = 0;
    for(int i = 0;i <n;i++){
        read(fd[i][0],&area2,sizeof(double));
        res += area2;
    }
	end = clock();
	double time = ((double) (end - start)) / CLOCKS_PER_SEC;


	printf(">> RESULT <<\n");
	printf("sum = %lf\nn = %d\ndx = %lf\n", res, n, interval_width);
	printf("TIME USED: %fs\n", time);
	printf("-------------\n");
	exit(0);
}