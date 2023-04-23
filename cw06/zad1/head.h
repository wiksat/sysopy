#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define MAX_CLIENTS 10
#define MAX_MSG_LEN 256
#define PROJECT_ID 1

typedef struct msg_buff {
	long mtype;
	int client_id;
	int catcher_pid;
	key_t q_key;
	char content[MAX_MSG_LEN];
	struct tm time_struct;
} msg_buff;

typedef enum mtype {
	INIT = 1,
	LIST = 2,
	TALL = 3,
	TONE = 4,
	STOP = 5
} mtype;

typedef struct client {
	int id;
	key_t key;
} client;

#endif // HEAD_H