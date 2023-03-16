#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    if (argc != 5) {
		printf("Wrong arguments amount!\n");
		return -1;
	}

	char* report_name = "pomiar_zad_1.txt";
	FILE* report = fopen(report_name, "wa");
    clock_t start, end;
    start = clock();
    

    FILE* file_in = fopen(argv[3], "r");
    if (file_in == NULL) {
        printf("Cannot open file!\n");
    }
    FILE* file_out = fopen(argv[4], "w");
    if (file_out == NULL) {
        printf("Cannot init file!\n");
    }
    char buffer[BUFFER_SIZE];
    while (!feof(file_in)) // to read file
    {
        int amount=fread(buffer, sizeof(char), BUFFER_SIZE, file_in);
		for(int i = 0; i < amount; i++) {
			if (buffer[i] == *argv[1]) {
				buffer[i] = *argv[2];
			}
		}
		fwrite(buffer, sizeof(char), amount, file_out);
    }

    fclose(file_in);
	fclose(file_out);

    end = clock();
    printf("LIB implementation execution time: %f seconds\n",(double)(end - start) / CLOCKS_PER_SEC);
    fprintf(report,"LIB implementation execution time: %f seconds\n",(double)(end - start) / CLOCKS_PER_SEC);
    start = clock();

    int file_in2 = open(argv[3], O_RDONLY);
    if (file_in2 == -1) {
        printf("Cannot open file!\n");
        return -1;
    }

	int file_out2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	if (file_out2 == -1) {
		printf("Cannot init file!\n");
        return -1;
	}
    char buffer2[BUFFER_SIZE];
    int amount2;
    while((amount2 = read(file_in2, buffer2, BUFFER_SIZE)) > 0) {
		for(int i = 0; i < amount2; i++) {
			if (buffer2[i] == *argv[1]) {
				buffer2[i] = *argv[2];
			}
		}
		write(file_out2, buffer2, amount2);
	}

    if (close(file_in2) == -1) {
        printf("Error closing input file: %s!\n", argv[3]);
        return -1;
    }
    if (close(file_out2) == -1) {
        printf("Error closing output file: %s!\n", argv[4]);
        return -1;
    }

    end = clock();
    printf("SYS implementation execution time: %f seconds\n",(double)(end - start) / CLOCKS_PER_SEC);
    fprintf(report,"SYS implementation execution time: %f seconds\n",(double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}