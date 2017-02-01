#ifndef TIMER_COLORED_H
#define TIMER_COLORED_H

#include "./Timer.h"
#include "../interface/ConsoleActor.h"

class TimerColored : public Timer
{
	private:
		ConsoleActor actor;
	public:
		TimerColored(ConsoleActor& actorArg);
		void restart(std::string text);
		void printSeconds(std::string preText, std::string postText);
};

#endif
