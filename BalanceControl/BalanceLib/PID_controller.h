#pragma once
class PID_controller
{
public:
	PID_controller();
	PID_controller(double kp, double ki, double kd);
	double get_kp() const;
	double get_ki() const;
	double get_kd() const;
	void set_kp(double kp);
	void set_ki(double ki);
	void set_kd(double kd);
	void set_process_value(double value);
	double get_control_value() const;
	void step();

private:
	double kp_;
	double ki_;
	double kd_;
	double y_; // process value
	double y_prev_; // previous process value
	double u_; // control value
};

