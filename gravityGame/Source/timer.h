#pragma once

#include <thread>

class Timer {
private:
	std::chrono::system_clock::time_point end;
public:
	Timer();
	void startTimer(int time);
	bool checkTimer();
};
