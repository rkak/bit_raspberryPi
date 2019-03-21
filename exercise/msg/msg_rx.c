#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MSG_RX

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
		if(msgrcv(msqid, &stu[num_stu], sizeof(stu[num_stu]) - sizeof(int) - sizeof(double), 0, 0) == -1){
			fprintf(stderr, "msgrcv failed : %d\n", errno);
			exit(EXIT_FAILURE);
		}

//		if(strncmp(stu[num_stu].name, "finish", 6) == 0){
		if(stu[num_stu].msgType == 2){
			running = 0;
		} else{
			stu[num_stu].total = stu[num_stu].kor + stu[num_stu].eng + stu[num_stu].math;
			stu[num_stu].avg = stu[num_stu].total / 3.0;
			
			printf("name : %sid : %d\tkor : %d\teng : %d\tmath : %d\ttotal : %d\tavg : %lf\n", stu[num_stu].name, stu[num_stu].id, stu[num_stu].kor, stu[num_stu].eng, stu[num_stu].math, stu[num_stu].total, stu[num_stu].avg);
			num_stu++;	
		}
	}
	// step 3. msgctl()
	if(msgctl(msqid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl() failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
