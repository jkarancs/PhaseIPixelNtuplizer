#include "../interface/ConsoleColors.h"

namespace ConsoleColor 
{
	Modifier::Modifier(Code code_p) : code(code_p) 
	{

	}
	Modifier::~Modifier()
	{

	}
	std::ostream& operator <<(std::ostream& os_p, const Modifier& mod_p) 
	{
		return os_p << "\033[" << mod_p.code << "m";
	};
}

ConsoleColor::Modifier c_red(  ConsoleColor::FG_RED    );
ConsoleColor::Modifier c_blue( ConsoleColor::FG_BLUE   );
ConsoleColor::Modifier c_green(ConsoleColor::FG_GREEN  );
ConsoleColor::Modifier c_lgray(ConsoleColor::FG_LGRAY  );
ConsoleColor::Modifier c_dgray(ConsoleColor::FG_DGRAY  );
ConsoleColor::Modifier c_def(  ConsoleColor::FG_DEFAULT);