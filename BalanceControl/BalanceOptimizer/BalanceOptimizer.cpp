// BalanceOptimizer.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "../BalanceLib/PID_controller.h"
#include "../BalanceLib/test_PID_controller.h"
#include "../BalanceLib/Rocker.h"

#include <iostream>
#include <cmath>

static constexpr int FIVE_SECONDS = 5000;
static constexpr double MAX_ANGLE = 180.0 / 8;

bool run_trial(double, double, double);

int main()
{
	if (!test_PID_controller())
	{
		std::cout << "PID controller test failed.\n";
		return 1;
	}

	run_trial(0.89, 0.0, 0.0);

    return 0;
}

bool run_trial(double kp, double ki, double kd)
{
	Rocker rock_me_amadeus(0.0, 0.0, 0.5);
	PID_controller controller(kp, ki, kd, -MAX_ANGLE, MAX_ANGLE);

	double distance_traveled = 0.0;

	for (int step = 0; step < FIVE_SECONDS; ++step)
	{
		double current_position = rock_me_amadeus.get_position();
		controller.set_process_value(current_position);

		controller.step();
		rock_me_amadeus.step();

		if (!rock_me_amadeus.is_ball_on_rocker())
		{
			std::cout << "Free fall!\n";
			return false;
		}
		double updated_position = rock_me_amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = -controller.get_control_value();
		rock_me_amadeus.set_angle(angle);
	}

	double ball_speed = rock_me_amadeus.get_ball_speed();
	ball_speed -= 0.5;
	rock_me_amadeus.set_ball_speed(ball_speed);

	for (int step = 0; step < FIVE_SECONDS; ++step)
	{
		double current_position = rock_me_amadeus.get_position();
		controller.set_process_value(current_position);

		controller.step();
		rock_me_amadeus.step();

		if (!rock_me_amadeus.is_ball_on_rocker())
		{
			std::cout << "Free fall!\n";
			return false;
		}

		double updated_position = rock_me_amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = controller.get_control_value();
		rock_me_amadeus.set_angle(angle);

		std::cout << current_position << " cm\t" << angle << " degree\n";
	}

	std::cout << "Distance: " << distance_traveled << " cm\n";

	return true;
}