#ifndef CONSOLE_COLORS_H
#define CONSOLE_COLORS_H

#include <ostream>

namespace Console_color 
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
			~Modifier();
			friend std::ostream& operator <<(std::ostream& os_p, const Modifier& mod_p);
	};
}

extern Console_color::Modifier c_red;
extern Console_color::Modifier c_blue;
extern Console_color::Modifier c_green;
extern Console_color::Modifier c_lgray;
extern Console_color::Modifier c_dgray;
extern Console_color::Modifier c_def;


#endif