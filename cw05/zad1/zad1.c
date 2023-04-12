#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <limits.h>

void listing(char *option){
    FILE *file;
    char *command,line[LINE_MAX];
    if (strcmp(option,"sender") == 0){
        command = "echo | mail -H | sort -k 2";
    }else if (strcmp(option,"date") == 0){
        command = "echo | mail -H | sort -k 3";
    }else{
        fprintf(stderr,"Wrong option\n");
        exit(1);
    }
    if ((file = popen(command, "r")) == NULL) {
        fprintf(stderr, "Error while opening pipe\n");
        exit(1);
    }
    while(fgets(line,LINE_MAX,file) != NULL ){
        printf("%s \n", line);
    }
    pclose(file);
}
void sending(char *adress,char *title,char *content){
    FILE *file;
    char command[LINE_MAX];
    sprintf(command, "echo %s | mail -s %s %s", content, title, adress);
    if ((file = popen(command, "r")) == NULL) {
        perror("popen error\n");
        exit(1);
    }
    printf("Email sent to: %s\nTitle: %s\nContent:\n%s\n\n", adress, title, content);
    
    pclose(file);
}
int main(int argc,char *argv[]){
    if (argc != 2 && argc != 4){
        fprintf(stderr,"Wrong number of arguments \n");
        exit(1);
    }
    if (argc == 2){
        char *option = argv[1];
        listing(option);
    }else {
        char *adress = argv[1];
        char *title = argv[2];
        char *content = argv[3];
        sending(adress,title,content);
    }
    return 0;
}