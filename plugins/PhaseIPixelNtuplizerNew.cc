// CMSSW code
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
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
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"

// Datastructures - Keep all this in one file
// This has to be a versioned file
// It cannot go into separate files included from everywhere
#include "../interface/DataStructures_v1.h" // 2016 Nov 04, CMSSW_8_1_0_pre15

// Private codes
#include "../TrajAnalyzer/interface/TrajAnalyzer.h" // Do we really need this? Can't we just copy the methods here?
#include "../interface/PhaseITrackingEfficiencyFiltersNew.h"
#include "../interface/Console_colors.h"

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



class PhaseIPixelNtuplizerNew : public edm::EDAnalyzer {
public:
  PhaseIPixelNtuplizerNew(edm::ParameterSet const& iConfig);
  virtual ~PhaseIPixelNtuplizerNew();
  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

private:
  edm::ParameterSet iConfig_;

  // Options
  int clusterSaveDownlscaling;
  std::string ntupleOutputFilename = "Ntuple.root";
  TFile*      ntupleOutputFile;

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
  edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken;
  edm::EDGetTokenT<reco::VertexCollection>                 primaryVerticesToken;
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken;
  edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken;
  // edm::EDGetTokenT<TrajTrackAssociationCollection> trackAssociationToken_;

 
  // Private methods
  void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
  void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
  void printEvtInfo(const std::string& streamType);

  void getModuleData(ModuleData&, bool, const TrackerGeometry* const, const DetId &, const TrackerTopology* const, const std::map<uint32_t, int> &);

};


//---------------------------------------------------------
//                Constructor, Destructor

PhaseIPixelNtuplizerNew::PhaseIPixelNtuplizerNew(edm::ParameterSet const& iConfig) {
  iConfig_ = iConfig;
  clusterSaveDownlscaling = 1;

  primaryVerticesToken     = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
  clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
  //clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClustersPreSplitting"));
  trajTrackCollectionToken = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
  rawDataErrorToken        = consumes<edm::DetSetVector<SiPixelRawDataError> >(edm::InputTag("siPixelDigis"));
}

PhaseIPixelNtuplizerNew::~PhaseIPixelNtuplizerNew() {}



//---------------------------------------------------------
//                      begin/endJob

void PhaseIPixelNtuplizerNew::beginJob() {
  if(iConfig_.exists("fileName")) ntupleOutputFilename = iConfig_.getParameter<std::string>("filename");

  // Create output file
  ntupleOutputFile = new TFile(ntupleOutputFilename.c_str(), "RECREATE");
  if(!(ntupleOutputFile->IsOpen()))
    handleDefaultError("file_operations", "file_operations", { "Failed to open output file: ", ntupleOutputFilename });
  LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename << "\" created." << std::endl;

  // Tree definitions
  // Event tree
  eventTree_   = new TTree("eventTree", "The event");
  eventTree_->Branch("event",      &evt_,           evt_.list.c_str());

  // Cluster tree
  clustTree_ = new TTree("clustTree", "Pixel clusters");
  clustTree_->Branch("event",      &evt_,           evt_.list.c_str());
  clustTree_->Branch("mod_on",     &clu_.mod_on,    clu_.mod_on.list.c_str());
  clustTree_->Branch("mod",        &clu_.mod,       clu_.mod.list.c_str());
  clustTree_->Branch("clust",      &clu_,           clu_.list.c_str());

  // Track tree
  trackTree_ = new TTree("trackTree", "The track in the event");
  trackTree_->Branch("event",      &evt_,           evt_.list.c_str());
  trackTree_->Branch("track",      &track_,         track_.list.c_str());

  // Trajectory tree
  trajTree_    = new TTree("trajTree",   "Trajectory measurements in the Pixel");
  trajTree_ ->Branch("event",      &evt_,           evt_.list.c_str());
  trajTree_ ->Branch("mod_on",     &traj_.mod_on,   traj_.mod_on.list.c_str());
  trajTree_ ->Branch("mod",        &traj_.mod,      traj_.mod.list.c_str());
  trajTree_ ->Branch("clust",      &traj_.clu,      traj_.clu.list.c_str());
  trajTree_ ->Branch("track",      &track_,         track_.list.c_str());
  trajTree_ ->Branch("traj",       &traj_,          traj_.list.c_str());

}

void PhaseIPixelNtuplizerNew::endJob() {
  LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename << "\"." << std::endl;
  ntupleOutputFile->Write();
  ntupleOutputFile->Close();
}



//---------------------------------------------------------
//          begin/endRun, begin/endLuminosityBlock

void PhaseIPixelNtuplizerNew::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizerNew::endRun  (edm::Run const& iRun, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizerNew::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizerNew::endLuminosityBlock  (edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {}



//---------------------------------------------------------
//                       analyze

void PhaseIPixelNtuplizerNew::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  LogDebug("step") << "Executing PhaseIPixelNtuplizerNew::analyze()..." << std::endl;

  // Get vertices
  edm::Handle<reco::VertexCollection> vertexCollectionHandle;
  iEvent.getByToken(primaryVerticesToken, vertexCollectionHandle);

  // Get cluster collection
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
  iEvent.getByToken(clustersToken, clusterCollectionHandle);

  // Get Traj-Track Collection
  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  iEvent.getByToken(trajTrackCollectionToken, trajTrackCollectionHandle);

  // TrackerTopology and TrackerGeometry for module informations
  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
  const TrackerTopology* const tTopo = tTopoHandle.product();
  edm::ESHandle<TrackerGeometry> tkgeom;
  iSetup.get<TrackerDigiGeometryRecord>().get( tkgeom );
  const TrackerGeometry* const tkGeom = tkgeom.product();

  // FED errors
  edm::Handle<edm::DetSetVector<SiPixelRawDataError>> siPixelRawDataErrorCollectionHandle;
  iEvent.getByToken(rawDataErrorToken,                siPixelRawDataErrorCollectionHandle);

  std::map<uint32_t, int> federrors;
  if(siPixelRawDataErrorCollectionHandle.isValid()) for(const auto& pixel_error_set: *siPixelRawDataErrorCollectionHandle)
    for(const auto& pixel_error: pixel_error_set) if(pixel_error_set.detId()!=0xffffffff) {
      DetId detId(pixel_error_set.detId());
      int type = pixel_error.getType();
      federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
      // if(type>24&&type<=40) federr[type-25]++;
      // else edm::LogError("data_access") << "New FED error with not recognised error-type: " << type << std::endl;
    }

  //__________________________________________________________
  //                      Event Infos



  // Set data holder object
  evt_.init();
  evt_.fill         = 0; // FIXME
  evt_.run          = iEvent.id().run();
  evt_.ls           = iEvent.luminosityBlock();
  evt_.orb          = iEvent.orbitNumber();
  evt_.bx           = iEvent.bunchCrossing();
  evt_.evt          = iEvent.id().event();

  // Loop on vertices
  evt_.nvtx               = 0;
  evt_.vtxntrk = 0;
  for(const auto& current_vertex: *vertexCollectionHandle) {
    // Invalid vertex
    if(!current_vertex.isValid()) continue;
    // Check if it is the best vertex (largest trk number, preferably in the middle
    if (current_vertex.tracksSize() > (size_t)evt_.vtxntrk ||
        ( current_vertex.tracksSize() ==(size_t)evt_.vtxntrk &&
          std::abs(current_vertex.z()) < std::abs(evt_.vtxZ) ) ) {
      evt_.vtxntrk = current_vertex.tracksSize();
      evt_.vtxD0   = current_vertex.position().rho();
      evt_.vtxX    = current_vertex.x();
      evt_.vtxY    = current_vertex.y();
      evt_.vtxZ    = current_vertex.z();
      evt_.vtxndof = current_vertex.ndof();
      evt_.vtxchi2 = current_vertex.chi2();
      //primary_vtx         = static_cast<reco::VertexCollection::const_iterator>(&current_vertex);
    }
    // Counting the good vertices
    if(std::abs(current_vertex.z())<24.0 && std::abs(current_vertex.position().rho())<2.0 && current_vertex.ndof() >= 4)
      evt_.nvtx++;
  }

  // Quick loop to get number of clusters on layers/disk
  for (int i=0; i<7; ++i) evt_.nclu[i]=evt_.npix[i]=0;
  if(clusterCollectionHandle.isValid()) {
    for (const auto& clu_set : *clusterCollectionHandle) {
      int layDiskIndex = -1;
      DetId detId(clu_set.id());
      if      (detId.subdetId() == PixelSubdetector::PixelBarrel) layDiskIndex = tTopo->pxbLayer(detId.rawId())-1;
      else if (detId.subdetId() == PixelSubdetector::PixelEndcap) layDiskIndex = tTopo->pxfDisk(detId.rawId())+3;
      else continue;
      evt_.nclu[layDiskIndex] += clu_set.size();
      for (const auto& clu : clu_set) evt_.npix[layDiskIndex] += clu.size();
    }
  }

  // Quick loop to get track numbers
  evt_.ntracks = 0;
  for (int i=0; i<4; ++i) evt_.ntrackBPix[i]=evt_.ntrackBPixvalid[i]=0;
  for (int i=0; i<3; ++i) evt_.ntrackFPix[i]=evt_.ntrackFPixvalid[i]=0;
  for(const auto& pair : *trajTrackCollectionHandle) {
    const edm::Ref<std::vector<Trajectory>> traj  = pair.key;
    const reco::TrackRef                    track = pair.val;
    // Discarding tracks without pixel measurements
    if(!TrajAnalyzer::trajectoryHasPixelHit(traj)) continue;
    ++evt_.ntracks;
    for(const auto& meas : traj->measurements()) {
      if(!meas.updatedState().isValid()) continue;
      DetId detId(meas.recHit()->geographicalId());
      if      (detId.subdetId() == PixelSubdetector::PixelBarrel) {
	evt_.ntrackBPix[tTopo->pxbLayer(detId.rawId())-1]++;
	if (meas.recHit()->getType() == TrackingRecHit::valid)
	  evt_.ntrackBPixvalid[tTopo->pxbLayer(detId.rawId())-1]++;
      } else if (detId.subdetId() == PixelSubdetector::PixelEndcap) {
	evt_.ntrackFPix[tTopo->pxfDisk(detId.rawId())-1]++;
	if (meas.recHit()->getType() == TrackingRecHit::valid)
	  evt_.ntrackFPixvalid[tTopo->pxfDisk(detId.rawId())-1]++;
      }
    }
  }

  /* ToDo or remove
     evt_.trig               = 0;
     evt_.beamint            = 0;
     evt_.l1_rate            = 0;
     evt_.intlumi            = 0;
     evt_.instlumi           = 0;
     evt_.instlumi_ext       = 0;
     evt_.pileup             = 0;
     evt_.weight             = 0;
     evt_.good               = 0;
     evt_.tmuon              = 0;
     evt_.tmuon_err          = 0;
     evt_.tecal              = 0;
     evt_.tecal_raw          = 0;
     evt_.tecal_err          = 0;
     evt_.field              = 0;
     evt_.wbc                = 0;
     evt_.delay              = 0;
     evt_.trackSep           = 0;
     evt_.federrs_size       = 0;
     evt_.federrs[][]        = 0;
  */

  // Fill the tree
  eventTree_->Fill();



  //__________________________________________________________
  //                       Cluster Infos

  // Trying to access the clusters
  clu_.init();
  if(clusterCollectionHandle.isValid()) {
    int clusterCounter = 0;
    const edmNew::DetSetVector<SiPixelCluster>& currentClusterCollection = *clusterCollectionHandle;
    // Looping on clusters with the same location
    typedef edmNew::DetSetVector<SiPixelCluster>::const_iterator clustCollIt_t;
    for(clustCollIt_t currentClusterSetIt = currentClusterCollection.begin(); currentClusterSetIt != currentClusterCollection.end(); ++currentClusterSetIt) {
      const auto& currentClusterSet = *currentClusterSetIt;
      DetId detId(currentClusterSet.id());
      unsigned int subdetId = detId.subdetId();
      // Take only pixel clusters
      if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
        continue;

      typedef edmNew::DetSet<SiPixelCluster>::const_iterator clustSetIt_t;
      for(clustSetIt_t currentClusterIt = currentClusterSet.begin(); currentClusterIt != currentClusterSet.end(); ++currentClusterIt) {
        const auto& currentCluster = *currentClusterIt;
        // Serial num of cluster in the given module
        clu_.i = currentClusterIt - currentClusterSet.begin();
        // Set if there is a valid hits
        // clu_.edge;
        // clu_.badpix;
        // clu_.tworoc;
        // Module information
	getModuleData(clu_.mod,    0, tkGeom, detId, tTopo, federrors);
	getModuleData(clu_.mod_on, 1, tkGeom, detId, tTopo, federrors);
        // Position and size
        clu_.x     = currentCluster.x();
        clu_.y     = currentCluster.y();
        clu_.sizeX = currentCluster.sizeX();
        clu_.sizeY = currentCluster.sizeY();
        clu_.size  = currentCluster.size();
        // Charge
        clu_.charge = currentCluster.charge();
        // Misc.
        for(int i = 0; i < clu_.size && i < 1000; ++i) {
          const auto& currentPixels = currentCluster.pixels();
          clu_.adc[i]    = currentCluster.pixelADC()[i] / 1000.0;
          clu_.pix[i][0] = currentPixels[i].x;
          clu_.pix[i][1] = currentPixels[i].y;
        }
        // The number of saved clusters can be downscaled to save space
        if(clusterCounter++ % clusterSaveDownlscaling != 0) continue;
        clustTree_->Fill();
      }
    }
  } else handleDefaultError("data_access", "data_access", "Failed to fetch the clusters.");




  //__________________________________________________________
  //                Trajectory - Track Loop

  // Looping on the whole track collection
  int trackIndex = 0;
  for(const auto& currentTrackKeypair: *trajTrackCollectionHandle) {
  
    const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
    const reco::TrackRef                    track = currentTrackKeypair.val;

    // Discarding tracks without pixel measurements
    if(!TrajAnalyzer::trajectoryHasPixelHit(traj)) continue;

    //__________________________________________________________
    //                      Track Infos

    track_.init();
    // Finding the closest vertex to the track
    reco::VertexCollection::const_iterator closestVtx = TrajAnalyzer::findClosestVertexToTrack(track, vertexCollectionHandle);
    // Basic track quantities
    // FIXME: Add global track counting
    track_.i       = ++trackIndex;
    track_.quality = track->qualityMask();
    track_.pt      = track->pt();
    track_.p       = track->p();
    track_.eta     = track->eta();
    track_.theta   = track->theta();
    track_.phi     = track->phi();
    // FIXME: use best vertex selection instead of closest vertex selection
    track_.d0      = track->dxy(closestVtx->position()) * -1.0;
    track_.dz      = track->dz (closestVtx->position());
    // Zeroing counters
    track_.validfpix[0]   = 0;
    track_.validfpix[1]   = 0;
    track_.validfpix[2]   = 0;
    track_.validbpix[0]   = 0;
    track_.validbpix[1]   = 0;
    track_.validbpix[2]   = 0;
    track_.validbpix[3]   = 0;
    track_.pixhit[0]      = 0;
    track_.pixhit[1]      = 0;
    track_.validpixhit[0] = 0;
    track_.validpixhit[1] = 0;
    track_.bpix[0]        = 0;
    track_.bpix[1]        = 0;
    track_.bpix[2]        = 0;
    track_.bpix[3]        = 0;
    track_.fpix[0]        = 0;
    track_.fpix[1]        = 0;
    track_.fpix[2]        = 0;
    track_.strip          = 0;
    // Counted in this loop: 
    // barrel hits, valid barrel hits, forward hits, valid forward hits,
    // top of detector hits, top of detector hits, strip hits
    for(const TrajectoryMeasurement& measurement: traj->measurements()) {
      // Check measurement validity
      if(!measurement.updatedState().isValid()) continue;
      const auto& hit = measurement.recHit();
      // Det id
      DetId detId = hit->geographicalId();
      uint32_t subDetId = (detId.subdetId());
      // Counting the strip hits
      if(hit->isValid()) {
        if(subDetId == StripSubdetector::TIB || subDetId == StripSubdetector::TOB ||
	   subDetId == StripSubdetector::TID || subDetId == StripSubdetector::TEC)
	  ++track_.strip;
      }
      // Looking for pixel hits
      if(TrajAnalyzer::subdetidIsOnPixel(subDetId)) continue;
      // Looking for valid and missing hits
      const int& validhit = hit->getType() == TrackingRecHit::valid;
      // const int& missing  = hit->getType() == TrackingRecHit::missing;
      // Module info
      ModuleData mod;
      getModuleData(mod,    0, tkGeom, detId, tTopo, federrors);
      // Forward and barrel hits
      if(subDetId == PixelSubdetector::PixelBarrel) {
        track_.bpix[mod.layer - 1]++;
        if(validhit) track_.validbpix[mod.layer-1]++;
      }
      if(subDetId == PixelSubdetector::PixelEndcap) {
        track_.fpix[mod.disk - 1]++;
        if(validhit) track_.validfpix[mod.disk - 1]++;
      }
      // Fetch the hit
      const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
      // Check hit qualty
      if(pixhit) {
        // Position measurements
        TrajectoryStateCombiner  trajStateComb;
        TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
        auto globalPosition      = trajStateOnSurface.globalPosition();
        // const float& glx = globalPosition.x();
        const float& gly = globalPosition.y();
        // const float& glz = globalPosition.z();
        // Count op-of-detector tracks
        if(0 <= gly && gly != NOVAL_F) {
          track_.pixhit[0] += 1;
          if(validhit) track_.validpixhit[0] += 1;
        }
        // Count Bottom-of-detector tracks
        if(gly < 0 && gly != NOVAL_F) {
          track_.pixhit[1] += 1;
          if(validhit) track_.validpixhit[1] += 1;
        }
      }
    }

    trackTree_->Fill();



    //__________________________________________________________
    //                     Trajectory Infos
  
    traj_.init();
    for(const TrajectoryMeasurement& meas : traj->measurements()) {
      // Check measurement validity
      if(!meas.updatedState().isValid()) continue;
      
      TransientTrackingRecHit::ConstRecHitPointer recHit = meas.recHit();

      // Only looking for pixel hits
      DetId detId = recHit->geographicalId();
      uint32_t subdetId = (detId.subdetId());
      if(!TrajAnalyzer::subdetidIsOnPixel(subdetId)) continue;
      
      // We could match recHits to simHits, here, using recipe:
      //   https://github.com/cms-analysis/DPGAnalysis-SiPixelTools/blob/master/PixelHitAssociator/test/SiPixelRecHitsValid_pix.cc#L573-L632
      //TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt->recHit();
      //if (detId == hit->geographicalId()) matchToSimHits(associate, &(*hit), detId, theGeomDet,tTopo);
      
      // Looking for valid and missing hits
      traj_.validhit = recHit->getType() == TrackingRecHit::valid;
      traj_.missing  = recHit->getType() == TrackingRecHit::missing;
      // Save module data
      getModuleData(traj_.mod,    0, tkGeom, detId, tTopo, federrors);
      getModuleData(traj_.mod_on, 1, tkGeom, detId, tTopo, federrors);
      // Position measurements
      static TrajectoryStateCombiner trajStateComb; // operator () should be const, so this is fine as a static variable
      TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(meas.forwardPredictedState(), meas.backwardPredictedState());
      GlobalPoint globalPosition     = trajStateOnSurface.globalPosition();
      LocalPoint  localPosition      = trajStateOnSurface.localPosition();
      LocalError  localPositionError = trajStateOnSurface.localError().positionError();
      traj_.glx    = globalPosition.x();
      traj_.gly    = globalPosition.y();
      traj_.glz    = globalPosition.z();
      traj_.lx     = localPosition.x();
      traj_.ly     = localPosition.y();
      traj_.lz     = localPosition.z();
      traj_.lx_err = localPositionError.xx();
      traj_.ly_err = localPositionError.yy();
      // traj_.lz_err = localPositionError.zz();
      // traj_.onedge = std::abs(traj_.lx) < 0.55 && std::abs(traj_.ly) < 3.0;
      // Track local angles
      LocalTrajectoryParameters trajectoryParameters = trajStateOnSurface.localParameters();
      auto trajectoryMomentum = trajectoryParameters.momentum();
      LocalVector localTrackDirection = trajectoryMomentum / trajectoryMomentum.mag();
      traj_.alpha = atan2(localTrackDirection.z(), localTrackDirection.x());
      traj_.beta  = atan2(localTrackDirection.z(), localTrackDirection.y());
      
      // Read associated cluster parameters
      if (recHit->isValid() && recHit->hit()!=0) {
	const SiPixelRecHit *hit=(const SiPixelRecHit*)recHit->hit();
	SiPixelRecHit::ClusterRef const& clust=hit->cluster();
	if (clust.isNonnull()) {
	  traj_.clu.charge=clust->charge()/1000.0;
	  traj_.clu.size=clust->size();
	  traj_.clu.edge=hit->isOnEdge() ? 1 : 0;
	  traj_.clu.badpix=hit->hasBadPixels() ? 1 : 0;
	  traj_.clu.tworoc=hit->spansTwoROCs() ? 1 : 0;
	  traj_.clu.sizeX=clust->sizeX();
	  traj_.clu.sizeY=clust->sizeY();
	  traj_.clu.x=clust->x();
	  traj_.clu.y=clust->y();
	  for (int i=0; i<clust->size() && i<1000; i++) {
	    traj_.clu.adc[i]=float(clust->pixelADC()[i])/1000.0;
	    traj_.clu.pix[i][0]=((clust->pixels())[i]).x;
	    traj_.clu.pix[i][1]=((clust->pixels())[i]).y;
	  }
	  traj_.norm_charge = traj_.clu.charge*
	    sqrt(1.0/(1.0/pow(tan(traj_.alpha),2)+1.0/pow(tan(traj_.beta),2)+1.0));

	  traj_.dx_cl = std::abs(clust->x() - traj_.lx);
	  traj_.dy_cl = std::abs(clust->y() - traj_.ly);
	  traj_.d_cl = sqrt(traj_.dx_cl*traj_.dx_cl + traj_.dy_cl*traj_.dy_cl);
	}
      }

      // Get closest other traj measurement
      TrajAnalyzer::getClosestOtherTrackDistanceByLooping(meas, trajTrackCollectionHandle, traj_.d_tr, traj_.dx_tr, traj_.dy_tr);
      traj_.hit_near = (traj_.d_tr < 0.5); // 5 mm
      traj_.clust_near = (traj_.d_cl != NOVAL_F && traj_.d_cl < 0.05); // 500 um
      
      
      //++  // Add hit efficiency cuts to the saved fields
      //++  // Moved to src/PhaseI_tracking_efficiency_filters.cc
      //++  PhaseITrackingEfficiencyFilters filterObject(evt_, traj_);
      //++  int cutResults = filterObject.performCuts(
      //++    					PhaseITrackingEfficiencyFilters::Cuts::nvtx     |
      //++    					// PhaseITrackingEfficiencyFilters::Cuts::zerobias |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::federr   |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::hp       |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::pt       |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::nstrip   |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::d0       |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::dz       |
      //++    					// PhaseITrackingEfficiencyFilters::Cuts::pixhit   |
      //++    					// PhaseITrackingEfficiencyFilters::Cuts::lx_fid   |
      //++    					// PhaseITrackingEfficiencyFilters::Cuts::ly_fid   |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::valmis   |
      //++    					PhaseITrackingEfficiencyFilters::Cuts::hitsep
      //++    					);
      //++  traj_.pass_effcuts = cutResults;
      // Filling the tree
      trajTree_->Fill();
    }
  }

}


////////////////////
// Error handling //
////////////////////

void PhaseIPixelNtuplizerNew::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::string msg) {
  edm::LogError(streamType.c_str()) << c_red << msg << c_def << std::endl;
  printEvtInfo(streamType);
  throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizerNew::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::vector<std::string> msg) {
  edm::LogError(streamType.c_str()) << c_red;
  for(const auto& msg_part: msg)
    edm::LogError(streamType.c_str()) << msg_part;
  edm::LogError(streamType.c_str()) << c_def << std::endl;
  printEvtInfo(streamType);
  throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizerNew::printEvtInfo(const std::string& streamType) {
  edm::LogError(streamType.c_str()) << c_blue <<
    "Run: " << evt_.run << 
    " Ls: " << evt_.ls  << 
    " Evt:" << evt_.evt << c_def << std::endl;
}

// Module Data - Method works for both Phase 0/I, Offline/Online
void PhaseIPixelNtuplizerNew::getModuleData(ModuleData& mod, bool online, const TrackerGeometry* const tkGeom,
					    const DetId& detId, const TrackerTopology* const tTopo, const std::map<uint32_t, int>& federrors) {
  mod.init();

  bool phase = tkGeom->isThere(GeomDetEnumerators::P1PXB) && tkGeom->isThere(GeomDetEnumerators::P1PXEC);

  if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
    PixelBarrelName pbn(detId, tTopo, phase);
    mod.det      = detId.subdetId() - 1;
    mod.shl      = pbn.shell();
    mod.sec      = pbn.sectorName();
    mod.half     = pbn.isHalfModule();
    mod.layer    = tTopo->pxbLayer(detId.rawId());
    bool odd_lad = tTopo->pxbLadder(detId.rawId())%2;
    mod.outer    = (phase ? mod.layer==4 : mod.layer%2) ? odd_lad : !odd_lad;
    if (online) {
      mod.ladder = pbn.ladderName();
      mod.module = pbn.moduleName();
      // Use sign convention
      if (pbn.shell() == PixelBarrelName::Shell::mO ||
	  pbn.shell() == PixelBarrelName::Shell::mI)
	mod.module = -mod.module;
      if (pbn.shell() == PixelBarrelName::Shell::mO ||
	  pbn.shell() == PixelBarrelName::Shell::pO)
	mod.ladder = -mod.ladder;
    } else {
      mod.ladder = tTopo->pxbLadder(detId.rawId());
      mod.module = tTopo->pxbModule(detId.rawId()); 
    }
  } else if(detId.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap)) {
    PixelEndcapName pen(detId, tTopo, phase);
    mod.det    = detId.subdetId() - 1;
    mod.shl    = pen.halfCylinder();
    mod.ring   = pen.ringName();
    mod.side   = tTopo->pxfSide(detId.rawId());
    mod.disk   = tTopo->pxfDisk(detId.rawId());
    mod.panel  = tTopo->pxfPanel(detId.rawId());
    if (online) {
      mod.blade  = pen.bladeName();
      mod.module = pen.plaquetteName();
      // Use sign convention
      if (pen.halfCylinder() == PixelEndcapName::HalfCylinder::mO || 
	  pen.halfCylinder() == PixelEndcapName::HalfCylinder::mI)
	mod.disk   = -mod.disk;
      if (pen.halfCylinder() == PixelEndcapName::HalfCylinder::mO || 
	  pen.halfCylinder() == PixelEndcapName::HalfCylinder::pO)
	mod.blade  = -mod.blade;
    } else {
      mod.blade  = tTopo->pxfBlade(detId.rawId());
      mod.module = tTopo->pxfModule(detId.rawId());
    }
  }

  mod.rawid = detId.rawId();

  // FED error
  std::map<uint32_t, int>::const_iterator federrors_it=federrors.find(detId.rawId());
  mod.federr = (federrors_it!=federrors.end()) ? federrors_it->second :0;
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizerNew);

