#ifndef PHASEIPIXELNTUPLIZER_H
#define PHASEIPIXELNTUPLIZER_H

// CMSSW code
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"
//#include "DataFormats/TrackReco/interface/Track.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "RecoLocalTracker/Records/interface/TkPixelCPERecord.h"

// Datastructures - Keep all this in one file
// This has to be a versioned file
// It cannot go into separate files included from everywhere
#include "../interface/DataStructures_v3.h" // 2016 Dec 16, CMSSW_8_1_0

// New class for plotting Phase 0/1 Geometry (Will be added to DQM later)
#include "../interface/SiPixelCoordinates.h"

// ROOT Libraries
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1D.h>
// #include <TH2D.h>
#include <TRandom3.h>

// C++
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Compiler directives
#define EDM_ML_LOGDEBUG
#define ML_DEBUG

class PhaseIPixelNtuplizer : public edm::EDAnalyzer
{
	private:

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
	private:
		std::string ntupleOutputFilename_;
		edm::ParameterSet iConfig_;
		// States
		int isEventFromMc_ = -1;
		// Options
		int                      clusterSaveDownscaling_;
		TFile*                   ntupleOutputFile_;
		std::vector<std::string> triggerNames_;
		edm::InputTag            triggerTag_;
		// Trees
		TTree* eventTree_;
		TTree* lumiTree_;
		TTree* runTree_;
		TTree* digiTree_;
		TTree* clustTree_;
		TTree* trackTree_;
		TTree* trajTree_;
		// Tree field definitions are in the interface directory
		EventData       evt_;
		LumiData        lumi_;
		RunData         run_;
		Digi            digi_;
		Cluster         clu_;
		TrackData       track_;
		TrajMeasurement traj_;
		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken_;
		edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken_;
		edm::EDGetTokenT<edm::TriggerResults>                    triggerResultsToken_;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken_;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken_;
		// Tools
		SiPixelCoordinates coord_;
		const PixelClusterParameterEstimator* pixelClusterParameterEstimator_;
		const TrackerTopology*                trackerTopology_;
		const TrackerGeometry*                trackerGeometry_;
		// Private methods
		void                                getEvtInfo(const edm::Event& iEvent, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<edm::TriggerResults>& triggerResultsHandle, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle);
		void                                getClustInfo(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const std::map<uint32_t, int>& federrors);
		void                                getTrajTrackInfo(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const std::map<uint32_t, int>& federrors);
		std::map<reco::TrackRef, TrackData> getTrackInfo(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const std::map<uint32_t, int>& federrors);
		void                                handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
		void                                handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
		void                                printEvtInfo(const std::string& streamType);
		void                                getModuleData(ModuleData&, bool, const DetId &, const std::map<uint32_t, int> &);
		void                                getRocData(ModuleData&, bool, const DetId &, const PixelDigi*);
		void                                getRocData(ModuleData&, bool, const DetId &, const SiPixelCluster*);
		void                                getRocData(ModuleData&, bool, const SiPixelRecHit*);
};

namespace NtuplizerHelpers 
{
	std::map<uint32_t, int> getFedErrors(const edm::Event& iEvent, const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& rawDataErrorToken);
	bool detidIsOnPixel(const DetId& detid);
	int trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>& trajectory);
	reco::VertexCollection::const_iterator findClosestVertexToTrack(const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const unsigned int& minTracks = 0);
	std::pair<float, float> getLocalXY(const TrajectoryMeasurement& measurement);
	float trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs);
	void trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distanceSquared, float& dxSquared, float& dySquared);
	void trajMeasurementDistance(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distance, float& dx, float& dy);
	void getClosestOtherTrackDistanceByLooping(const TrajectoryMeasurement& measurement, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, float& distance, float& dx, float& dy);
	// int getTrackParentVtxNumTracks(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const reco::TrackRef trackToFind);
} // NtuplizerHelpers

#endif
