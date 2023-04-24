
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "head.h"

mqd_t server_queue;
client clients[MAX_CLIENTS];

void signal_handler(int sig) {
		msg_buff msg;
		msg.mtype = STOP;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].id != -1) {
				// int client_qid = msgget(clients[i].key, 0);
				// msgsnd(client_qid, &msg, sizeof(msg_buff), 0);
				mqd_t destination=mq_open(clients[i].key,O_RDWR);
				mq_send(destination,(char *)&msg,sizeof(msg_buff),0);
				mq_receive(server_queue,(char *)&msg,sizeof(msg_buff),0);
				mq_close(destination);
			}
		}
	
	mq_close(server_queue);
	exit(0);
}

void rcv_init(msg_buff msg) {
	int client_idx = 0; 
	while (client_idx < MAX_CLIENTS && clients[client_idx].id != -1) {
		client_idx++;
	}

	if (client_idx == MAX_CLIENTS) {
		printf("SERVER  Server can only handle %d clients!\n", MAX_CLIENTS);
		msg.client_id = -1;
	}
	else {
		clients[client_idx].id = msg.client_id;
		strcpy(clients[client_idx].key, msg.content);
	}

	msg.client_id = client_idx;
	mqd_t cQueueDesc = mq_open(msg.content, O_RDWR);
	mq_send(cQueueDesc, (char *) &msg, sizeof(msg_buff), 0);
	mq_close(cQueueDesc);

	struct tm msg_time = msg.time_struct;
	printf("SERVER - [%02d:%02d:%02d] - New client: %d\n", 
		msg_time.tm_hour,
		msg_time.tm_min,
		msg_time.tm_sec,
		clients[client_idx].id);
}


void rcv_stop(msg_buff msg) {
	int id = msg.client_id;
	// int client_queue = msgget(clients[id].key, 0);
	printf("SERVER - Client [%d] has left the server\n", clients[id].id);
	clients[id].id = -1;
	clients[id].key = "";
	// msgsnd(client_queue, &msg, sizeof(msg_buff), 0);
}


void rcv_list(msg_buff msg) {
	int id = msg.client_id;
	strcpy(msg.content, "");
	char tmp[256];
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].id != -1) {
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


	// int client_queue = msgget(clients[id].key, 0);
	// msgsnd(client_queue, &msg, sizeof(msg_buff), 0);
	mqd_t destination = mq_open(clients[id].key, O_RDWR);
	mq_send(destination,(char *) &msg,sizeof(msg_buff),0);
	mq_close(destination);

}
void rcv_2all(msg_buff msg) {
	int id = msg.client_id;
	msg.client_id = clients[id].id;
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (i != id && clients[i].id != -1) {
			mqd_t catcher_desc = mq_open(clients[i].key, O_RDWR);
			mq_send(catcher_desc, (char *) &msg, sizeof(msg_buff), 0);
			mq_close(catcher_desc);
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
		// int client_queue = msgget(clients[i].key, 0);
		// msg.client_id = clients[msg.client_id].id;
		// msgsnd(client_queue, &msg, sizeof(msg_buff), 0);

		mqd_t destination = mq_open(clients[i].key, O_RDWR);
		mq_send(destination,(char *) &msg,sizeof(msg_buff),0);
		mq_close(destination);

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
		client *new_client = malloc(sizeof(client));
		new_client->id = -1;
		new_client->key = (char *) calloc(10, sizeof(char));
		clients[i] = *new_client;
	}

	server_queue = create_queue(SERVER_Q);


	printf("SERVER - starting server...\n");

    msg_buff msg;
	while (1) {
		mq_receive(server_queue, (char *)&msg, sizeof(msg_buff),NULL);
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

