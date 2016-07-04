#ifndef DIGI_H
#define DIGI_H

#include "Module_structure.hh"
#include "Digi_structure.hh"

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class Digi : public DigiData
{
	public:

	ModuleData mod; // offline module number
	ModuleData mod_on; // online module number
  
	Digi() { mod.init(); mod_on.init(); }
	void init()
	{
		DigiData::init();
		mod.init();
		mod_on.init();
	};
};

#endif