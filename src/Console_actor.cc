#include "../interface/Console_actor.h"

Console_actor::Console_actor(std::string name_p, Console_color::Modifier modifier_p) : name(name_p), c_modifier(modifier_p)
{

}

Console_actor::~Console_actor()
{

}

std::ostream& operator <<(std::ostream& os_p, const Console_actor& actor_p) 
{
	return os_p << actor_p.c_modifier << actor_p.name << ": " << c_def;
};
