#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// IPC function
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mymsg.h"

int main(void){
	int running = 1;
	struct myData person[10];
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
		printf("Name: ");
		fgets(person[personNum].name, sizeof(person[personNum].name), stdin);
		if(strncmp(person[personNum].name, "end", 3) == 0){
			
			person[personNum].msgType = 2;
			running = 0;	
		} else{
			printf("Height: ");
			scanf("%d", &person[personNum].height);
			// clear input buffer
			while(getchar() != '\n');
			printf("weight: ");
			scanf("%d", &person[personNum].weight);
			while(getchar() != '\n');
			person[personNum].msgType = 1;		// it depends on each person
		}

		

		// step 2. mesage send
		if(msgsnd(msgid, &person[personNum], sizeof(person) - sizeof(long), 0) ==  -1){
			// long is msgType, so it doesn't send.
			// message is nod sneded
			fprintf(stderr, "Error: msgsnd failed : %d\n", errno);
			exit(EXIT_FAILURE);	
		}
		personNum++;
	}
	exit(EXIT_SUCCESS);
}
