#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>

// IPC functino
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <signal.h>

#define NAME_LEN	20
#define NUM_OF_STU	3

struct my_stu {
	char name[NAME_LEN];
	int id;
	int kor;
	int eng;
	int math;
	int total;
	int avg;

};

int my_sync = 0;


void mysyncHandler(int sync){
	my_sync = !my_sync;
}

int main(void){
	int shmid;
	struct my_stu * stu;
	void * shmmem = (void *)0;
	
	pid_t pid;


	// SIGUSR1 for sync between child with parent
	signal(SIGUSR1, mysyncHandler);

	// step 0. ftok()
	
	// step 1. shmget()
	// args are a key, size of structure and numboer of structure, perm | flag
	// return is shmid
	shmid = shmget((key_t)1234, sizeof(struct my_stu) * NUM_OF_STU, 0666 | IPC_CREAT);
	if(shmid == -1){
		fprintf(stderr, "shmget failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// step 2. shmat()
	// args : shmid, addr which you want to get, flag
	// return : address which is allocated
	shmmem = shmat(shmid, (void *)0, SHM_RND);

	// type conversion void * to struct *
	stu = (struct my_stu *)shmmem;

	// step 3. memory access

	// child is receive process and parent is send process
	pid = fork();

	switch(pid){
	// failed
	case -1 : 
		fprintf(stderr, "fork() error : %d\n", errno);
		exit(EXIT_FAILURE);
	// child process
	case 0 : 
		// if my_sync == 0, then keep going.
		// when parent give a signal SIGUSR1 to child, my_sync becomes 0
		while(!my_sync);
		for(int i=0; i < NUM_OF_STU; i++){
			stu[i].total = stu[i].kor + stu[i].eng + stu[i].math;
			stu[i].avg = stu[i].total / 3.0;
		}	
		for(int i=0; i < NUM_OF_STU; i++){
			printf("name : %s\tkor : %d\teng : %d\tmath : %d\ttotal : %d\tavg : %d\n", stu[i].name, stu[i].kor, stu[i].eng, stu[i].math, stu[i].total, stu[i].avg);
		}

	// step 4. shmdt()
		if(shmdt(shmmem) == -1){
			fprintf(stderr, "shmdt failed : %d\n", errno);
			exit(EXIT_FAILURE);
		}
	// step 5. shmctl(). IPC_RMID
		if(shmctl(shmid, IPC_RMID, 0) == -1){
			fprintf(stderr, "shmid failed : %d\n", errno);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);

	default:
		for(int i=0; i < NUM_OF_STU; i++){
			printf("name : ");
			fgets(stu[i].name, NAME_LEN, stdin);	
			stu[i].id = i + 1;
			printf("kor : ");
			scanf("%d", &stu[i].kor);
			while(getchar() != '\n');
			printf("eng : ");
			scanf("%d", &stu[i].eng);
			while(getchar() != '\n');
			printf("math : ");
			scanf("%d", &stu[i].math);
			while(getchar() != '\n');
		}	
		kill(pid, SIGUSR1);
		wait(0);
		exit(EXIT_SUCCESS);
	}
}
