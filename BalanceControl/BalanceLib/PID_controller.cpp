#include <limits>
#include <stdexcept>
#include "PID_controller.h"
#include "Global_definitions.h"

using namespace std;

static constexpr double EPSILON = 1E-6;

PID_controller::PID_controller() : kp_{ 1 }, ki_{ 0 }, kd_{ 0 }, kb_{ 0 }, y_sum_{ 0 }, y_prev_{ 0 },
	limit_output_{ false }, lower_saturation_limit_{ std::numeric_limits<double>::lowest() }, upper_saturation_limit_{ std::numeric_limits<double>::max() }
{
}

PID_controller::PID_controller(double kp, double ki, double kd) : kp_{ kp }, ki_{ ki }, kd_{ kd }, kb_{ 0 }, y_sum_{ 0 }, y_prev_{ 0 },
	limit_output_{false}, lower_saturation_limit_{ std::numeric_limits<double>::lowest() }, upper_saturation_limit_{ std::numeric_limits<double>::max() }
{
	if (kp_ < -EPSILON || ki_ < -EPSILON || kd_ < -EPSILON)
		throw range_error("Some constant out of range.");
}

PID_controller::PID_controller(double kp, double ki, double kd, double lower_saturation_limit, double upper_saturation_limit) : kp_{ kp }, ki_{ ki }, kd_{ kd }, kb_{ 0 }, y_sum_{ 0 }, y_prev_{ 0 },
	limit_output_{ true }, lower_saturation_limit_{ lower_saturation_limit }, upper_saturation_limit_{ upper_saturation_limit }
{
	if(kp_ < -EPSILON || ki_ < -EPSILON || kd_ < -EPSILON)
		throw range_error("Some constant out of range.");
	if (lower_saturation_limit_ > upper_saturation_limit_)
		throw range_error("Lower saturation limit greater than upper saturation limit.");
}

PID_controller::PID_controller(double kp, double ki, double kd, double lower_saturation_limit, double upper_saturation_limit, double kb) : kp_{ kp }, ki_{ ki }, kd_{ kd }, kb_{ kb }, y_sum_{ 0 }, y_prev_{ 0 },
limit_output_{ true }, lower_saturation_limit_{ lower_saturation_limit }, upper_saturation_limit_{ upper_saturation_limit }
{
	if (kp_ < -EPSILON || ki_ < -EPSILON || kd_ < -EPSILON || kb_ < -EPSILON)
		throw range_error("Some constant out of range.");
	if (lower_saturation_limit_ > upper_saturation_limit_)
		throw range_error("Lower saturation limit greater than upper saturation limit.");
}

double PID_controller::get_kp() const
{
	return kp_;
}

double PID_controller::get_ki() const
{
	return ki_;
}

double PID_controller::get_kd() const
{
	return kd_;
}

double PID_controller::get_kb() const
{
	return kb_;
}

void PID_controller::set_kp(double kp)
{ 
	if (kp_ < -EPSILON)
		throw range_error("Constant kp out of range.");
	kp_ = kp;
}

void PID_controller::set_ki(double ki)
{
	if (ki_ < -EPSILON)
		throw range_error("Constant ki out of range.");
	ki_ = ki;
}

void PID_controller::set_kd(double kd)
{
	if (kd_ < -EPSILON)
		throw range_error("Constant kd out of range.");
	kd_ = kd;
}

void PID_controller::set_kb(double kb)
{
	if (kb_ < -EPSILON)
		throw range_error("Constant kb out of range.");
	kb_ = kb;
}

void PID_controller::set_process_value(double y)
{
	y_ = y;
}

double PID_controller::get_control_value() const
{
	return u_;
}

bool PID_controller::get_limit_output() const
{
	return limit_output_;
}

double PID_controller::get_lower_saturation_limit() const
{
	return lower_saturation_limit_;
}

double PID_controller::get_upper_saturation_limit() const
{
	return upper_saturation_limit_;
}

void PID_controller::set_saturation_limits(double lower_limit, double upper_limit)
{
	lower_saturation_limit_ = lower_limit;
	upper_saturation_limit_ = upper_limit;
	if (lower_saturation_limit_ > upper_saturation_limit_)
		throw range_error("Lower saturation limit greater than upper saturation limit.");
}

void PID_controller::set_limit_output(bool on)
{
	limit_output_ = on;
}

void PID_controller::step()
{
	y_sum_ += STEPSIZE * (ki_* y_ + kb_* y_back_);
	double y_slope = kd_ * (y_ - y_prev_) / STEPSIZE;
	u_ = -(kp_ * y_ +  y_sum_  + y_slope);
	y_back_ = 0.0;
	if (limit_output_)
	{
		if (u_ > upper_saturation_limit_)
		{
			y_back_ = upper_saturation_limit_ - u_;
			u_ = upper_saturation_limit_;
		}
		else if (u_ < lower_saturation_limit_)
		{
			y_back_ = lower_saturation_limit_ - u_;
			u_ = lower_saturation_limit_;
		}
	}
	y_prev_ = y_;
}
