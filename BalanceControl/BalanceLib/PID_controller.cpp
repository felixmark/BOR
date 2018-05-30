#include "PID_controller.h"



PID_controller::PID_controller() :kp_{ 1 }, ki_{ 0 }, kd_{ 0 }
{
}

PID_controller::PID_controller(double kp, double ki, double kd):kp_{ kp }, ki_{ ki }, kd_{ kd }
{
}

PID_controller::get_kp() const
{
	return kp_;
}

PID_controller::get_ki() const
{
	return ki_;
}

PID_controller::get_kd() const
{
	return kd_;
}

PID_controller::set_kp(double kp)
{ 
	kp_ = kp;
}

PID_controller::set_ki(double ki)
{
	ki_ = ki;
}

PID_controller::set_kd(double kd)
{
	kd_ = kd;
}

PID_controller::set_process_value(double y)
{
	y_ = y;

}

PID_controller::get_control_value() const
{
	return u_;

}

PID_controller::step()
{
	u_ = -(kp_ * y_ + ki_* STEPSIZE * y_ + kd_*(y_ - y_prev) / STEPSIZE);
	y_prev = y_;

}
