#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define loopCount	5
// #define delayTime 	1000
// #define delayMax	1024

// #define DEBUG

int ctrlServo(int gpio){
	int i;
	int j;
	int k;
	int period = 20000;
	int pwmtime = 1000;
	// step 2. pin direction setup
	pinMode(gpio, OUTPUT);
/*	for(i = 0; i < period; i++){
		digitalWrite(gpio, HIGH);
		usleep(pwmtime);
		digitalWrite(gpio, LOW);
		usleep(period-pwmtime);
	}
*/
	while(1){
		digitalWrite(gpio, HIGH);
		usleep(pwmtime);
		digitalWrite(gpio, LOW);
		usleep(period-pwmtime);
	}
	return 0;
}


int main(int argc, char *argv[]){
	int i;
	int gpioNo;

	// stemp 1. setupt wiring Pi.
	wiringPiSetup();		// initialization of wiringPi

	// Pin number error
	if(argc < 2){
		printf("Usage : %s pgioNo\n", argv[0]);
		return -1;
	}

	gpioNo = atoi(argv[1]);

#ifdef DEBUG 
	printf("gpioNo : %d\n", gpioNo);
#endif


	ctrlServo(gpioNo);

	return 0;
}
