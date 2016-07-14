#ifndef TRAJ_MEASUREMENT_H
#define TRAJ_MEASUREMENT_H

#include "Module_structure.hh"
#include "Cluster.hh"
#include "Track_structure.hh"
#include "Traj_measurement_structure.hh"

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class TrajMeasurement : public TrajMeasData
{
	public:
		// Extra: TrajMeasData in Traj_measurement_structure.hh
		ModuleData mod; // offline module number
		ModuleData mod_on; // online module number
		// ClustData clu;
		// TrackData trk;

		TrajMeasurement() { mod.init(); mod_on.init(); /*clu.init(); trk.init(); */}
		void init()
		{
			TrajMeasData::init();
			mod.init();
			mod_on.init();
			// clu.init();
			// trk.init();
		};
};

#endif