#include "../interface/Console_colors.h"

namespace Console_color 
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

Console_color::Modifier c_red(  Console_color::FG_RED    );
Console_color::Modifier c_blue( Console_color::FG_BLUE   );
Console_color::Modifier c_green(Console_color::FG_GREEN  );
Console_color::Modifier c_lgray(Console_color::FG_LGRAY  );
Console_color::Modifier c_dgray(Console_color::FG_DGRAY  );
Console_color::Modifier c_def(  Console_color::FG_DEFAULT);