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

constexpr int LED_CNT = 5;

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

	led_1_.write(Status::ON);
	led_2_.write(Status::ON);
	led_3_.write(Status::ON);
	led_4_.write(Status::ON);
	led_5_.write(Status::ON);
	this_thread::sleep_for(chrono::seconds(1));
	led_1_.write(Status::OFF);
	led_2_.write(Status::OFF);
	led_3_.write(Status::OFF);
	led_4_.write(Status::OFF);
	led_5_.write(Status::OFF);

	cout << rocker_;
	cout << "Loading complex physics simulation";
	for (int i = 0; i < 3; ++i) {
		cout << ".";
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	for (int cnt = 0; true; cnt++) {
		// Simulate 0,1 seconds
		clear_screen();

		for (long cnt = 0; cnt < sim_time; ++cnt) {
			double current_position = rocker_.get_position();
			controller.set_process_value(current_position);
			rocker_.step();
			controller.step();
			double angle = -controller.get_control_value();
			rocker_.set_angle(angle);
		}

		if (rocker_.get_position() > -100 && rocker_.get_position() < 200 / LED_CNT - 100) {
			turn_one_led_on(1);
		} else if (rocker_.get_position() >  200 / LED_CNT - 100 && rocker_.get_position() < -10) {
			turn_one_led_on(2);
		} else if (rocker_.get_position() >  -10 && rocker_.get_position() < 10) {
			turn_one_led_on(3);
		} else if (rocker_.get_position() >  10 && rocker_.get_position() < 4 * 200 / LED_CNT - 100) {
			turn_one_led_on(4);
		} else if (rocker_.get_position() >  4 * 200 / LED_CNT - 100 && rocker_.get_position() < 5 * 200 / LED_CNT - 100) {
			turn_one_led_on(5);
		} else if (rocker_.get_position > 200 || rocker_.get_position < -200) {
			led_1_.write(Status::ON);
			led_2_.write(Status::ON);
			led_3_.write(Status::ON);
			led_4_.write(Status::ON);
			led_5_.write(Status::ON);
		}

		cout << rocker_;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Pi_rocker::turn_one_led_on(int led) {
	led_1_.write(led == 1 ? Status::ON : Status::OFF);
	led_2_.write(led == 2 ? Status::ON : Status::OFF);
	led_3_.write(led == 3 ? Status::ON : Status::OFF);
	led_4_.write(led == 4 ? Status::ON : Status::OFF);
	led_5_.write(led == 5 ? Status::ON : Status::OFF);
}