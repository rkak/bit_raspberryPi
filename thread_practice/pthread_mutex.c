#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/types.h>

int cnt = 0;

// 
pthread_mutex_t mutex_cnt;

void * inc_function(void * arg){
	pthread_mutex_lock(&mutex_cnt);

	// --- critical section --- //
	printf("Before increase: %d\n", cnt);
	cnt++;
	printf("After increase: %d\n", cnt);
	// --- end critical section --- //

	pthread_mutex_unlock(&mutex_cnt);
}

void * dec_function(void * arg){
	pthread_mutex_lock(&mutex_cnt);

	// --- critical section --- //
	printf("Before decrease: %d\n", cnt);
	cnt--;
	printf("After decrease: %d\n", cnt);
	// --- end critical section --- //

	pthread_mutex_unlock(&mutex_cnt);
}

int main(void){
	pthread_t p_thread[2];

	// mutex initialization
	// what is pthread_mutex_t???
	pthread_mutex_init(&mutex_cnt, NULL);

	pthread_create(&p_thread[0], NULL, inc_function, NULL);
	pthread_create(&p_thread[1], NULL, dec_function, NULL);

	pthread_join(p_thread[0], NULL);
	pthread_join(p_thread[1], NULL);

	pthread_mutex_destroy(&mutex_cnt);

	return 0;

}
