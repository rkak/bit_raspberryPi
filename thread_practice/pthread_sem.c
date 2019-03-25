#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//IPC
#include <sys/ipc.h>
#include <sys/sem.h>


#include <pthread.h>


/*
struct sembuf {
	short sem_num;	// semaphore number. it can be multiple.
	short sem_op;	// semaphore 
	short sem_flg;	// option
}

IPC_NOWAIT	: no wait
SEM_UNDO

*/

union semun{
	int	val;
	struct semid_ds	*buf;
	unsigned short int *array;
}arg;

int semid;
int cnt = 0;


void p(){
	struct sembuf pbuf;	//
	pbuf.sem_num = 0;
	pbuf.sem_op = -1;
	pbuf.sem_flg = SEM_UNDO;

	if(semop(semid, &pbuf, 1) == -1){
		perror("Error : p() semop()");
	}
}

void v(){
	struct sembuf vbuf;
	vbuf.sem_num = 0;
	vbuf.sem_op = 1;
	vbuf.sem_flg = SEM_UNDO;
	if(semop(semid, &vbuf, 1) == -1){
		perror("Error : v() semop()");
	}
}

void * fun_thread1(void * arg){	// return : void *
	while(1){
		p();
		cnt++;
		v();
		if(cnt > 50000)
			break;
	}

}

void * fun_thread2(void * arg){
	while(1){
		p();
		cnt --;
		v();
		if(cnt < 50000)
			break;
	}

}

void * fun_thread3(void * arg){
	while(1){
		p();
		printf("cnt : %d\n", cnt);
		fflush(stdout);
		v();
	}

}

int main(int argc, char *argv[]){
	// variable tid
	pthread_t p_thread[3];

	// step1. semget
	if((semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0666)) == -1){	// cannot be used this sempahore on other process
		perror("Error : semget()");
		return -1;

	}

	arg.val -1;

	// step2. semctl SETVAl
	if(semctl(semid, 0, SETVAL, arg) == -1){
		perror("Error : semctl() SETVAL");
		return -1;
	}


	if(pthread_create(&p_thread[0], NULL, fun_thread1, NULL) == -1){
		perror("Error : pthread_create() p_thread[0]");
	}
	if(pthread_create(&p_thread[1], NULL, fun_thread2, NULL) == -1){
		perror("Error : pthread_create() p_thread[1]");
	}
	if(pthread_create(&p_thread[2], NULL, fun_thread3, NULL) == -1){
		perror("Error : pthread_create() p_thread[1]");
	}

	pthread_join(p_thread[0], NULL);
	pthread_join(p_thread[1], NULL);
	pthread_join(p_thread[2], NULL);

	// step3. semctl IPC_RMID
	// close semaphore
	if(semctl(semid, 0, IPC_RMID, arg) == -1){
		perror("Error : semctl()");
		return -1;
	}
	printf("Program End\n");
	return 0;
}
