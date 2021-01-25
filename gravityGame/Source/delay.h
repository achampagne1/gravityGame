#pragma once

#include <time.h>

class Delay {
	public:
		void delay(int seconds) {
            // Converting time into milli_seconds 
            int milli_seconds = 1000 * seconds;

            // Storing start time 
            clock_t start_time = clock();

            // looping till required time is not achieved 
            while (clock() < start_time + milli_seconds);
		}
	private:
};