#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Wrong arguments amount!\n");
		return -1;
	}


	char* report_name = "pomiar_zad_2.txt";
	FILE* report_file = fopen(report_name, "wa");

	FILE* file_in = fopen(argv[1], "rb");
	if (file_in==NULL) {
        printf("Cannot open file!\n");
		return -1;
	}
	FILE* file_out = fopen(argv[2], "wb");
	if (file_out==NULL) {
        printf("Cannot init file!\n");
		return -1;
	}
    clock_t start, end;

    // zczytanie długości pliku
	fseek(file_in, 0, SEEK_END);
	long file_size = ftell(file_in);
	fseek(file_in, 0, SEEK_SET);

	start = clock();

    char buffer[1];
	for(int i = file_size - 1; i >= 0; i--) {
		fseek(file_in, i, SEEK_SET);
		fread(buffer, sizeof(char), 1, file_in);
		fwrite(buffer, sizeof(char), 1, file_out);
	}


	end = clock();
    printf("CHAR implementation execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	fprintf(report_file, "CHAR implementation execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);


	start = clock();

    fseek(file_out, 0, SEEK_SET);
    char buffer2[BUFFER_SIZE];
    int blocks = file_size / BUFFER_SIZE;
	int remainder = file_size % BUFFER_SIZE;
    
    if (remainder > 0) {
        fseek(file_in, (blocks)*BUFFER_SIZE, SEEK_SET);
        fread(buffer2, remainder, 1, file_in);
        fseek(file_in, 0, SEEK_SET);
        char rev_buffer[BUFFER_SIZE];
		for(int j = 0; j < remainder; j++) {
			rev_buffer[j] = buffer2[remainder - 1 - j];
		}
		fwrite(rev_buffer, sizeof(char), remainder, file_out);
    }


    for (int i = blocks - 1; i >= 0; i--) {
        fseek(file_in, i * BUFFER_SIZE, SEEK_SET);
        fread(buffer2, BUFFER_SIZE, 1, file_in);
        fseek(file_in, -BUFFER_SIZE, SEEK_CUR);
        char rev_buffer[BUFFER_SIZE];
		for(size_t j = 0; j < BUFFER_SIZE; j++) {
			rev_buffer[j] = buffer2[BUFFER_SIZE - 1 - j];
		}
		fwrite(rev_buffer, sizeof(char), BUFFER_SIZE, file_out);
    }



	end = clock();
	printf( "BLOCK implementation execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	fprintf(report_file, "BLOCK implementation execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	fclose(file_in);
	fclose(file_out);
	fclose(report_file);

	return 0;
}