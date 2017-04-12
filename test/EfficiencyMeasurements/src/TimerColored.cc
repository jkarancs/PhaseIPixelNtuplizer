#include "../interface/TimerColored.h"

TimerColored::TimerColored(ConsoleActor& actorArg) : actor(actorArg)
{

}

void TimerColored::restart(std::string text)
{
	std::cout << actor << text << std::endl;
	time(&(this -> startTime));
}

void TimerColored::printSeconds(std::string preText, std::string postText)
{
	time(&currentTime);
	double secondsElapsed = difftime(currentTime, startTime);
	std::cout << actor << preText << secondsElapsed << postText << std::endl;
}