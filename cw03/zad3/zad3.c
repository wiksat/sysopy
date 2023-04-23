#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
void explore(char* path, char* chain){

    DIR *dirp; //struktura do folderu
    struct dirent *dp; //struktura do przechowywanie info o pojedynczym pliku
    struct stat buf; //wykorzystywana do przechowywania informacji o pliku w systemie plików.
    pid_t pid;
	char path_with_file[PATH_MAX];

    dirp = opendir(path); 


    if (dirp == NULL) {
		perror("opendir error");
		exit(1);
    }
    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, "..")==0 || strcmp(dp->d_name, ".") == 0) {
			continue;
		}
		snprintf(path_with_file, PATH_MAX, "%s/%s", path, dp->d_name);

        if (stat(path_with_file, &buf) == -1) { //nazwa pliku do funkcji stat i wpisanie wyniku do buf
			perror("stat error");
			exit(1);
        }
        if (S_ISDIR(buf.st_mode)) { //jeśli katalog
			pid = fork();
			if (pid == -1) {
				perror("fork error");
				exit(1);
			}
			else if (pid == 0) {
				explore(path_with_file, chain);
				exit(0);
			}
        }
        else{ // jeśli plik
            FILE* file;
			size_t len = 10;
            char* line=NULL;
			if ((file = fopen(path_with_file, "r")) == NULL) {
				perror("open error");
				exit(1);
			}

			getline(&line, &len, file); //funkcja automatycznie alokuja pamięć na potrzebę bufora (line-bufor, len-rozmiar bufora, file-plik)
			if (strncmp(line, chain, strlen(chain)) == 0) {
				printf("PID: %d, %s\n", getpid(), realpath(path_with_file, NULL));
			}

			free(line);
			fclose(file);
        } 
    }
    	if (closedir(dirp) == -1) {
		perror("closedir error");
		exit(1);
	}
}

int main(int argc, char* argv[]){
	if (argc != 3) {
		printf("Wrong arguments amount!\n");
		return 1;
	}
    char* path = argv[1];
    char* chain = argv[2];
    if (strlen(path) > 255) {
		printf("Text is too long\n");
		return 1;
    }

    explore(path,chain);

    return 0;
}