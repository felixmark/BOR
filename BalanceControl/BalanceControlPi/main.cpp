#include "../BalanceLib/piproxy.h"
#include <iostream>
#include "../BalanceLib/Rocker.h"
#include <thread>
#include <fstream>
#include "../BalanceLib/json.hpp"
#include "../BalanceLib/PinManager.h"
#include "../BalanceLib/DigitalOutput.h"
#include "../BalanceLib/Status.h"
#include "../BalanceLib/DigitalInput.h"

using namespace std;

using json = nlohmann::json;

int main(void) {
	wiringPiSetup();

	ifstream file("pins.json");
	json json_pins;
	file >> json_pins;

	Digital_output led_1("led_1", json_pins["led_1"].get<int>(), Status::OFF);
	Digital_output led_2("led_2", json_pins["led_2"].get<int>(), Status::OFF);
	Digital_output led_3("led_3", json_pins["led_3"].get<int>(), Status::OFF);
	Digital_output led_4("led_4", json_pins["led_4"].get<int>(), Status::OFF);
	Digital_output led_5("led_5", json_pins["led_5"].get<int>(), Status::OFF);
	Digital_input btn_1("btn_1", json_pins["btn_1"].get<int>(), PUD_DOWN);
	Digital_input btn_2("btn_2", json_pins["btn_2"].get<int>(), PUD_DOWN);

	Rocker rocker(80, 100, 0);

	cout << rocker;
	cout << "Loading complex physics simulation.." << endl;
	std::this_thread::sleep_for(std::chrono::seconds(4));

	for (int n = 0; n < 100; ++n) {
		// Simulate n x 0,5 seconds
		system("cls");
		for (int i = 0; i < 100; ++i) { rocker.step(); }
		cout << rocker;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
