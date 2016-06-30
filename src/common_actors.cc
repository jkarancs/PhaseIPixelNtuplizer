#include <iostream>

#include "../interface/common_actors.h"

Console_actor process_prompt("Process", c_green);
Console_actor timer_prompt(  "Timer",   c_blue );
Console_actor debug_prompt(  "Debug",   c_lgray);
Console_actor error_prompt(  "Error",   c_red);

void test_prompts_and_colors()
{
	std::cerr << c_red << "AAAARGH!" << c_def << std::endl;
	std::cerr << process_prompt << "process." << std::endl;
	std::cerr << timer_prompt   << "timer." << std::endl;
	std::cerr << debug_prompt   << "debug." << std::endl;
	std::cerr << error_prompt   << "error." << std::endl;
}