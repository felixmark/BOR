#pragma once
#include "../BalanceLib/Rocker.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "json.hpp"

class Pi_rocker {
private:
	// =============== PRIVATE VARIABLES ===============
	Rocker rocker_;
	Digital_output led_1_;
	Digital_output led_2_;
	Digital_output led_3_;
	Digital_output led_4_;
	Digital_output led_5_;
	Digital_input btn_1_;
	Digital_input btn_2_;

	// =============== PRIVATE METHODS ===============
	void clear_screen();
	void turn_one_led_on(int led);

public:
	// =============== PUBLIC METHODS ===============
	Pi_rocker(nlohmann::json json);
	void run();
};

