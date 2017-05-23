#ifndef PHASEIPIXELNTUPLIZER_H
#define PHASEIPIXELNTUPLIZER_H

/*
#define ADD_CHECK_PLOTS_TO_NTUPLE

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
#pragma message("ADD_CHECK_PLOTS_TO_NTUPLE defined." \
		" Ignore this message if this is the intended behaviour.")
#else
#pragma message("ADD_CHECK_PLOTS_TO_NTUPLE is NOT defined." \
		" No cluster occupancy plots will be generated in the ntuple file.")
#endif
*/

// CMSSW code
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
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
#include "DataFormats/Common/interface/ConditionsInEdm.h"
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
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
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
#include "../interface/DataStructures_v5.h" // 2017 May 16, CMSSW_9_1_0

// SiPixelCoordinates: new class for plotting Phase 0/1 Geometry
#include "DQM/SiPixelPhase1Common/interface/SiPixelCoordinates.h"

// Helpers to save canvases
#include "../interface/common_functions.h"

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
  edm::ParameterSet iConfig_;
  std::string ntupleOutputFilename_;

  // States
  int isEventFromMc_;

  // Options
  int isCosmicTracking_;
  int clusterSaveDownscaling_;
  int saveDigiTree_;
  int saveTrackTree_;
  int saveNonPropagatedExtraTrajTree_;
  int minVertexSize_;

  // Misc. data
  TFile*                                 ntupleOutputFile_;
  edm::Handle<edm::ConditionsInRunBlock> conditionsInRunBlock_;
  std::vector<std::string>               triggerNames_;
  edm::InputTag                          triggerTag_;
  std::map<uint32_t, int>                federrors_;
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
  edm::EDGetTokenT<std::vector<PileupSummaryInfo>>         pileupSummaryToken_;
  edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigiCollectionToken_;
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  std::vector<edm::EDGetTokenT<std::vector<PSimHit>>>      simhitCollectionTokens_;
#endif
  edm::EDGetTokenT<edm::ConditionsInRunBlock>              conditionsInRunBlockToken_;

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
  void setTriggerTable();

  void getEvtData(const edm::Event&, const edm::Handle<reco::VertexCollection>&,
		  const edm::Handle<edm::TriggerResults>&,
		  const edm::Handle<std::vector<PileupSummaryInfo>>&,
		  const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>&,
		  const edm::Handle<TrajTrackAssociationCollection>&);

  int getTriggerInfo(const edm::Event&, const edm::Handle<edm::TriggerResults>&);

  float getPileupInfo(const edm::Handle<std::vector<PileupSummaryInfo>>&);

  void getDigiData(const edm::Handle<edm::DetSetVector<PixelDigi>>&);

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  void getSimhitData(const std::vector<edm::Handle<edm::PSimHitContainer>>&);
#endif

  void getClustData(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>&);

  std::map<reco::TrackRef, TrackData> getTrackData(const edm::Handle<reco::VertexCollection>&,
						   const edm::Handle<TrajTrackAssociationCollection>&);

  void getTrajTrackData(const edm::Handle<reco::VertexCollection>&,
			const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>&,
			const edm::Handle<TrajTrackAssociationCollection>&);

  void getTrajTrackDataCosmics(const edm::Handle<reco::VertexCollection>&,
			       const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>&,
			       const edm::Handle<TrajTrackAssociationCollection>&);

  void checkAndSaveTrajMeasurementData(const TrajectoryMeasurement&,
				       const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>&,
				       const edm::Handle<TrajTrackAssociationCollection>&,
				       TTree*);

  std::vector<TrajectoryMeasurement> getLayer1ExtrapolatedHitsFromMeas
  (const TrajectoryMeasurement&);

  void getDisk1PropagationData(const edm::Handle<TrajTrackAssociationCollection>&);

  void handleDefaultError(const std::string&, const std::string&, std::string);

  void handleDefaultError(const std::string&, const std::string&, std::vector<std::string>);

  void printEvtInfo(const std::string&);

  void getModuleData(ModuleData&, bool, const DetId&);

  void getRocData(ModuleData&, bool, const DetId&, const PixelDigi*);

  void getRocData(ModuleData&, bool, const DetId&, const SiPixelCluster*);

  void getRocData(ModuleData&, bool, const SiPixelRecHit*);

  void propagateTrackToLayer1(const edm::Ref<std::vector<Trajectory>>&, const reco::TrackRef);

  std::tuple<std::vector<TrajectoryMeasurement>::const_iterator, float>
  findMatchingTrajMeasurement(const GlobalPoint&, const ModuleData&,
			      const std::vector<TrajectoryMeasurement>&);

  const SiPixelCluster* getClosestClusterOnDetSetToPoint(const edmNew::DetSet<SiPixelCluster>&,
							 const LocalPoint&);

  float trajMeasGlobalPointDistanceSquared(const TrajectoryMeasurement&, const GlobalPoint&);

  float clusterPointDistanceSquared(const DetId&, const SiPixelCluster&, const LocalPoint&);

  LocalPoint clusterPointDistanceVector(const DetId&, const SiPixelCluster&, const LocalPoint&);

  float clusterPointDistance(const DetId&, const SiPixelCluster&, const LocalPoint&);

  void printTrackCompositionInfo(const edm::Ref<std::vector<Trajectory>>&,
				 const reco::TrackRef&,
				 const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>,
				 const edm::Handle<reco::VertexCollection>&);
};

namespace NtuplizerHelpers 
{
  std::map<uint32_t, int>
  getFedErrors(const edm::Event&,
	       const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>&);

  bool detidIsOnPixel(const DetId&);

  bool areIdenticalModules(const ModuleData&, const ModuleData&);

  int trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>&);

  reco::VertexCollection::const_iterator
  findClosestVertexToTrack(const reco::TrackRef&,
			   const edm::Handle<reco::VertexCollection>&, const unsigned int&);

  TrajectoryStateOnSurface getTrajectoryStateOnSurface(const TrajectoryMeasurement&);

  std::pair<float, float> getLocalXY(const TrajectoryMeasurement&);

  float trajMeasurementDistanceSquared(const TrajectoryMeasurement&, const TrajectoryMeasurement&);

  void trajMeasurementDistanceSquared(const TrajectoryMeasurement&, const TrajectoryMeasurement&,
				      float&, float&, float&);

  void trajMeasurementDistance(const TrajectoryMeasurement&, const TrajectoryMeasurement&,
			       float&, float&, float&);

  void getClosestOtherTrajMeasurementDistanceByLooping
  (const TrajectoryMeasurement&,
   const edm::Handle<TrajTrackAssociationCollection>&,
   float&, float&, float&);

  // int getTrackParentVtxNumTracks(const edm::Handle<reco::VertexCollection>&, const reco::TrackRef);

} // NtuplizerHelpers

#endif
