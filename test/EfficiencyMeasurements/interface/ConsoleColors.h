#ifndef CONSOLE_COLORS_H
#define CONSOLE_COLORS_H

#include <ostream>

namespace ConsoleColor 
{
	enum Code
	{
		FG_DGRAY   = 30,
		FG_RED     = 31,
		FG_GREEN   = 32,
		FG_BLUE    = 34,
		FG_LGRAY   = 37,
		FG_DEFAULT = 39,
		BG_RED     = 41,
		BG_GREEN   = 42,
		BG_BLUE    = 44,
		BG_DEFAULT = 49
	};
	class Modifier 
	{
		private:
			Code code;
		public:
			Modifier(Code code_p);
			Modifier(Modifier& other_p);
			~Modifier();
			friend std::ostream& operator <<(std::ostream& osArg, const Modifier& modArg);
	};
}

extern ConsoleColor::Modifier c_red;
extern ConsoleColor::Modifier c_blue;
extern ConsoleColor::Modifier c_green;
extern ConsoleColor::Modifier c_lgray;
extern ConsoleColor::Modifier c_dgray;
extern ConsoleColor::Modifier c_def;

#endif