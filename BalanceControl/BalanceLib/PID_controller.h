#pragma once
class PID_controller
{
public:
	PID_controller();
	PID_controller(double kp, double ki, double kd);
	PID_controller(double kp, double ki, double kd, double lower_saturation_limit, double upper_saturation_limit_);
	PID_controller(double kp, double ki, double kd, double lower_saturation_limit, double upper_saturation_limit_, double kb);
	double get_kp() const;
	double get_ki() const;
	double get_kd() const;
	double get_kb() const;
	bool get_limit_output() const;
	double get_lower_saturation_limit() const;
	double get_upper_saturation_limit() const;
	void set_kp(double kp);
	void set_ki(double ki);
	void set_kd(double kd);
	void set_kb(double kb);
	void set_saturation_limits(double lower_limit, double upper_limit);
	void set_limit_output(bool on);
	void set_process_value(double value);
	double get_control_value() const;
	void step();

private:
	double kp_;
	double ki_;
	double kd_;
	double kb_;
	double y_; // process value
	double y_sum_; // integral over time of process value
	double y_back_;
	double y_prev_; // previous process value
	double u_; // control value
	bool limit_output_;
	double lower_saturation_limit_;
	double upper_saturation_limit_;
};

