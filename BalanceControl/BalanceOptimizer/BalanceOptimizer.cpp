// BalanceOptimizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\BalanceLib\PID_controller.h"
#include "..\BalanceLib\test_PID_controller.h"



int main()
{
	if (!test_PID_controller())
		return 1;

    return 0;
}

