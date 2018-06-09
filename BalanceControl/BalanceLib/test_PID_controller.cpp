#include <iostream>
#include <cmath>
#include "../BalanceLib/test_PID_controller.h"
#include "../BalanceLib/PID_controller.h"

constexpr double EPSILON = 1E-3;

static double stimulus_values[] = {
	0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
	1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
	1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
	1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
	1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000,
	1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000 };

static double target_values[] = {
	 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
	-1.101,-0.102,-0.103,-0.104,-0.105,-0.106,-0.107,-0.108,-0.109,-0.110,
	-0.111,-0.112,-0.113,-0.114,-0.115,-0.116,-0.117,-0.118,-0.119,-0.120,
	-0.121,-0.122,-0.123,-0.124,-0.125,-0.126,-0.127,-0.128,-0.129,-0.130,
	-0.131,-0.132,-0.133,-0.134,-0.135,-0.136,-0.137,-0.138,-0.139,-0.140,
	-0.141,-0.142,-0.143,-0.144,-0.145,-0.146,-0.147,-0.148,-0.149,-0.150};

bool test_PID_controller(void)
{
	PID_controller controller(0.1, 1.0, 0.001);

	int n_max = sizeof(stimulus_values) / sizeof(double);
	for (int n = 0; n < n_max; ++n) {
		controller.set_process_value(stimulus_values[n]);
		controller.step();
		if (fabs(controller.get_control_value()-target_values[n]) > EPSILON)
			return false;
	}

	return true;
}
