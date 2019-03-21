#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// ipc function
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "mymsg.h"

int main(void){
	int running = 1;
        struct myData person[10];
	struct msqid_ds msqstat;	// a number of the rest of msg in queue
        int msqid;
        int personNum = 0;
        int msgid;

        // step 0. ftok();

        // step 1.
        msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
        if(msgid == -1){
                fprintf(stderr, "Error : msgget failed : %d\n", errno);
                exit(EXIT_FAILURE);
        }
         
 
        while(running){
		if(msgrcv(msgid, &person[personNum], sizeof(person) - sizeof(long), 0, 0) == -1){
			fprintf(stderr, "msgrcv failed : %d", errno);
			exit(EXIT_FAILURE);
		}

		if(person[personNum].msgType == 2){
			running = 0;
		} else{
			printf("Receive Data Person[%d]\n", personNum);
			printf("Name : %s\nheight : %d\nweight : %d\n", person[personNum].name, person[personNum].height, person[personNum].weight);
		}

		if(msgctl(msgid, IPC_STAT, &msqstat) == -1){
			perror("Fail : msgctl()");
			exit(EXIT_FAILURE);
		}
		printf("remain message count : %d\n", msqstat.msg_qnum);

		if(msqstat.msg_qnum > 0)
			running = 1;
	}

	// step3. msgctl IPC_RMID
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "Fail : msgctl() IPC_RMID : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
