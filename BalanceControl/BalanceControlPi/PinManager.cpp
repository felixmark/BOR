#include "PinManager.h"

std::vector<int> Pin_manager::reserved_pins_{};

Pin_manager::Pin_manager() {}

int Pin_manager::reserve(int pin) {
	std::vector<int>::iterator position = std::find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	if (position != reserved_pins_.end()) {
		throw std::runtime_error("Pin was already reserved!");
		return -1;
	} else {
		reserved_pins_.push_back(pin);
		return pin;
	}
}

void Pin_manager::release(int pin) {
	std::vector<int>::iterator position = std::find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	if (position != reserved_pins_.end())
		reserved_pins_.erase(position);
	else  throw std::runtime_error("Pin was not reserved!");
}
