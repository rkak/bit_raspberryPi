#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// IPC function
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	100

int main(void){
	int shmid;
	int * shmaddr;
	void *shared_mem = (void *)0;	// initialization
					// why do this?
					// it means no memory allocated in shared_mem
	int i;
	// step 0. ftok()

	// step 1. shmget()
	shmid = shmget((key_t)1234, sizeof(int) * SHMSIZE, 0666 | IPC_CREAT);
	if(shmid == -1){
		fprintf(stderr, "shmget failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	// step 2. shmat() second arg is what you want address.
	//		if (void *)0 means nothing what you want
	shared_mem = shmat(shmid, (void *)0, SHM_RND);	// it is like a malloc()
	if(shared_mem == (void *)-1){
		fprintf(stderr, "shmat failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	printf("Memory attached at %X\n", (int)shared_mem);

	shmaddr = (int *)shared_mem;

	// step 3. memory access
	for(i=0; i<SHMSIZE; i++){
		*(shmaddr + i) = i + 1;
		printf("shmaddr: %X, data: %d\n", shmaddr + i, *(shmaddr + i));
	}

	sleep(4);
/*
	// step 4. shmdt()
	if(shmdt(shared_mem) == -1){
		fprintf(stderr, "shmdt failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}

	// stpe 5. shmctl() : IPC_RMID
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}
*/

	exit(EXIT_SUCCESS);
}
