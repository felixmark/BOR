#pragma once
#include "../BalanceLib/Rocker.h"
#include "DigitalOutput.h"
#include "DigitalInput.h"

class Pi_rocker {
private:
	Rocker rocker_;
	Digital_output led_1_;
	Digital_output led_2_;
	Digital_output led_3_;
	Digital_output led_4_;
	Digital_output led_5_;
	Digital_input btn_1_;
	Digital_input btn_2_;


public:
	Pi_rocker();
	void run();
};

