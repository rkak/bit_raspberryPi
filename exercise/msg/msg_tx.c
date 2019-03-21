#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// IPC function
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mymsg.h"

int main(void){
	int msqid;
	struct my_msg stu[NUM_OF_STU];
	int running = 1;	
	int num_stu = 0;
	// step 0. ftok()

	// step 1. msgget()
	msqid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msqid == -1){
		fprintf(stderr, "msgget() failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// step 2. msgsnd() / msgrcv()
	while(running){
		printf("stu name : ");
		fgets(stu[num_stu].name, sizeof(stu[num_stu].name), stdin);

		if(strncmp(stu[num_stu].name, "finish", 6) == 0){
			running = 0;
			stu[num_stu].msgType = 2;
		}else{
			stu[num_stu].msgType = 1;
			stu[num_stu].id = num_stu + 1;

			printf("stu %d\nkor : ", stu[num_stu].id);
			scanf("%d", &stu[num_stu].kor);
			while(getchar() != '\n');

			printf("eng : ");
			scanf("%d", &stu[num_stu].eng);
			while(getchar() != '\n');

			printf("math : ");
			scanf("%d", &stu[num_stu].math);
			while(getchar() != '\n');
		}

		if(msgsnd(msqid, &stu[num_stu], sizeof(stu[num_stu]), 0) == -1){
			fprintf(stderr, "msgsnd failed : %d\n", errno);
			exit(EXIT_FAILURE);
		}


		num_stu++;	
		
	}
	// step 3. msgctl()
	
	exit(EXIT_SUCCESS);
}
