#include <wiringPi.h>
#include <iostream>
#include "../BalanceLib/Rocker.h"

using namespace std;

int main(void)
{
	wiringPiSetup();

	Rocker rocker(10, 15, 0);
	cout << "Angle: " << rocker.get_angle() << endl;
	cout << "Position: " << rocker.get_angle() << endl;
	cout << "Elapsed time: " << rocker.get_time_elapsed() << endl;

	rocker.step();

	cout << "Angle: " << rocker.get_angle() << endl;
	cout << "Position: " << rocker.get_angle() << endl;
	cout << "Elapsed time: " << rocker.get_time_elapsed() << endl;

	return 0;
}