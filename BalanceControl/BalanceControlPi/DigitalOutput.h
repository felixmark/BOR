#pragma once
#include "IDigitalOutput.h"
#include "piproxy.h"
#include <string>
#include <iostream>
#include <memory>

class Digital_output : public IDigital_output {
private:
	std::string name_;
	int pin_;

public:
	Digital_output(std::string name, int pin, Status status);
	Digital_output(const Digital_output& other) = delete;
	Digital_output& operator =(const Digital_output& other) = delete;
	Digital_output(Digital_output&& other) = default;
	Digital_output& operator =(Digital_output&& other) = default;
	~Digital_output();
	virtual Status read() const override;
	virtual bool write(Status status) override;
	virtual std::string get_name() const override;
	virtual int get_pin() const override;
	friend std::ostream& operator<< (std::ostream& os, const Digital_output& dout);
};