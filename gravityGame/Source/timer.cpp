#include "timer.h"

Timer::Timer() {
}

void Timer::startTimer(int time) {
	end = std::chrono::system_clock::now() + std::chrono::milliseconds(time);
}

bool Timer::checkTimer() {
	if (std::chrono::system_clock::now() >= end)
		return true;
	else
		return false;
}