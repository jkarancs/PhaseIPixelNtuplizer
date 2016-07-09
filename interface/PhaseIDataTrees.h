#ifndef PHASEI_DATA_TREE_H
#define PHASEI_DATA_TREE_H

//////////////////////
// Tree definitions //
//////////////////////

#include "../interface/Tree_structures/Event_tree_structure.hh"
#include "../interface/Tree_structures/Luminosity_tree_structure.hh"
#include "../interface/Tree_structures/Run_structure.hh"
#include "../interface/Tree_structures/Cluster.hh"
#include "../interface/Tree_structures/Traj_measurement.hh"

////////////////////////////
// Message logger service //
////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"

////////////////////
// Root libraries //
////////////////////

#include <TROOT.h>
#include <TTree.h>

class PhaseIDataTrees
{
	public:
		static void define_event_tree_branches(TTree*& event_tree, EventData& event_field);
		static void define_cluster_tree_branches(TTree*& cluster_tree, EventData& event_field, Cluster& cluster_field);
		static void define_traj_tree_branches(TTree*& traj_tree, EventData& event_field, TrajMeasurement& traj_field);
		static void set_event_tree_data_fields(TTree*& event_tree, EventData& event_field);
		static void set_cluster_tree_data_fields(TTree*& cluster_tree, EventData& event_field, Cluster& cluster_field);
		static void set_traj_tree_data_fields(TTree*& traj_tree, EventData& event_field, TrajMeasurement& traj_field);
};

#endif