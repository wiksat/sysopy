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

mqd_t server_queue;
mqd_t client_queue;
int waiting = 0;
char qName[10];

char random_letter() {
	return rand() % ('Z' - 'A' + 1) + 'A';
}

int handle_init() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = INIT;
	msg.client_id = client_pid;
	msg.time_struct = *localtime(&msg_time);

	strcpy(msg.content, qName);
	int sndtest = mq_send(server_queue, (char *) &msg, sizeof(msg_buff), 0);
	int rcvtest = mq_receive(client_queue, (char *) &msg, sizeof(msg_buff), NULL);

	client_idx = msg.client_id;
	if (client_idx == -1 || sndtest == -1 || rcvtest == -1) {
		printf("Initialization NOK\n");
	}
	else {
		printf("Initialization OK\n");
	}

	return client_idx;
}

void handle_2all(char* message) {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = TALL;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);
	strcpy(msg.content, message);
	// msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	mq_send(server_queue, (char *) &msg, sizeof(msg_buff), 0);
}
void handle_stop() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = STOP;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	// msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	// msgctl(client_queue, IPC_RMID, NULL);

	mq_send(server_queue, (char *) &msg, sizeof(msg_buff), 0);
	mq_close(server_queue);
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
	mq_send(server_queue, (char *) &msg, sizeof(msg_buff), 0);
	// msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
}
void handle_server_message() {
	msg_buff msg_rcv;
	struct timespec my_time;
	clock_gettime(CLOCK_REALTIME, &my_time);
	my_time.tv_sec += 0.1;
	while (mq_timedreceive(client_queue, (char *) &msg_rcv, sizeof(msg_buff), NULL, &my_time) != -1) {
		if (msg_rcv.mtype == STOP) {
			printf("Recived stop message, leaving...\n");
			handle_stop();
		}
		else {
			struct tm msg_time = msg_rcv.time_struct;
			printf("\n[%02d:%02d:%02d] [%d]: \"%s\"\n",
				msg_time.tm_hour,
				msg_time.tm_min,
				msg_time.tm_sec,
	        	msg_rcv.client_id,
	        	msg_rcv.content);
			waiting = 0;
		}
	}
}


void handle_list() {
	time_t msg_time = time(NULL);
	msg_buff msg;
	msg.mtype = LIST;
	msg.client_id = client_idx;
	msg.time_struct = *localtime(&msg_time);

	// msgsnd(server_queue, &msg, sizeof(msg_buff), 0);
	// msgrcv(client_queue, &msg, sizeof(msg_buff), 0, 0);

	mq_send(server_queue, (char *) &msg, sizeof(msg_buff), 0);
	mq_receive(client_queue, (char *) &msg, sizeof(msg_buff), NULL);
	printf(">>>> ACTIVE CLIENTS <<<<\n");
	printf("%s", msg.content);
}
void random_name() {
	qName[0] = '/';
	for (int i = 1; i < 10; i++) {
		qName[i] = random_letter();
	}
}
int main() {
    setbuf(stdout, NULL);
	printf("PID: %d\n", getpid());
	client_pid = getpid();
	srand(time(NULL));
	random_name();
    client_queue = create_queue(qName);
    server_queue = mq_open(SERVER_Q, O_RDWR);
    client_idx = handle_init();	

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