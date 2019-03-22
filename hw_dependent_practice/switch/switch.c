#include <wiringPi.h>

#define SW1	1
#define SW2	3
#define LED1	2

void pinAssign(void){
    pinMode(SW1, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(SW2, INPUT);
}

int main()
{
  wiringPiSetup();
  char val1, val2;

  pinAssign();

  while(1)
  { 
   val1=digitalRead(SW1);
   val2 = digitalRead(SW2);

   if(val1 && val2)//check if the button is pressed, if yes, turn on the LED
	digitalWrite(LED1,LOW);
   else
	digitalWrite(LED1,HIGH);
  }	
}

