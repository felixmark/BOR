#include "stdafx.h"
#include "Rocker.h"
#include <math.h>
#include <iostream>

Rocker::Rocker() : Rocker(50, 0, 0) {
}

Rocker::Rocker(double position, double angle, double ball_speed) : position_{ position }, angle_{ angle }, desired_angle_{ angle }, ball_speed_{ ball_speed }, time_elapsed_{ 0 } {
	if (angle_ > MAX_ANGLE_) {
		angle_ = MAX_ANGLE_;
	}
	else if (angle_ < -MAX_ANGLE_) {
		angle_ = -MAX_ANGLE_;
	}
	desired_angle_ = angle_;
}

void Rocker::step() {
	// Apply super complicated physics
	// a = g * sin(angle)

	// Calculate new angle
	if (desired_angle_ - angle_ > MAX_ANGLE_CHANGE_) {
		angle_ = angle_ + MAX_ANGLE_CHANGE_;
	}
	else if (desired_angle_ - angle_ < -MAX_ANGLE_CHANGE_) {
		angle_ = angle_ - MAX_ANGLE_CHANGE_;
	}
	else {
		angle_ = desired_angle_;
	}
	double rad_angle = (angle_ / 180.0)*PI;

	// Calculate acceleration
	double acceleration = -(GRAVITY * sin(rad_angle));	// m/s^2

														// Calculate new speed and position
	double speed = acceleration * STEPSIZE;				// m/s
	ball_speed_ = ball_speed_ + speed;					// m/s
	double distance = ball_speed_ * STEPSIZE;			// m
	position_ = position_ + distance * 100;				// cm

														// Add elapsed time to local Variable
	time_elapsed_ += STEPSIZE;
}

void Rocker::push_ball(double speed) {
}

void Rocker::set_angle(double angle) {
	if (angle > MAX_ANGLE_) {
		desired_angle_ = MAX_ANGLE_;
	}
	else if (angle < -MAX_ANGLE_) {
		desired_angle_ = -MAX_ANGLE_;
	}
	else {
		desired_angle_ = angle;
	}
}

double Rocker::get_angle() const {
	return angle_;
}

void Rocker::set_position(double position) {
	position_ = position;
}

double Rocker::get_position() const {
	return position_;
}

void Rocker::set_ball_speed(double ball_speed) {
	ball_speed_ = ball_speed;
}

double Rocker::get_ball_speed() const {
	return ball_speed_;
}

double Rocker::get_time_elapsed() const {
	return time_elapsed_;
}

bool Rocker::is_ball_on_rocker() const {
	return (position_ < -100 || position_ > 100) ? false : true;
}

std::ostream & operator<<(std::ostream & os, Rocker & rocker) {
	os << "Angle: " << rocker.get_angle() << " deg" << std::endl;
	os << "Position: " << rocker.get_position() << " cm" << std::endl;
	os << "Speed: " << (rocker.get_ball_speed()) << " m/s" << std::endl;
	os << "Elapsed time: " << rocker.get_time_elapsed() << " s" << std::endl;

	if (rocker.is_ball_on_rocker()) {
		for (int i = 0; i < (rocker.get_position() + 100) / 2; ++i) os << " ";
		os << "O" << std::endl;
		for (int i = 0; i < 100; ++i) os << "-";
	}
	else {
		os << "Fallen off the Rocker!" << std::endl;
	}
	os << std::endl;
	return os;
}
