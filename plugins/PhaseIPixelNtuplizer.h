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
// Position
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

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

// To get token for FED errors
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"

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

////////////////////////////////
// Hit efficiency measurement //
////////////////////////////////

// Cuts for hit efficicency
#include "../interface/PhaseI_tracking_efficiency_filters.h"

////////////////////////////
// Message logger service //
////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
// Adding some colors :)
#include "../interface/Console_colors.h"

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

		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError> > raw_data_error_token;

		//////////
		// Data //
		//////////

		// FED errors
		std::map<uint32_t, int> get_FED_errors(const edm::Event& iEvent);
		// Event data
		void get_nvtx_and_vtx_data(const edm::Event& iEvent); // FIXME: add reco for phase_I
		// Module data
		ModuleData get_offline_module_data(const uint32_t& rawId, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors);
		ModuleData convert_offline_to_online_module_data(const ModuleData mod_off);
		int convert_offline_module_to_online_module_coordinate(const int& module);
		int convert_offline_ladder_to_online_ladder_coordinate(const int& off_ladder, const int& quarter_size);
		int convert_offline_disk_to_online_disk_coordinate(const int& side, const int& disk);
		int convert_offline_blade_to_online_blade_coordinate(const int& blade);
		// Clusters
		void get_clusters(const edm::Event& iEvent, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors);
		// Trajectory measurements
		int  trajectory_has_pixel_hit(const edm::Ref<std::vector<Trajectory>>& trajectory);
		void get_traj_measurements(const edm::Event& iEvent, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors);
		void get_track_data();
		void get_hit_efficiency_cuts();

		////////////////////
		// Error handling //
		////////////////////

		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::string msg);
		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::vector<std::string> msg);
		void print_evt_info(const std::string& stream_type);

		/////////////
		// Utility //
		/////////////

		void clear_all_containers();

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
};

#endif