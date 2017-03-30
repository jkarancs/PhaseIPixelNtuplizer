#ifndef PHASEIPIXELNTUPLIZER_H
#define PHASEIPIXELNTUPLIZER_H

#define ADD_CHECK_PLOTS_TO_NTUPLE

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
#pragma message("ADD_CHECK_PLOTS_TO_NTUPLE defined. Ignore this message if this is the intended behaviour (or if you don't know what this means).")
#include "../interface/common_functions.h"
#else
#pragma message("ADD_CHECK_PLOTS_TO_NTUPLE is NOT defined. No cluster occupancy plots will be generated in the ntuple file.")
#endif

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
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTrackerEvent.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoLocalTracker/Records/interface/TkPixelCPERecord.h"

// Datastructures - Keep all this in one file
// This has to be a versioned file
// It cannot go into separate files included from everywhere
#include "../interface/DataStructures_v4.h" // 2017 Mar 9, CMSSW_9_0_0_pre6

// New class for plotting Phase 0/1 Geometry (Will be added to DQM later)
#include "../interface/SiPixelCoordinates.h"

// ROOT Libraries
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1D.h>
#include <TH2D.h>
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
		int                           clusterSaveDownscaling_;
		TFile*                        ntupleOutputFile_;
		std::vector<std::string>      triggerNames_;
		edm::InputTag                 triggerTag_;
		std::map<uint32_t, int>       federrors_;
		// Trees
		TTree* eventTree_;
		TTree* lumiTree_;
		TTree* runTree_;
		TTree* digiTree_;
		TTree* clustTree_;
		TTree* trackTree_;
		TTree* trajTree_;
		TTree* nonPropagatedExtraTrajTree_;
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
		edm::EDGetTokenT<MeasurementTrackerEvent>                measurementTrackerEventToken_;
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
		edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigiCollectionToken_;
		std::vector<edm::EDGetTokenT<std::vector<PSimHit>>>      simhitCollectionTokens_;
#endif
		// Tools
		SiPixelCoordinates coord_;
		const PixelClusterParameterEstimator* pixelClusterParameterEstimator_;
		const TrackerTopology*                trackerTopology_;
		const TrackerGeometry*                trackerGeometry_;
		const Propagator*                     trackerPropagator_;
		const MeasurementTracker*             measurementTracker_;
		const MeasurementTrackerEvent*        measurementTrackerEvent_;
		const MeasurementEstimator*           chi2MeasurementEstimator_;
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
		// Check plots
		TH2D* simhitOccupancy_fwd;
		TH2D* simhitOccupancy_l1;
		TH2D* simhitOccupancy_l2;
		TH2D* simhitOccupancy_l3;
		TH2D* simhitOccupancy_l4;
		
		TH2D* digiOccupancy_fwd;
		TH2D* digiOccupancy_l1;
		TH2D* digiOccupancy_l2;
		TH2D* digiOccupancy_l3;
		TH2D* digiOccupancy_l4;

		TH2D* clustOccupancy_fwd;
		TH2D* clustOccupancy_l1;
		TH2D* clustOccupancy_l2;
		TH2D* clustOccupancy_l3;
		TH2D* clustOccupancy_l4;

		TH2D* rechitOccupancy_fwd;
		TH2D* rechitOccupancy_l1;
		TH2D* rechitOccupancy_l2;
		TH2D* rechitOccupancy_l3;
		TH2D* rechitOccupancy_l4;

		TH2D* clustOccupancyROCBins_fwd;
		TH2D* clustOccupancyROCBins_l1;
		TH2D* clustOccupancyROCBins_l2;
		TH2D* clustOccupancyROCBins_l3;
		TH2D* clustOccupancyROCBins_l4;
		
		TH2D* rechitOccupancyROCBins_fwd;
		TH2D* rechitOccupancyROCBins_l1;
		TH2D* rechitOccupancyROCBins_l2;
		TH2D* rechitOccupancyROCBins_l3;
		TH2D* rechitOccupancyROCBins_l4;
#endif
		TH1D* disk1PropagationEtaNumhits;
		TH1D* disk1PropagationEtaEfficiency;
		// Private methods
		void                                getEvtData(const edm::Event& iEvent, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<edm::TriggerResults>& triggerResultsHandle, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle);
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
		void                                getSimhitData(const std::vector<edm::Handle<edm::PSimHitContainer>>& simhitCollectionHandles);
		void                                getDigiData(const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollectionHandle);
#endif
		void                                getClustData(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle);
		std::map<reco::TrackRef, TrackData> getTrackData(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle);
		void                                getTrajTrackData(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle);
		void                                checkAndSaveTrajMeasurementData(const TrajectoryMeasurement& measurement, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, TTree* targetTree);
		std::vector<TrajectoryMeasurement>  getLayer1ExtrapolatedHitsFromMeas(const TrajectoryMeasurement& trajMeasurement);
		void                                getDisk1PropagationData(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle);
		void                                handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
		void                                handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
		void                                printEvtInfo(const std::string& streamType);
		void                                getModuleData(ModuleData &mod, bool online, const DetId &detId);
		void                                getRocData(ModuleData &mod, bool online, const DetId &detId, const PixelDigi *digi);
		void                                getRocData(ModuleData &mod, bool online, const DetId &detId, const SiPixelCluster *cluster);
		void                                getRocData(ModuleData &mod, bool online, const SiPixelRecHit *rechit);
	private:
		void                       propagateTrackToLayer1(const edm::Ref<std::vector<Trajectory>>& trajectory, const reco::TrackRef track);
		std::tuple<std::vector<TrajectoryMeasurement>::const_iterator, float> 
			findMatchingTrajMeasurement(const GlobalPoint& referencePoint, const ModuleData& referenceModInfo, const std::vector<TrajectoryMeasurement>& possibleMatches);
		const      SiPixelCluster* getClosestClusterOnDetSetToPoint(const edmNew::DetSet<SiPixelCluster>& clustersOnDet, const LocalPoint& referencePoint);
		float                      trajMeasGlobalPointDistanceSquared(const TrajectoryMeasurement& trajMeasurement, const GlobalPoint& referencePoint);
		float                      clusterPointDistanceSquared(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint);
		LocalPoint                 clusterPointDistanceVector (const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint);
		float                      clusterPointDistance       (const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint);
		void                       printTrackCompositionInfo(const edm::Ref<std::vector<Trajectory>>& trajectory, const reco::TrackRef& track, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle);
};

namespace NtuplizerHelpers 
{
	std::map<uint32_t, int> getFedErrors(const edm::Event& iEvent, const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& rawDataErrorToken);
	bool detidIsOnPixel(const DetId& detid);
	bool areIdenticalModules(const ModuleData& lhs, const ModuleData& rhs);
	int trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>& trajectory);
	reco::VertexCollection::const_iterator findClosestVertexToTrack(const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const unsigned int& minTracks = 0);
	TrajectoryStateOnSurface getTrajectoryStateOnSurface(const TrajectoryMeasurement& measurement);
	std::pair<float, float> getLocalXY(const TrajectoryMeasurement& measurement);
	float trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs);
	void trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distanceSquared, float& dxSquared, float& dySquared);
	void trajMeasurementDistance(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distance, float& dx, float& dy);
	void getClosestOtherTrajMeasurementDistanceByLooping(const TrajectoryMeasurement& measurement, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, float& distance, float& dx, float& dy);
	// int getTrackParentVtxNumTracks(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const reco::TrackRef trackToFind);
} // NtuplizerHelpers

#endif
