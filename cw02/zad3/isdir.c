#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int main() {
    DIR *dirp; //struktura do folderu
    struct dirent *dp; //struktura do przechowywanie info o pojedynczym pliku
    struct stat buf; //wykorzystywana do przechowywania informacji o pliku w systemie plików.
    long long total_size = 0;
    
    dirp = opendir("."); //abecny folder
    if (dirp == NULL) {
        printf("Error while opening directory!\n");
        return -1;
    }
    
    while ((dp = readdir(dirp)) != NULL) {
        if (stat(dp->d_name, &buf) == -1) { //nazwa pliku do funkcji stat i wpisanie wyniku do buf
            printf("Stat Error!\n");
            return -1;
        }
        if (!S_ISDIR(buf.st_mode)) { //jeśli nie jest to katalog
            printf("%10lld %s\n", (long long) buf.st_size, dp->d_name); // to taki fajny tabulator, jest czytelniej
            total_size += buf.st_size;
        }
    }
    
    printf("Total size: %lld\n", total_size);
    
    if (closedir(dirp) == -1) {
        printf("Error while closing directory!\n");
        return -1;
    }
    
    return 0;
}