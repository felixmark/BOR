#include "PiRocker.h"
#include <fstream>
#include "json.hpp"
#include "PinManager.h"
#include "DigitalOutput.h"
#include "Status.h"
#include "DigitalInput.h"
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;

void Pi_rocker::clear_screen() {
#ifdef _WIN32
	std::system("cls");
#else
	// Assume POSIX
	std::system("clear");
#endif
}

Pi_rocker::Pi_rocker(nlohmann::json json_pins) :
	rocker_{ 0, 100, 0 }, 
	led_1_{ Digital_output("led_1", json_pins["led_1"].get<int>(), Status::OFF) },
	led_2_{ Digital_output("led_2", json_pins["led_2"].get<int>(), Status::OFF) },
	led_3_{ Digital_output("led_3", json_pins["led_3"].get<int>(), Status::OFF) },
	led_4_{ Digital_output("led_4", json_pins["led_4"].get<int>(), Status::OFF) },
	led_5_{ Digital_output("led_5", json_pins["led_5"].get<int>(), Status::OFF) },
	btn_1_{ Digital_input("btn_1", json_pins["btn_1"].get<int>(), PUD_DOWN) },
	btn_2_{ Digital_input("btn_2", json_pins["btn_2"].get<int>(), PUD_DOWN) } {
	/* Initialization list */
}

void Pi_rocker::run() {
	cout << rocker_;
	cout << "Loading complex physics simulation";
	for (int i = 0; i < 3; ++i) {
		cout << ".";
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	for (int cnt = 0; true; cnt++) {
		if (cnt % 10 == 0) {
			rocker_.set_angle(-rocker_.get_angle());
		}

		// Simulate 0,1 seconds
		clear_screen();
		for (int i = 0; i < 100; ++i) { rocker_.step(); }
		cout << rocker_;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}