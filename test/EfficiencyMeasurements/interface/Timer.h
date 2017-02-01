#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <iostream>

class Timer
{
	protected:
		time_t startTime;
		time_t currentTime;
		double secondsElapsed;	
	public:
		Timer();
		~Timer();
		virtual void restart(std::string  text);
		virtual void printSeconds(std::string preText, std::string postText);
};

#endif
