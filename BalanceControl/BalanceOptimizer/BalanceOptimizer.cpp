﻿// BalanceOptimizer.cpp : Defines the entry point for the console application.
#include "../BalanceLib/PID_controller.h"
#include "test_PID_controller.h"
#include "../BalanceLib/Rocker.h"

#include <iostream>
#include <vector>


static constexpr int FIVE_SECONDS = 5000;
static constexpr double MAX_ANGLE = 180.0 / 8;
static constexpr int MAX_SIZE = 30;

bool run_trial(double kp, double ki, double kd, double &distance_traveled);

//Solution Klasse speichert einen Parametersatz für den Regler und die zurückgelegte Distanz.
class Solution
{
public:
	double distance_traveled;
	double kp;
	double ki;
	double kd;
	Solution(double distance_traveled, double kp_, double ki_, double kd_);
};

Solution::Solution(double distance_traveled_, double kp_, double ki_, double kd_) : 
	distance_traveled(distance_traveled_), kp(kp_), ki(ki_), kd(kd_)
{
}


int main()
{
	std::vector<Solution> solutions;

	if (!test_PID_controller())
	{
		std::cout << "PID controller test failed.\n";
		return 1;
	}


//Suche nach Lösung, bei der Ball nicht von Wippe fällt beginnt
	std::cout << "Balance-Optimizer startet. Bitte warten... " << std::endl;
	double distance_traveled;
	for(int kp_index = 0; kp_index<MAX_SIZE; ++kp_index)
		for (int ki_index = 0; ki_index<MAX_SIZE; ++ki_index)
			for (int kd_index = 0; kd_index < MAX_SIZE; ++kd_index)
			{
				run_trial(kp_index / 30.0, ki_index / 3000.0, kd_index / 3000.0, distance_traveled);
				if (distance_traveled < std::numeric_limits<double>::max())
				{
					solutions.push_back(Solution(distance_traveled, kp_index / 30.0, ki_index / 3000.0, kd_index / 3000.0));
				}

			}
//Suche nach optimierter Lösung beginnt mittels Gradientenabstiegsverfahren
	std::cout << "Fast geschafft!! Bitte noch etwas mehr Geduld... " << std::endl;
	std::vector<Solution> optima;
	double step_size = 1.0;

	for (const Solution& s : solutions) 
	{
		double kp_start = s.kp;
		double ki_start = s.ki;
		double kd_start = s.kd;
		double kp_min = 0;
		double ki_min = 0;
		double kd_min = 0;
		double distance_min = 0;

		double opt[3][3][3];
		for (int step = 0; step < 100; step++)
		{
			for (int kp_index = 0; kp_index < 3; ++kp_index)
				for (int ki_index = 0; ki_index < 3; ++ki_index)
					for (int kd_index = 0; kd_index < 3; ++kd_index)
					{
						double kp = kp_start + (kp_index - 1)*step_size / 30.0;
						double ki = ki_start + (ki_index - 1) * step_size / 1000.0;
						double kd = kd_start + (kd_index - 1) * step_size / 1000.0;
						kp = kp < 0.0 ? 0.0 : kp;
						ki = ki < 0.0 ? 0.0 : ki;
						kd = kd < 0.0 ? 0.0 : kd;
						run_trial(kp, ki, kd, opt[kp_index][ki_index][kd_index]);
					}

			kp_min = 0;
			ki_min = 0;
			kd_min = 0;
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
							kp_min = kp_min < 0.0 ? 0.0 : kp_min;
							ki_min = ki_min < 0.0 ? 0.0 : ki_min;
							kd_min = kd_min < 0.0 ? 0.0 : kd_min;
						}

			kp_start = kp_min;
			ki_start = ki_min;
			kd_start = kd_min;

			step_size /= 1.1;
	
		}

		optima.push_back(Solution(distance_min, kp_min, ki_min, kd_min));
	}

	Solution optimum(std::numeric_limits<double>::max(), 0.0, 0.0, 0.0);
	for (const Solution& o : optima)
	{
		if (o.distance_traveled <= optimum.distance_traveled)
		{
			optimum = o;
		}
	}

	std::cout << "minimale Distanz = "<< optimum.distance_traveled << std:: endl;
	std::cout << "kp = " << optimum.kp << std::endl;
	std::cout << "ki = " << optimum.ki << std::endl;
	std::cout << "kd = " << optimum.kd << std::endl;

    return 0; 
}

//Es folgt ein Schubs nach rechts, dann ein Schubs nach links. Zurückgelegte Entfernung wird gemesssen. 
//Return true, wenn Ball auf der Wippe geblieben ist.
//Return false, wenn Ball nicht mehr auf der Wippe ist.
bool run_trial(double kp, double ki, double kd, double &distance_traveled)
{
	Rocker amadeus(0.0, 0.0, 0.5);

	kp = kp < 0.0 ? 0.0 : kp;
	ki = ki < 0.0 ? 0.0 : ki;
	kd = kd < 0.0 ? 0.0 : kd;
	PID_controller controller(kp, ki, kd, -MAX_ANGLE, MAX_ANGLE);

	distance_traveled = 0.0;

	for (int step = 0; step < FIVE_SECONDS; ++step)
	{
		double current_position = amadeus.get_position();
		controller.set_process_value(current_position);

		controller.step();
		amadeus.step();

		if (!amadeus.is_ball_on_rocker())
		{
			distance_traveled = std::numeric_limits<double>::max();
			return false;
		}
		double updated_position = amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = -controller.get_control_value();
		amadeus.set_angle(angle);

	}


	double ball_speed = amadeus.get_ball_speed();
	ball_speed -= 0.5;
	amadeus.set_ball_speed(ball_speed);

	for (int step = 0; step < FIVE_SECONDS; ++step)
	{
		double current_position = amadeus.get_position();
		controller.set_process_value(current_position);

		controller.step();
		amadeus.step();

		if (!amadeus.is_ball_on_rocker())
		{
			distance_traveled = std::numeric_limits<double>::max();
			return false;
		}

		double updated_position = amadeus.get_position();
		distance_traveled += fabs(updated_position - current_position);

		double angle = controller.get_control_value();
		amadeus.set_angle(angle);

	}

	return true;
}