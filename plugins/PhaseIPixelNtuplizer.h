#ifndef	PHASEIPIXELNTUPLIZER_H
#define	PHASEIPIXELNTUPLIZER_H

//////////////////////////
// EDM plugin libraries //
//////////////////////////

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
// #include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

///////////
// Other //
///////////

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
#include <TFile.h>
#include <TTree.h>
// #include <TH1F.h>
// #include <TH2F.h>

////////////////
// C++ system //
////////////////

#include <iostream>
#include <string>
#include <vector>

class PhaseIPixelNtuplizer : public edm::EDAnalyzer
{
	private:
		edm::ParameterSet iConfig;

		/////////////////
		// Output file //
		/////////////////

		// Default: "Ntuple.root"

		std::string ntuple_output_filename;
		TFile*      ntuple_output_file;

		/////////////////
		// Tree system //
		/////////////////

		TTree* event_tree;
		TTree* lumi_tree;
		TTree* run_tree;
		TTree* track_tree;
		TTree* clust_tree;
		TTree* traj_tree;
		TTree* digi_tree;

		// Tree field definitions are in the interface directory

		EventData event_field;
		LumiData lumi_field;
		std::vector<TrackData> track_list_field;
		std::vector<Cluster> clust_list_field;
		std::vector<std::vector<TrajMeasurement>> traj_measurement_list_field;

	public:
		PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig);
		virtual ~PhaseIPixelNtuplizer();
		virtual void beginJob();
		virtual void endJob();
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
};

#endif