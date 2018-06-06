#include "PiRocker.h"
#include <fstream>
#include "json.hpp"
#include "PinManager.h"
#include "DigitalOutput.h"
#include "Status.h"
#include "DigitalInput.h"
#include <thread>
#include <chrono>

using namespace std;
using json = nlohmann::json;

Pi_rocker::Pi_rocker() : rocker_{0, 100, 0} {
	ifstream file("pins.json");
	json json_pins;
	file >> json_pins;

	led_1_ = Digital_output( "led_1", json_pins["led_1"].get<int>(), Status::OFF );
	led_2_ = Digital_output( "led_2", json_pins["led_2"].get<int>(), Status::OFF );
	led_3_ = Digital_output( "led_3", json_pins["led_3"].get<int>(), Status::OFF );
	led_4_ = Digital_output( "led_4", json_pins["led_4"].get<int>(), Status::OFF );
	led_5_ = Digital_output( "led_5", json_pins["led_5"].get<int>(), Status::OFF );
	btn_1_ = Digital_input( "btn_1", json_pins["btn_1"].get<int>(), PUD_DOWN );
	btn_2_ = Digital_input( "btn_2", json_pins["btn_2"].get<int>(), PUD_DOWN );
}

void Pi_rocker::run() {
	cout << rocker_;
	cout << "Loading complex physics simulation.." << endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	while(1) {
		// Simulate 0,1 seconds
		system("cls");
		for (int i = 0; i < 100; ++i) { rocker_.step(); }
		cout << rocker_;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
