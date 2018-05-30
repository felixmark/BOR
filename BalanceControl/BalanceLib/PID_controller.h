#pragma once
class PID_controller
{
public:
	PID_controller(double kp, double ki, double kd);
	double get_kp() const;
	double get_ki() const;
	double get_kd() const;
	void set_kp(double kp);
	void set_ki(double ki);
	void set_kd(double kd);
	void set_process_value(double value);
	double get_control_value(double value) const;
	void step();

private:
	double p_;
	double i_;
	double d_;
};

