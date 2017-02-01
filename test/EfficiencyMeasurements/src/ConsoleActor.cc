#include "../interface/ConsoleActor.h"

ConsoleActor::ConsoleActor(std::string nameArg, ConsoleColor::Modifier modifierArg) : name(nameArg), c_modifier(modifierArg)
{

}

ConsoleActor::~ConsoleActor()
{

}

std::ostream& operator <<(std::ostream& osArg, const ConsoleActor& actorArg) 
{
	return osArg << actorArg.c_modifier << actorArg.name << ": " << c_def;
};
