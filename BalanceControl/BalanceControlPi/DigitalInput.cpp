#include "DigitalInput.h"
#include "PinManager.h"

Digital_input::Digital_input(std::string name, int pin, int pullUpDown) : name_{ name }, pin_{ pin } {
	// Setup Pin
	Pin_manager pm;
	int res_pin = pm.reserve(pin);
	pin_ = res_pin;
	pinMode(pin_, INPUT);
	pullUpDnControl(pin_, pullUpDown);
}

Digital_input::~Digital_input() {
	Pin_manager pm;
	pm.release(pin_);
}

Status Digital_input::read() const {
	return int_to_status(digitalRead(pin_));
}

std::string Digital_input::get_name() const {
	return name_;
}

int Digital_input::get_pin() const {
	return pin_;
}

std::ostream & operator<<(std::ostream & os, const Digital_input & di) {
	return os << di.name_ << " (Pin " << di.pin_ << "): " << (di.read() == Status::ON ? "ON" : "OFF");
}
