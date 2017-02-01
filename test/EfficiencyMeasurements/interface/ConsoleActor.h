#ifndef CONSOLE_ACTOR_H
#define CONSOLE_ACTOR_H

#include "../interface/ConsoleColors.h"

#include <string>
#include <ostream>

class ConsoleActor
{
	private:
		std::string name;
		ConsoleColor::Modifier c_modifier;
	public:
		ConsoleActor(std::string nameArg, ConsoleColor::Modifier modifierArg);
		~ConsoleActor();
		friend std::ostream& operator <<(std::ostream& osArg, const ConsoleActor& actorArg);
};

#endif