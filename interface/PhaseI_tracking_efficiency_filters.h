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

class PhaseI_tracking_efficiency_filters
{
	private:
		// Disable default constructor
		PhaseI_tracking_efficiency_filters() = delete;
		// Data fields
		const EventData&       event_field;
		const TrajMeasurement& traj_field;
	private:
		int nvtx_cut();
		int zerobias_cut();
		int federr_cut();
		int hp_cut();
		int pt_cut();
		int nstrip_cut();
		int d0_cut();
		int dz_cut();
		int pixhit_cut();
		int noscan_cut();
		int goodmod_cut();
		int lx_fid_cut();
		int ly_fid_cut();
		int valmis_cut();
		int hitsep_cut();
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
			noscan   = 1 >> 8,
			goodmod  = 1 >> 9,
			lx_fid   = 1 >> 10,
			ly_fid   = 1 >> 11,
			valmis   = 1 >> 12,
			hitsep   = 1 >> 13,
		};
		PhaseI_tracking_efficiency_filters(const EventData& event_field_ref, const TrajMeasurement& traj_field_ref);
		~PhaseI_tracking_efficiency_filters();
		bool perform_cuts(uint32_t cut_list);
};

#endif