#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define loopCount	5
// #define delayTime 	1000
// #define delayMax	1024

// #define DEBUG

int ctrlLed(int gpio, int kind, int delayTime, int delayMax){
	int start = 0;
	int end = 0;

	start = delayTime;
	end = delayMax;

	while(1){
		digitalWrite(gpio, HIGH);
		usleep(start);
		digitalWrite(gpio, LOW);
		// led getting bright
		if(kind == 1){
			usleep(delayMax - start);
			start++;
			if(start >= delayMax)
				break;
		}else{
			usleep(delayTime - start);
			start--;
			if(start <= delayTime)
				break;
		}
	}
	return 0;
}

int ledOnOff(int gpio){
	int i;
	int delayTime = 0;
	int delayMax = 1024;
	// step 2. pin direction setup
	pinMode(gpio, OUTPUT);
	for(i = 0; i < loopCount; i++){
/*		for(delayTime = 0; delayTime < delayMax; delayTime++){
			digitalWrite(gpio, HIGH);
			usleep(delayTime);
			digitalWrite(gpio, LOW);
			usleep(delayMax - delayTime);
		}
		for(delayTime = delayMax; delayTime > 0; delayTime--){
			digitalWrite(gpio, HIGH);
			usleep(delayTime);
			digitalWrite(gpio, LOW);
			usleep(delayMax - delayTime);
		}
*/
		ctrlLed(gpio, 1, delayTime, delayMax);
		ctrlLed(gpio, 0, delayMax, delayTime);
	//	digitalWrite(gpio, LOW);
	//	usleep(100);
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


	ledOnOff(gpioNo);

	return 0;
}
