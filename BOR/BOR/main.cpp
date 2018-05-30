#include <wiringPi.h>

#define	LED	17

int main(void)
{
	wiringPiSetup();

	pinMode(LED, OUTPUT);

	while (true) {
		digitalWrite(LED, HIGH);  // Ein
		delay(500); // ms
		digitalWrite(LED, LOW);	  // Aus
		delay(500);
	}
	return 0;
}