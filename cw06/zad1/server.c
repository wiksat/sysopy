#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "head.h"

int server_q;
client clients[MAX_CLIENTS];

void signal_handler(int sig) {
	if (server_q != -1) {
		msg_buff msg;
		msg.mtype = STOP;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].key != -1) {
				int client_qid = msgget(clients[i].key, 0);
				msgsnd(client_qid, &msg, sizeof(msg_buff), 0);
			}
		}
	}
	msgctl(server_q, IPC_RMID, NULL);
	exit(0);
}
void rcv_init(msg_buff msg) {
	int client_id = 0; 
	while (client_id < MAX_CLIENTS && clients[client_id].key != -1) {
		client_id++;
	}

	if (client_id == MAX_CLIENTS) {
		printf("SERVER - Server can only handle %d clients! All slots are taken.\\n", MAX_CLIENTS);
		msg.client_id = -1;
	}
	else {
		clients[client_id].id = msg.client_id;
		clients[client_id].key = msg.q_key;
		msg.client_id = client_id;
	}

	
	int client_queue = msgget(msg.q_key, 0);
	msgsnd(client_queue, &msg, sizeof(msg_buff), 0);

	struct tm msg_time = msg.time_struct;
	printf("SERVER - [%02d:%02d:%02d] | New client added: %d\n", 
		msg_time.tm_hour,
		msg_time.tm_min,
		msg_time.tm_sec,
		clients[client_id].id);
}

void rcv_stop(msg_buff msg) {
	int id = msg.client_id;
	int client_queue = msgget(clients[id].key, 0);
	printf("SERVER - Client [%d] has left the server\n", clients[id].id);
	clients[id].id = -1;
	clients[id].key = -1;
	msgsnd(client_queue, &msg, sizeof(msg_buff), 0);
}


void rcv_list(msg_buff msg) {
	int id = msg.client_id;
	char tmp[256];
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].key != -1) {
			sprintf(tmp, "ID: %d\n", clients[i].id);
			strcat(msg.content, tmp);
		}
	}

	struct tm msg_time = msg.time_struct;
	printf("[%d] - [%02d:%02d:%02d] | LIST\n", 
		clients[id].id,
		msg_time.tm_hour,
		msg_time.tm_min,
		msg_time.tm_sec);


	int client_queue = msgget(clients[id].key, 0);
	msgsnd(client_queue, &msg, sizeof(msg_buff), 0);
}
void rcv_2all(msg_buff msg) {
	int id = msg.client_id;
	msg.client_id = clients[id].id;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (i != id && clients[i].key != -1) {
			int client_queue = msgget(clients[i].key, 0);
			msgsnd(client_queue, &msg, sizeof(msg_buff), 0);
		}
	}

	struct tm msg_time = msg.time_struct;
	printf("[%d] - [%02d:%02d:%02d] - 2ALL\n", 
		clients[id].id,
		msg_time.tm_hour,
		msg_time.tm_min,
		msg_time.tm_sec);
}

void rcv_2one(msg_buff msg) {
	int catcher_pid = msg.catcher_pid;

	int i = 0;
	while(i < MAX_CLIENTS && clients[i].id != catcher_pid) {
		i++;
	}
	if (i == MAX_CLIENTS) {
		printf("Can not find [%d] user\n", catcher_pid);
	}
	else {
		int client_queue = msgget(clients[i].key, 0);
		msg.client_id = clients[msg.client_id].id;
		msgsnd(client_queue, &msg, sizeof(msg_buff), 0);

		struct tm msg_time = msg.time_struct;
		printf("[%d] - [%02d:%02d:%02d] - 2ONE: %d\n", 
		msg.client_id,
		msg_time.tm_hour,
		msg_time.tm_min,
		msg_time.tm_sec,
		catcher_pid);
	}
}

int main() {
    setbuf(stdout, NULL);
	signal(SIGINT, signal_handler);

	for (int i = 0; i < MAX_CLIENTS; i++) {
		client new_client;
		new_client.key = -1;
		clients[i] = new_client;
	}

	key_t s_key = ftok(getenv("HOME"), PROJECT_ID);
	if (s_key == -1) {
		perror("ftok");
		exit(1);
	}

	server_q = msgget(s_key, IPC_CREAT | 0666);
	if (server_q == -1) {
		perror("msgget");
		exit(1);
	}

	printf("SERVER - starting server...\n");

    msg_buff msg;
	while (1) {
		msgrcv(server_q, &msg, sizeof(msg_buff), -6, 0);

		switch(msg.mtype) {
			case INIT:
				rcv_init(msg);
				break;
			case STOP:
				rcv_stop(msg);
				break;
			case LIST:
				rcv_list(msg);
				break;
			case TALL:
				rcv_2all(msg);
				break;
			case TONE:
				rcv_2one(msg);
				break;
			default:
				printf("Wrong message type [%ld]\n", msg.mtype);
		}
	}
}

