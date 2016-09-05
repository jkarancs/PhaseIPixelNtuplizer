#ifndef CLUSTER_H
#define CLUSTER_H

#include "Cluster_structure.hh"
#include "../../ModuleDataFetcher/interface/ModuleData.h"

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class Cluster : public ClustData
{
	public:

		ModuleData mod; // offline module number
		ModuleData mod_on; // online module number

		Cluster() { mod.init(); mod_on.init();};
		void init() 
		{
			ClustData::init();
			mod.init();
			mod_on.init();
		}
	
};

#endif