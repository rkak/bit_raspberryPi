#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// POSIX Thread
#include <pthread.h>

// time
#include <sys/time.h>
#include <time.h>

typedef struct {
	long long start;
	long long end;
	long long sum;
}mydata;

void * sum_func(void * data){ 
	int i=0;
	mydata * d = (mydata *)data;
	long long first = 0;
	long long last = 0;
	long long sum = 0;

	first = d->start;
	last = d->end;
//	printf("first : %d\tlast : %d\n", first, last);
	for(i=first; i <= last; i++){
		sum += i;
	}
	d->sum = sum;
	return (void*)(&(d->sum));
}

int main(void){
	time_t UTCtime;
	struct timeval UTCtime_p, UTCtime_c;
	long diff_sec = 0;
	long diff_usec = 0;

	pthread_t p_thread[2];
	int ret;
	int status;
	int value1;
	int value2;
	long long whole_start;
	long long whole_end;

	long long sum = 0;

	printf("input a start number : ");
	scanf("%lld", &whole_start);
	getchar();

	printf("input a end number : ");
	scanf("%lld", &whole_end);
	getchar();

	mydata d1;
	d1.start = whole_start;
	d1.end = (whole_end - whole_start + 1) / 2;

	gettimeofday(&UTCtime_p, NULL);

	if((ret = pthread_create(&p_thread[0], NULL, sum_func, (void*)&d1)) < 0){
		perror("Error : pthread[0]_create");
		return -1;
	}

	mydata d2;
	d2.start = d1.end + 1;
	d2.end = whole_end;
	
	if((ret = pthread_create(&p_thread[1], NULL, sum_func, (void*)&d2)) < 0){
		perror("Error : pthread[1]_create");
		return -1;
	}

	pthread_join(p_thread[0], (void **) &value1);
	pthread_join(p_thread[1], (void **) &value2);

	gettimeofday(&UTCtime_c, NULL);

	sum = *(long long *)value1 + *(long long *)value2;
	diff_sec = UTCtime_c.tv_sec - UTCtime_p.tv_sec;
	diff_usec = UTCtime_c.tv_usec - UTCtime_p.tv_usec;

	printf("time : %ld.%ld\n", diff_sec, diff_usec);
	printf("sum = %lld\n", sum);
}
