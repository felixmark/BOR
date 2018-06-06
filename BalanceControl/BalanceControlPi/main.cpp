#include "../BalanceLib/Rocker.h"

#include <iostream>
#include <thread>
#include "PiRocker.h"

using namespace std;

int main(void) {
	wiringPiSetup();

	Pi_rocker pi_rocker;
	pi_rocker.run();

	return 0;
}
