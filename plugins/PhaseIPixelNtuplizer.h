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
#include "FWCore/Framework/interface/ESHandle.h"

///////////
// Tools //
///////////

// Trajectory measurements
#include "DataFormats/TrackReco/interface/Track.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"

///////////
// Other //
///////////

// Vertex collection handling
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// To get tokens for pixeldigis, rawdataerrors and clusters 
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

// To get token for tracks
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

//////////////////////
// Tree definitions //
//////////////////////

// Branch definitions are hidden in PhaseIDataTrees.h
#include "../interface/PhaseIDataTrees.h"

// Branch structures are hidden in the Tree_structures directory
#include "../interface/Tree_structures/Event_tree_structure.hh"
#include "../interface/Tree_structures/Luminosity_tree_structure.hh"
#include "../interface/Tree_structures/Run_structure.hh"
#include "../interface/Tree_structures/Cluster.hh"
#include "../interface/Tree_structures/Traj_measurement.hh"

////////////////////////////
// Message logger service //
////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
// Adding some colors :)
#include "../interface/Console_colors.hh"

////////////////////
// Root libraries //
////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1F.h>
// #include <TH2F.h>
#include <TRandom3.h>

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

		/////////////
		// Options //
		/////////////

		int cluster_save_downlscaling;

		/////////////////
		// Output file //
		/////////////////

		// Default: "Ntuple.root"
		std::string ntuple_output_filename = "Ntuple.root";
		TFile*      ntuple_output_file;

		/////////////////
		// Tree system //
		/////////////////

		TTree* event_tree;
		TTree* lumi_tree;
		TTree* run_tree;
		TTree* track_tree;
		TTree* cluster_tree;
		TTree* traj_tree;
		// TTree* digi_tree;

		// Tree field definitions are in the interface directory
		EventData       event_field;
		LumiData        lumi_field;
		TrajMeasurement traj_field;
		Cluster         cluster_field;

		// For handling data collections
		std::vector<TrackData>                    complete_track_collection;                        // Track collection
		std::vector<Cluster>                      complete_cluster_collection;                        // Cluster collection
		std::vector<std::vector<TrajMeasurement>> complete_traj_meas_collection; // Trajectory meaurement collection

		///////////////////////////////////
		// Tokens for accessing the data //
		///////////////////////////////////

		edm::EDGetTokenT<reco::VertexCollection> primary_vertices_token;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > clusters_token;
		edm::EDGetTokenT<TrajTrackAssociationCollection> traj_track_collection_token;
		// edm::EDGetTokenT<TrajTrackAssociationCollection> trackAssociationToken_;

		/////////////////
		// For testing //
		/////////////////

		TRandom3 random;

	public:
		PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig);
		virtual ~PhaseIPixelNtuplizer();
		virtual void beginJob();
		virtual void endJob();
		virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

		/////////////////////////////
		// Event tree field values //
		/////////////////////////////

		void get_nvtx_and_vtx_data(const edm::Event& iEvent); // FIXME: add reco for phase_I

		///////////////////////////////
		// Cluster tree field values //
		///////////////////////////////


		////////////////////
		// Error handling //
		////////////////////

		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::string msg);
		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::vector<std::string> msg);
		void print_evt_info(const std::string& stream_type);

		//////////
		// Test //
		//////////

		virtual void produce_fake_events(const int& num_events);
};

#endif