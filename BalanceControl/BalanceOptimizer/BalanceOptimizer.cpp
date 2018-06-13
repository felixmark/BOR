// BalanceOptimizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../BalanceLib/PID_controller.h"
#include "test_PID_controller.h"
#include "../BalanceLib/Rocker.h"

#include <iostream>
#include <random>
#include <array>
#include <cmath>

static constexpr int FIVE_SECONDS = 5000;
static constexpr double MAX_ANGLE = 180.0 / 8;
static constexpr int MAX_SIZE = 30;

bool run_trial(double kp, double ki, double kd, double &distance_traveled);

int main()
{
	if (!test_PID_controller())
	{
		std::cout << "PID controller test failed.\n";
		return 1;
	}
/*
	double distance_traveled[MAX_SIZE][MAX_SIZE][MAX_SIZE];
	for(int kp_index = 0; kp_index<MAX_SIZE; ++kp_index)
		for (int ki_index = 0; ki_index<MAX_SIZE; ++ki_index)
			for (int kd_index = 0; kd_index<MAX_SIZE; ++kd_index)
				run_trial(kp_index /30.0, ki_index /1000.0, kd_index /1000.0, distance_traveled[kp_index][ki_index][kd_index]);

	double kp_start = 0;
	double ki_start = 0;
	double kd_start = 0;
	double distance_min = distance_traveled[0][0][0];
	for (int kp_index = 0; kp_index<MAX_SIZE; ++kp_index)
		for (int ki_index = 0; ki_index<MAX_SIZE; ++ki_index)
			for (int kd_index = 0; kd_index<MAX_SIZE; ++kd_index)
				if (distance_traveled[kp_index][ki_index][kd_index] < distance_min)
				{ 
					distance_min = distance_traveled[kp_index][ki_index][kd_index];
					kp_start = kp_index / 30.0;
					ki_start = ki_index / 1000.0;
					kd_start = kd_index / 1000.0;
				}
*/

	std::random_device rd;
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> dis(0.5, 1.0);


	double kp_start = 0.1 *  dis(gen);
	double ki_start = 0.007 *  dis(gen);
	double kd_start = 0.008 *  dis(gen);
	double distance_min;


	double step_size = 1.0;
	double opt[3][3][3];
	for (int step = 0; step < 100; step++)
	{
		for (int kp_index = 0; kp_index < 3; ++kp_index)
			for (int ki_index = 0; ki_index < 3; ++ki_index)
				for (int kd_index = 0; kd_index < 3; ++kd_index)
					run_trial(kp_start + (kp_index - 1)*step_size / 30.0, ki_start + (ki_index - 1) * step_size / 1000.0, kd_start + (kd_index - 1) * step_size / 1000.0, opt[kp_index][ki_index][kd_index]);

		double kp_min = 0;
		double ki_min = 0;
		double kd_min = 0;
		distance_min = opt[0][0][0];
		for (int kp_index = 0; kp_index < 3; ++kp_index)
			for (int ki_index = 0; ki_index < 3; ++ki_index)
				for (int kd_index = 0; kd_index < 3; ++kd_index)
					if (opt[kp_index][ki_index][kd_index] < distance_min)
					{
						distance_min = opt[kp_index][ki_index][kd_index];
						kp_min = kp_start + (kp_index - 1) * step_size / 30.0;
						ki_min = ki_start + (ki_index - 1) * step_size / 1000.0;
						kd_min = kd_start + (kd_index - 1) * step_size / 1000.0;
					}

		kp_start = kp_min;
		ki_start = ki_min;
		kd_start = kd_min;

		step_size /= 1.1;

		std::cout << "minimale Distanz = " << distance_min << std::endl;
		std::cout << "kp = " << kp_start << std::endl;
		std::cout << "ki = " << ki_start << std::endl;
		std::cout << "kd = " << kd_start << std::endl;

	}

	std::cout << "minimale Distanz = "<< distance_min << std:: endl;
	std::cout << "kp = " << kp_start << std::endl;
	std::cout << "ki = " << ki_start << std::endl;
	std::cout << "kd = " << kd_start << std::endl;
    return 0;
}

bool run_trial(double kp, double ki, double kd, double &distance_traveled)
{
	Rocker rock_me_amadeus(0.0, 0.0, 0.5);
	PID_controller controller(kp, ki, kd, -MAX_ANGLE, MAX_ANGLE);

	distance_traveled = 0.0;

	for (int step = 0; step < FIVE_SECONDS; ++step)
	{
		double current_position = rock_me_amadeus.get_position();
		controller.set_process_value(current_position);

		controller.step();
		rock_me_amadeus.step();

		if (!rock_me_amadeus.is_ball_on_rocker())
		{
			distance_traveled = std::numeric_limits<double>::max();
			return false;
		}
		double updated_position = rock_me_amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = -controller.get_control_value();
		rock_me_amadeus.set_angle(angle);

//		std::cout << current_position << " cm\t" << angle << " degree\n";
	}

	// std::cout << "push\n";

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
			distance_traveled = std::numeric_limits<double>::max();
			return false;
		}

		double updated_position = rock_me_amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = controller.get_control_value();
		rock_me_amadeus.set_angle(angle);

//		std::cout << current_position << " cm\t" << angle << " degree\n";
	}

	return true;
}