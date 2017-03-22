#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <iostream>

class Timer
{
	protected:
		time_t startTime;
		time_t currentTime;
	public:
		Timer();
		~Timer();
		virtual void restart(std::string  text);
		virtual void printSeconds(std::string preText, std::string postText);
		virtual double getSecondsElapsed();
};

#endif
