#ifndef RUN_STRUCTURE_H
#define RUN_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

#include <string>

class RunData 
{
	public:
		int fill;
		int run;

		std::string list;

		RunData() { init(); };
		void init() 
		{
			fill=NOVAL_I;
			run=NOVAL_I;
			list="fill/I:run";
		};

		void* get(std::string field_name_p)
		{
			if(field_name_p == "fill") { return &(this -> fill); }
			if(field_name_p == "run")  { return &(this -> run); }
			std::cerr << "Error: RunData::get() is unable to find the field: " << field_name_p << ". " << std::endl;
			exit(-1);
		}
};


#endif