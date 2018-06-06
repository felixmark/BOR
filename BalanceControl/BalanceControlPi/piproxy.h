#pragma once

#if _WIN32 || _WIN64
#include "../BalanceControlWin/winpi.h"
#else
#include <wiringPi.h>
#endif