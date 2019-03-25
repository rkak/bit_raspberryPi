#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// POSIX Thread
#include <pthread.h>

// time
#include <sys/time.h>
#include <time.h>



int sum = 0;

void * sum_func(void * data){
	int i=0;
	int last = *(int *)data;

	for(i=1; i <= last; i++){
		sum += i;
	}
	return (void*)(last * last);
}

int main(void){
	time_t UTCtime;
	struct timeval UTCtime_p, UTCtime_c;
	long diff_sec = 0;
	long diff_usec = 0;


	pthread_t p_thread;
	int ret;
	int status;
	int last = 200000;

	gettimeofday(&UTCtime_p, NULL);

	if((ret = pthread_create(&p_thread, NULL, sum_func, (void*)&last)) < 0){
		perror("Error : pthread_create");
		return -1;
	}
	
	pthread_join(p_thread, (void **)&status);

	gettimeofday(&UTCtime_c, NULL);

	diff_sec = UTCtime_c.tv_sec - UTCtime_p.tv_sec;
	diff_usec = UTCtime_c.tv_usec - UTCtime_p.tv_usec;

	printf("time : %ld.%ld\n", diff_sec, diff_usec);
}
