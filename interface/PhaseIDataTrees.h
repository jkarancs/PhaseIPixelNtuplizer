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
		static void defineEventTreeBranches(  TTree*& eventTree,   EventData& eventField);
		static void defineClusterTreeBranches(TTree*& clusterTree, EventData& eventField, Cluster& clusterField);
		static void defineTrajTreeBranches(   TTree*& trajTree,    EventData& eventField, TrajMeasurement& trajField);
		static void setEventTreeDataFields(   TTree*& eventTree,   EventData& eventField);
		static void setClusterTreeDataFields( TTree*& clusterTree, EventData& eventField, Cluster& clusterField);
		static void setTrajTreeDataFields(    TTree*& trajTree,    EventData& eventField, TrajMeasurement& trajField);
};

#endif