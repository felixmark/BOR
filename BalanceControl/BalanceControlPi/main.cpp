#include "../BalanceControlWin/piproxy.h"
#include "../BalanceLib/Rocker.h"
#include "../BalanceLib/PID_controller.h"

#include <iostream>
#include <thread>
#include <fstream>
#include "PiRocker.h"

using namespace std;
using Json = nlohmann::json;

int main(void) {
	wiringPiSetup();

	ifstream file("pins.json");
	Json json_pins;
	file >> json_pins;
	file.close();
	
	Pi_rocker pi_rocker(json_pins);
	pi_rocker.run();

	return 0;
}
