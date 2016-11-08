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
// #include "DataFormats/TrackReco/interface/Track.h"
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

// Fed errors
#include "../FedErrorFetcher/interface/FedErrorFetcher.h"
// Module data
#include "../ModuleDataFetcher/interface/ModuleDataProducer.h"
// Traj analyzer
#include "../TrajAnalyzer/interface/TrajAnalyzer.h"

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
#include "../interface/PhaseITrackingEfficiencyFilters.h"

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

		// Save only every nth cluster
		int clusterSaveDownlscaling;

		/////////////////
		// Output file //
		/////////////////

		// Default: "Ntuple.root"
		std::string ntupleOutputFilename = "Ntuple.root";
		TFile*      ntupleOutputFile;

		/////////////////
		// Tree system //
		/////////////////

		TTree* eventTree;
		TTree* lumiTree;
		TTree* runTree;
		TTree* trackTree;
		TTree* clusterTree;
		TTree* trajTree;
		// TTree* digiTree;

		// Tree field definitions are in the interface directory
		EventData       eventField;
		LumiData        lumiField;
		TrajMeasurement trajField;
		Cluster         clusterField;

		// For handling data collections
		std::vector<TrackData>                    completeTrackCollection;                        // Track collection
		std::vector<Cluster>                      completeClusterCollection;                        // Cluster collection
		std::vector<std::vector<TrajMeasurement>> completeTrajMeasCollection; // Trajectory meaurement collection

		///////////////////////////////////
		// Tokens for accessing the data //
		///////////////////////////////////

		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken;
		edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken;
		// edm::EDGetTokenT<TrajTrackAssociationCollection> trackAssociationToken_;

		//////////
		// Data //
		//////////

		// Event data
		void getNvtxAndVtxData(const edm::Event& iEvent); // FIXME: add reco for phase_I
		// Clusters
		void handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors);
		// Trajectory measurements
		int  trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>& trajectory);
		void handleTrajMeasurements(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors);
		void getTrackData(const edm::Ref<std::vector<Trajectory>>& traj, const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors);
		void getHitEfficiencyCuts();

		////////////////////
		// Error handling //
		////////////////////

		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
		void printEvtInfo(const std::string& streamType);

		/////////////
		// Utility //
		/////////////

		void clearAllContainers();

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