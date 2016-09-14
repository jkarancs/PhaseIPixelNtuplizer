#ifndef PHASEI_TRACKING_EFFICIENCY_FILTERS_H
#define PHASEI_TRACKING_EFFICIENCY_FILTERS_H

//////////////////////
// Tree definitions //
//////////////////////

#include "../interface/Tree_structures/Event_tree_structure.hh"
// #include "../interface/Tree_structures/Luminosity_tree_structure.hh"
// #include "../interface/Tree_structures/Run_structure.hh"
// #include "../interface/Tree_structures/Cluster.hh"
#include "../interface/Tree_structures/Traj_measurement.hh"

class PhaseITrackingEfficiencyFilters
{
	private:
		// Disable default constructor
		PhaseITrackingEfficiencyFilters() = delete;
		// Data fields
		const EventData&       eventField;
		const TrajMeasurement& trajField;
	private:
		int nvtxCut();
		int zerobiasCut();
		int federrCut();
		int hpCut();
		int ptCut();
		int nstripCut();
		int d0Cut();
		int dzCut();
		int pixhitCut();
		int goodModCut();
		int lxFidCut();
		int lyFidCut();
		int valmisCut();
		int hitSepCut();
	public:
		enum Cuts
		{
			nvtx     = 0,
			zerobias = 1 >> 0,
			federr   = 1 >> 1,
			hp       = 1 >> 2,
			pt       = 1 >> 3,
			nstrip   = 1 >> 4,
			d0       = 1 >> 5,
			dz       = 1 >> 6,
			pixhit   = 1 >> 7,
			goodmod  = 1 >> 8,
			lx_fid   = 1 >> 9,
			ly_fid   = 1 >> 10,
			valmis   = 1 >> 11,
			hitsep   = 1 >> 12,
		};
		PhaseITrackingEfficiencyFilters(const EventData& eventField, const TrajMeasurement& trajField);
		~PhaseITrackingEfficiencyFilters();
		bool performCuts(uint32_t cutList);
};

#endif