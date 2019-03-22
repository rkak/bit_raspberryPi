#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>

// tim function
#include <time.h>
#include <sys/time.h>

#define SW1	1
#define SW2	3
#define LED1	2

void pinAssign(void){
    pinMode(SW1, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(SW2, INPUT);
}

int main(){
	wiringPiSetup();
	char val1, val2;
	int btnBlock = 0;
//	int blockTime = 0;

	struct timeval tv, start_tv, end_tv, diff_tv;
	time_t t;

	pinAssign();

	printf("If you push left button, it starts\n");

	while (digitalRead(SW1));


	if((t = gettimeofday(&start_tv, NULL)) == -1){
		return -1;
	}


	printf("If you push left button again, it is rapped\n");
	sleep(1);

	printf("Start\n");


	while (1) {
		if ((t = gettimeofday(&end_tv, NULL)) == -1) {
			perror("gettimeofday() call error");
			return -1;
		}

		if (start_tv.tv_usec > end_tv.tv_usec) {
			end_tv.tv_sec--;
			diff_tv.tv_usec = end_tv.tv_usec - start_tv.tv_usec + 1000000;
			diff_tv.tv_sec = end_tv.tv_sec - start_tv.tv_sec;
		}
		else {
			diff_tv.tv_usec = end_tv.tv_usec - start_tv.tv_usec;
			diff_tv.tv_sec = end_tv.tv_sec - start_tv.tv_sec;
		}
		diff_tv.tv_usec /= 10000;
		printf("Time : %2ld.%ld\r", diff_tv.tv_sec, diff_tv.tv_usec);
		fflush(stdout);
		usleep(500);
		//blockTime += 500 / 100;
		if (btnBlock && digitalRead(SW1)) {
			btnBlock = 0;
		}
		if (!digitalRead(SW1) && !btnBlock) {
			printf("\n");
			btnBlock = 1;
			blockTime = 0;
		}
		if (!digitalRead(SW2)) {
			printf("\n");
			break;
		}
	  }
	printf("Stopped\n");
}

