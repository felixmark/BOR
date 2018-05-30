#include "Rocker.h"
#include <math.h>

Rocker::Rocker() : Rocker(50, 0, 0) {
}

Rocker::Rocker(double position, double angle, double ball_speed) : position_{ position }, angle_{ angle }, desired_angle_{ angle }, ball_speed_ { ball_speed } {
	/* Constructor with Initialisation List */
}

void Rocker::step() {
	// Apply super complicated physics
	// a = g * sin(angle)

	// Calculate new angle
	if (desired_angle_ > MAX_ANGLE_CHANGE_) {
		angle_ = angle_ + MAX_ANGLE_CHANGE_;
	} else if (desired_angle_ < -MAX_ANGLE_CHANGE_) {
		angle_ = angle_ - MAX_ANGLE_CHANGE_;
	} else {
		angle_ = desired_angle_;
	}

	// Calculate a, v and d
	double acceleration = GRAVITY * sin(angle_);
	double speed = acceleration * STEPSIZE;
	double distance = speed * STEPSIZE;

	// Set new Values
	ball_speed_ = ball_speed_ + speed;
	position_ = position_ + distance;

	// Add elapsed time to local Variable
	time_elapsed_ += STEPSIZE;
}

void Rocker::push_ball(double speed) {
}

void Rocker::set_angle(double angle) {
	if (angle > MAX_ANGLE_) {
		desired_angle_ = MAX_ANGLE_;
	} else if (angle < -MAX_ANGLE_) {
		desired_angle_ = -MAX_ANGLE_;
	} else {
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
