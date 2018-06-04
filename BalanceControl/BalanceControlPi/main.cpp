#include "../BalanceLib/piproxy.h"
#include <iostream>
#include "../BalanceLib/Rocker.h"
#include <thread>

using namespace std;

int main(void) {
	Rocker rocker(80, 100, 0);

	cout << rocker;
	cout << "Loading complex physics simulation.." << endl;
	std::this_thread::sleep_for(std::chrono::seconds(4));

	for (int n = 0; n < 50; ++n) {
		// Simulate n x 0,5 seconds
		system("cls");
		for (int i = 0; i < 500; ++i) { rocker.step(); }
		cout << rocker;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return 0;
}
