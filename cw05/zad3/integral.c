#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

double lower_bound = 0.0;
double upper_bound = 1.0;

double current_pos = 0.0;

double fun(double x) {
	return 4 / (x*x + 1);
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
	double begin = strtod(argv[1], NULL);
	double end = strtod(argv[2], NULL);
	double dx = strtod(argv[3], NULL);
	double res = calculate_integral(begin, end, dx);

    FILE* stream = fopen("/tmp/my_fifo","w");
    fwrite(&res,sizeof(double),1,stream);
    fclose(stream);
	exit(0);
}