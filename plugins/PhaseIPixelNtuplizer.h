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

#include "../interface/common_actors.h"

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

		// Tree definitions are in the interface directory

		TTree* event_tree;
		TTree* lumi_tree;
		TTree* run_tree;
		TTree* track_tree;
		TTree* clust_tree;
		TTree* traj_tree;
		TTree* digi_tree;

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