#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ftw.h>

long long total_size = 0;

int print_info_about_file(const char *path, const struct stat *buff, int typefile __attribute__((unused))) {
	if (!S_ISDIR(buff->st_mode)) {
		printf("%s: %ld\n", path, buff->st_size);
		total_size += (long long)buff->st_size;
	}
	return 0;
}
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Wrong arguments amount!\n");
		return -1;
	}

	if (ftw( argv[1], print_info_about_file, 16) == -1) {
		//argumenty: nazwa katalogu, wskaźnik na funkcje dla elementu oraz glebokosc przeszukiwania
		printf("Error while ftw!\n");
		return -1;
	}

	printf("TOTAL SIZE: %lld\n", total_size);
    return 0;
}