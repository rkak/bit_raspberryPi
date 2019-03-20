#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msg_st{
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main(void){
	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;
	struct msqid_ds msqstat;

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if(msgctl(msgid, IPC_STAT, &msqstat) == 1){
		perror("msgclt() fail");
	}

	while(running){
		if(msgrcv(msgid, (void *)&some_data, sizeof(some_data), msg_to_receive, 0) == -1){
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s", some_data.some_text);
		if(strncmp(some_data.some_text, "end", 3) == 0){
			running = 0;
		}

		msqstat.msg_qnum--;
		if(msqstat.msg_qnum > 0){
			running = 1;
		}
	}
}
