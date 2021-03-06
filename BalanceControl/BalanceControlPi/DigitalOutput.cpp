#include "DigitalOutput.h"
#include "PinManager.h"

Digital_output::Digital_output(std::string name, int pin, Status status) : name_{ name }, pin_{ pin } {
	// Setup
	Pin_manager pm;
	int res_pin = pm.reserve(pin);
	pin_ = res_pin;
	pinMode(pin_, OUTPUT);
	digitalWrite(pin_, status_to_highlow(status));
}

Digital_output::~Digital_output() {
	Pin_manager pm;
	pm.release(pin_);
}

Status Digital_output::read() const {
	return int_to_status(digitalRead(pin_));
}

bool Digital_output::write(Status status) {
	if (status == read()) {
		return false;
	}
	digitalWrite(pin_, status_to_highlow(status));
	return true;
}

std::string Digital_output::get_name() const {
	return name_;
}

int Digital_output::get_pin() const {
	return pin_;
}

std::ostream & operator<<(std::ostream & os, const Digital_output & dout) {
	return os << dout.name_ << " (Pin " << dout.pin_ << "): " << (dout.read() == Status::ON ? "ON" : "OFF");
}
