#pragma once
#include "Global_definitions.h"


class Rocker {
private:
	static constexpr double MAX_ANGLE_ = (PI * 180) / 8;
	static constexpr double MAX_ANGLE_CHANGE_ = (PI * 180) / 32;

	double position_;
	double angle_;
	double desired_angle_;
	double ball_speed_;
	double time_elapsed_;

public:
	Rocker();
	Rocker(double position, double angle, double ball_speed);
	
	// =============== COMPLEX PHYSICS ===============
	void step();
	void push_ball(double speed);

	// =============== GETTERS AND SETTERS ===============
	void set_angle(double angle);
	double get_angle() const;
	void set_position(double position);
	double get_position() const;
	void set_ball_speed(double ball_speed);
	double get_ball_speed() const;
	double get_time_elapsed() const;
};

