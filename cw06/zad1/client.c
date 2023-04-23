#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <poll.h>

#include "head.h"

key_t key;
int client_idx;
int client_pid;
int server_queue;
int client_queue;
int waiting = 0;


void handle_init() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = INIT;
	msg.q_key = key;
	msg.client_id = client_pid;
	msg.time_struct = *localtime(&msg_time);

	int sndtest = msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	int rcvtest = msgrcv(client_queue, &msg, sizeof(msg_buff), 0, 0);

	client_idx = msg.client_id;
	if (client_idx >-1 || sndtest > -1 || rcvtest > -1) {

		printf("Initialization OK\n");
	}
	else {
		printf("Initialization NOK!\n");
	}
}
void handle_2all(char* message) {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = TALL;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);
	strcpy(msg.content, message);

	msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
}
void handle_stop() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = STOP;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	msgctl(client_queue, IPC_RMID, NULL);
	exit(0);
}
void handle_2one(char* message, int c_pid) {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = TONE;
	msg.client_id = client_idx;
	msg.catcher_pid = c_pid;
	msg.time_struct = *localtime(&msg_time);
	strcpy(msg.content, message);

	msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
}
void handle_server_message() {
	msg_buff *msg_rcv = malloc(sizeof(msg_buff));

	while(msgrcv(client_queue, msg_rcv, sizeof(msg_buff), 0, IPC_NOWAIT) >= 0);

	if (msg_rcv->mtype == STOP) {
		printf("Recived stop message, leaving...\n");
		handle_stop();
	}
	else if (msg_rcv->mtype == TALL || msg_rcv->mtype == TONE) {
		struct tm msg_time = msg_rcv->time_struct;
		printf("\n[%02d:%02d:%02d] [%d]: \"%s\"\n",
			msg_time.tm_hour,
			msg_time.tm_min,
			msg_time.tm_sec,
        	msg_rcv->client_id,
        	msg_rcv->content);
		waiting = 0;
	}
}


void handle_list() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = LIST;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	msgrcv(client_queue, &msg, sizeof(msg_buff), 0, 0);
	printf(">>> ACTIVE CLIENTS <<<\n");
	printf("%s", msg.content);
}
int main() {
    setbuf(stdout, NULL);
	printf("PID: %d\n", getpid());

	key = ftok(getenv("HOME"), getpid());
	client_queue = msgget(key, IPC_CREAT | 0666);
	if (client_queue == -1) {
		perror("msgget");
		exit(1);
	}
	key_t server_key = ftok(getenv("HOME"), PROJECT_ID);
	server_queue = msgget(server_key, 0);
	client_pid = getpid();
	handle_init();

	signal(SIGINT, handle_stop);

	struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

	size_t len;
	ssize_t read;
	char* line = NULL;

	printf("AVAILABLE COMMANDS: [LIST|2ALL|2ONE|STOP]\n\n");
	while (1) {
		handle_server_message();
        if (!waiting) {
			printf("> ");
			fflush(stdout);
			waiting = 1;
		}
		int ret = poll(fds, 1, 1000);
		if (ret == 0) {
			continue;
		}
		else if (ret > 0 && fds[0].revents & POLLIN) {
			read = getline(&line, &len, stdin);
			line[read - 1] = '\0';
			waiting = 0;
		}
        else {
            perror("poll");
            exit(1);
        }

		if (strcmp(line, "") == 0) {
			continue;
		}

		char* command = strtok(line, " \n");
		if (strcmp(command, "INIT") == 0) {
			handle_init();
		}
		else if (client_idx == -1) {
			printf("You need to set server connection first!\n");
		}
		else if (strcmp(command, "LIST") == 0) {
			handle_list();
		}
		else if (strcmp(command, "2ALL") == 0) {
			command = strtok(NULL, " \n");
			char* message = command;
	
			handle_2all(message);
			printf("Message was sent\n");
		}
		else if (strcmp(command, "2ONE") == 0) {
			command = strtok(NULL, " \n");
			

			int c_pid = atoi(command);
			command = strtok(NULL, " \n");
			char* message = command;

			handle_2one(message, c_pid);
			printf("Message was sent\n");
		}
		else if (strcmp(command, "STOP") == 0) {
			handle_stop();
		}
		else {
			printf("Unknown command!\n");
		}
	}
}