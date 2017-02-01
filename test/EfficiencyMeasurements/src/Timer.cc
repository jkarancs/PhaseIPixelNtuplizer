#include "../interface/Timer.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::restart(std::string  text)
{
	std::cout << text << std::endl;
	time(&(this -> startTime));
}

void Timer::printSeconds(std::string preText, std::string postText)
{
	time(&currentTime);
	secondsElapsed = difftime(currentTime, startTime);
	std::cout << preText << secondsElapsed << postText << std::endl;
}