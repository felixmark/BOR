#pragma once
#include "IDigitalInput.h"
#include "piproxy.h"
#include <string>
#include <iostream>
#include <memory>

class Digital_input : public IDigital_input {
private:
	std::string name_;
	int pin_;

public:
	Digital_input();
	Digital_input(std::string name, int pin, int pullUpDown);
	Digital_input(const Digital_input& other) = delete;
	Digital_input& operator =(const Digital_input& other) = delete;
	Digital_input(Digital_input&& other) = default;
	Digital_input& operator =(Digital_input&& other) = default;
	~Digital_input();
	virtual Status read() const override;
	virtual std::string get_name() const override;
	virtual int get_pin() const override;
	friend std::ostream& operator<< (std::ostream& os, const Digital_input& di);
};