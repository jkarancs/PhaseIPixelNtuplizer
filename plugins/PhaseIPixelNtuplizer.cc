#include "PhaseIPixelNtuplizer.h"
#include "TVector.h"

constexpr int                  PhaseIPixelNtuplizer::ZEROBIAS_TRIGGER_BIT;
constexpr int                  PhaseIPixelNtuplizer::ZEROBIAS_BITMASK;
constexpr int                  PhaseIPixelNtuplizer::VERTEX_NUMTRACK_CUT_VAL;
constexpr int                  PhaseIPixelNtuplizer::TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr int                  PhaseIPixelNtuplizer::TRACK_QUALITY_HIGH_PURITY_MASK;
constexpr float                PhaseIPixelNtuplizer::TRACK_PT_CUT_VAL;
constexpr int                  PhaseIPixelNtuplizer::TRACK_NSTRIP_CUT_VAL;
constexpr std::array<float, 4> PhaseIPixelNtuplizer::TRACK_D0_CUT_BARREL_VAL;
constexpr float                PhaseIPixelNtuplizer::TRACK_D0_CUT_FORWARD_VAL;
constexpr float                PhaseIPixelNtuplizer::TRACK_DZ_CUT_BARREL_VAL;
constexpr float                PhaseIPixelNtuplizer::TRACK_DZ_CUT_FORWARD_VAL;
constexpr float                PhaseIPixelNtuplizer::MEAS_HITSEP_CUT_VAL;
constexpr float                PhaseIPixelNtuplizer::HIT_CLUST_NEAR_CUT_VAL;
constexpr float                PhaseIPixelNtuplizer::BARREL_MODULE_EDGE_X_CUT;
constexpr float                PhaseIPixelNtuplizer::BARREL_MODULE_EDGE_Y_CUT;

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig) : 
  iConfig_(iConfig),
  ntupleOutputFilename_(iConfig.getUntrackedParameter<std::string>
			("outputFileName", "Ntuple.root")),
  isEventFromMc_(iConfig.getUntrackedParameter<bool>("MC", false)),
  isCosmicTracking_(iConfig.getUntrackedParameter<bool>("cosmics", false)),
  clusterSaveDownscaling_(iConfig.getUntrackedParameter<int>("clusterSaveDownscaleFactor",100)),
  trackSaveDownscaling_(iConfig.getUntrackedParameter<int>("trackSaveDownscaleFactor",1)),
  eventSaveDownscaling_(iConfig.getUntrackedParameter<int>("eventSaveDownscaleFactor",1)),
  saveDigiTree_(iConfig.getUntrackedParameter<bool>("saveDigiTree", false)),
  saveTrackTree_(iConfig.getUntrackedParameter<bool>("saveTrackTree", false)),
  saveNonPropagatedExtraTrajTree_(iConfig.getUntrackedParameter<bool>
          ("saveNonPropagatedExtraTrajTree", false)),
  keepAllGlobalMuons_(iConfig.getUntrackedParameter<bool>("keepAllGlobalMuons", true)),
  keepAllTrackerMuons_(iConfig.getUntrackedParameter<bool>("keepAllTrackerMuons", true)),
  npixFromDigiCollection_(iConfig.getUntrackedParameter<bool>("npixFromDigiCollection", false)),
  minVertexSize_(15),
  efficiencyCalculationFrequency_(iConfig.getUntrackedParameter<int>("efficiencyCalculationFrequency_", 1))
{

  if(isCosmicTracking_) 
    std::cout << "Running with cosmics setting turned on." << std::endl;
  if(saveDigiTree_)
    std::cout << "Option recognized: request to save digis." << std::endl;
  if(saveTrackTree_) 
    std::cout << "Option recognized: request to save all tracks as a separate tree." << std::endl;
  if(saveNonPropagatedExtraTrajTree_) 
    std::cout << "Option recognized: request to save the traj." \
      " measurements replaced by track propagation as a separate tree." << std::endl;

  // Tokens
  rawDataErrorToken_ = consumes<edm::DetSetVector<SiPixelRawDataError>>
    (edm::InputTag("siPixelDigis"));
  primaryVerticesToken_  = consumes<reco::VertexCollection>
    (edm::InputTag("offlinePrimaryVertices"));
  triggerResultsToken_ = consumes<edm::TriggerResults>(triggerTag_);
  pileupSummaryToken_ = consumes<std::vector<PileupSummaryInfo>>(edm::InputTag("addPileupInfo"));
  clustersToken_ = consumes<edmNew::DetSetVector<SiPixelCluster>>
    (iConfig.getParameter<edm::InputTag>("clusterCollection"));
  trajTrackCollectionToken_ = consumes<TrajTrackAssociationCollection>
    (iConfig.getParameter<edm::InputTag>("trajectoryInput"));
  trajTrackCollectionToken_ = consumes<TrajTrackAssociationCollection>
    (iConfig.getParameter<edm::InputTag>("trajectoryInput"));
  measurementTrackerEventToken_ = consumes<MeasurementTrackerEvent>
    (edm::InputTag("MeasurementTrackerEvent"));

  // Save digi tree only if saveDigiTree_ option is set
#if ADD_SIM_INFO >0
  if (isEventFromMc_) {
    if(saveDigiTree_ || npixFromDigiCollection_) pixelDigiCollectionToken_ = consumes<edm::DetSetVector<PixelDigi>>
						   (edm::InputTag("simSiPixelDigis"));
    trackerHitAssociatorConfig_ = TrackerHitAssociator::Config(iConfig, consumesCollector());
    simhitCollectionTokens_.insert
      (simhitCollectionTokens_.end(), {
	consumes<std::vector<PSimHit> >(edm::InputTag("g4SimHits", "TrackerHitsPixelBarrelHighTof")),
	  consumes<std::vector<PSimHit> >(edm::InputTag("g4SimHits", "TrackerHitsPixelBarrelLowTof")),
	  consumes<std::vector<PSimHit> >(edm::InputTag("g4SimHits", "TrackerHitsPixelEndcapHighTof")),
	  consumes<std::vector<PSimHit> >(edm::InputTag("g4SimHits", "TrackerHitsPixelEndcapLowTof")) });
    //	conditionsInRunBlockToken_ = mayConsume<edm::ConditionsInRunBlock, edm::InRun>(edm::InputTag("conditionsInEdm"));
    
    simTrackToken_ = consumes<edm::SimTrackContainer>(edm::InputTag("g4SimHits"));
  }
#endif

  muonCollectionToken_ = consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muonCollection"));
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer() {}

void PhaseIPixelNtuplizer::beginJob()
{
  setTriggerTable();

  // Create output file
  ntupleOutputFile_ = new TFile(ntupleOutputFilename_.c_str(), "RECREATE");
  if(!(ntupleOutputFile_ -> IsOpen())) {
    handleDefaultError("file_operations", "file_operations",
		       { "Failed to open output file: ", ntupleOutputFilename_ });
  }
  LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename_ 
			      << "\" created." << std::endl;

  // Tree definitions
  eventTree_ = new TTree("eventTree", "The event.");
  if(saveDigiTree_)
    digiTree_ = new TTree("digiTree",   "Digis in the Pixel detector.");
  clustTree_ = new TTree("clustTree", "Pixel clusters.");
  if(saveTrackTree_) {
    trackTree_ = new TTree("trackTree", "The track in the event.");
  }
  trajTree_  = new TTree("trajTree",   "Trajectory measurements in the Pixel detector.");
  if(saveNonPropagatedExtraTrajTree_) {
    nonPropagatedExtraTrajTree_  = new TTree("nonPropagatedExtraTrajTree",
					     "The original trajectroy measurements replaced by"
					     " propagated hits in the Pixel detector.");
  }
  trajROCEfficiencyTree_ = new TTree("trajROCEfficiencyTree", "ROC and module efficiencies.");
  // Event tree
  eventTree_ -> Branch("event", &evt_, evt_.list.c_str());
  // Digi tree
  if(saveDigiTree_) {
    digiTree_ -> Branch("event", &evt_,  evt_.list.c_str());
    digiTree_ -> Branch("digi",  &digi_, digi_.list.c_str());
  }
  // Cluster tree
  clustTree_ -> Branch("event",     &evt_,         evt_        .list.c_str());
  clustTree_ -> Branch("mod_on",    &clu_.mod_on,  clu_.mod_on .list.c_str());
  clustTree_ -> Branch("mod",       &clu_.mod,     clu_.mod    .list.c_str());
  clustTree_ -> Branch("clust",     &clu_,         clu_        .list.c_str());
  clustTree_ -> Branch("clust_adc", &clu_.adc,     "adc[size]/F");
  clustTree_ -> Branch("clust_pix", &clu_.pix,     "pix[size][2]/F");
  // Track tree
  if(saveTrackTree_) {
    trackTree_ -> Branch("event",     &evt_,         evt_        .list.c_str());
    trackTree_ -> Branch("track",     &track_,       track_      .list.c_str());
  }
  // Trajectory tree
  trajTree_  -> Branch("event",     &evt_,          evt_        .list.c_str());
  trajTree_  -> Branch("mod_on",    &traj_.mod_on,  traj_.mod_on.list.c_str());
  trajTree_  -> Branch("mod",       &traj_.mod,     traj_.mod   .list.c_str());
  trajTree_  -> Branch("clust",     &traj_.clu,     traj_.clu   .list.c_str());
  trajTree_  -> Branch("clust_adc", &traj_.clu.adc, "adc[size]/F");
  trajTree_  -> Branch("clust_pix", &traj_.clu.pix, "pix[size][2]/F");
  trajTree_  -> Branch("track",     &track_,        track_      .list.c_str());
  trajTree_  -> Branch("traj",      &traj_,         traj_       .list.c_str());
  // Additional trajectory tree
  if(saveNonPropagatedExtraTrajTree_) {
    nonPropagatedExtraTrajTree_  = new TTree
      ("nonPropagatedExtraTrajTree",
       "The original trajectroy measurements replaced by propagated hits in the Pixel detector.");
    nonPropagatedExtraTrajTree_  -> Branch("event",     &evt_,          evt_        .list.c_str());
    nonPropagatedExtraTrajTree_  -> Branch("mod_on",    &traj_.mod_on,  traj_.mod_on.list.c_str());
    nonPropagatedExtraTrajTree_  -> Branch("mod",       &traj_.mod,     traj_.mod   .list.c_str());
    nonPropagatedExtraTrajTree_  -> Branch("clust",     &traj_.clu,     traj_.clu   .list.c_str());
    nonPropagatedExtraTrajTree_  -> Branch("clust_adc", &traj_.clu.adc, "adc[size]/F");
    nonPropagatedExtraTrajTree_  -> Branch("clust_pix", &traj_.clu.pix, "pix[size][2]/F");
    nonPropagatedExtraTrajTree_  -> Branch("track",     &track_,        track_      .list.c_str());
    nonPropagatedExtraTrajTree_  -> Branch("traj",      &traj_,         traj_       .list.c_str());
  }
  // Efficiency of the detector parts the trajectory measurements are located on
  trajROCEfficiencyTree_ -> Branch("trajROCEfficiency", &trajROCEff_, trajROCEff_.list.c_str());
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  simhitOccupancy_fwd        = new TH2D("simhitOccupancy_fwd", "simhit occupancy - forward", 
					150, -52.15, 52.15,  300,  -3.14159,  3.14159);
  simhitOccupancy_l1         = new TH2D("simhitOccupancy_l1",  "simhit occupancy - layer 1", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  simhitOccupancy_l2         = new TH2D("simhitOccupancy_l2",  "simhit occupancy - layer 2", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  simhitOccupancy_l3         = new TH2D("simhitOccupancy_l3",  "simhit occupancy - layer 3", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  simhitOccupancy_l4         = new TH2D("simhitOccupancy_l4",  "simhit occupancy - layer 4", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  digiOccupancy_fwd          = new TH2D("digiOccupancy_fwd",   "digi occupancy - forward",   
					150, -52.15, 52.15,  300,  -3.14159,  3.14159);
  digiOccupancy_l1           = new TH2D("digiOccupancy_l1",    "digi occupancy - layer 1",   
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  digiOccupancy_l2           = new TH2D("digiOccupancy_l2",    "digi occupancy - layer 2",   
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  digiOccupancy_l3           = new TH2D("digiOccupancy_l3",    "digi occupancy - layer 3",   
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  digiOccupancy_l4           = new TH2D("digiOccupancy_l4",    "digi occupancy - layer 4",   
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancy_fwd         = new TH2D("clustOccupancy_fwd",  "cluster occupancy - forward",
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancy_l1          = new TH2D("clustOccupancy_l1",   "cluster occupancy - layer 1",
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancy_l2          = new TH2D("clustOccupancy_l2",   "cluster occupancy - layer 2",
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancy_l3          = new TH2D("clustOccupancy_l3",   "cluster occupancy - layer 3",
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancy_l4          = new TH2D("clustOccupancy_l4",   "cluster occupancy - layer 4",
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  rechitOccupancy_fwd        = new TH2D("rechitOccupancy_fwd", "rechit occupancy - forward", 
					150, -52.15, 52.15,  300,  -3.14159,  3.14159);
  rechitOccupancy_l1         = new TH2D("rechitOccupancy_l1",  "rechit occupancy - layer 1", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  rechitOccupancy_l2         = new TH2D("rechitOccupancy_l2",  "rechit occupancy - layer 2", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  rechitOccupancy_l3         = new TH2D("rechitOccupancy_l3",  "rechit occupancy - layer 3", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  rechitOccupancy_l4         = new TH2D("rechitOccupancy_l4",  "rechit occupancy - layer 4", 
					150, -26.7,  26.7,   300,  -3.14159,  3.14159);
  clustOccupancyROCBins_fwd  = new TH2D
    ("clustOccupancyROCBins_fwd",  "cluster occupancy on ROCs - forward",
     112,-3.5,3.5, 140,-17.5,17.5);
  clustOccupancyROCBins_l1   = new TH2D
    ("clustOccupancyROCBins_l1",   "cluster occupancy on ROCs - layer 1",
     72, -4.5,4.5,  26, -6.5, 6.5);
  clustOccupancyROCBins_l2   = new TH2D
    ("clustOccupancyROCBins_l2",   "cluster occupancy on ROCs - layer 2",
     72, -4.5,4.5,  58,-14.5,14.5);
  clustOccupancyROCBins_l3   = new TH2D
    ("clustOccupancyROCBins_l3",   "cluster occupancy on ROCs - layer 3",
     72, -4.5,4.5,  90,-22.5,22.5);
  clustOccupancyROCBins_l4   = new TH2D
    ("clustOccupancyROCBins_l4",   "cluster occupancy on ROCs - layer 4",
     72, -4.5,4.5, 130,-32.5,32.5);
  rechitOccupancyROCBins_fwd = new TH2D
    ("rechitOccupancyROCBins_fwd", "rechit occupancy on ROCS - forward",
     112,-3.5,3.5, 140,-17.5,17.5);
  rechitOccupancyROCBins_l1  = new TH2D
    ("rechitOccupancyROCBins_l1",  "rechit occupancy on ROCS - layer 1",
     72, -4.5,4.5,  26, -6.5, 6.5);
  rechitOccupancyROCBins_l2  = new TH2D
    ("rechitOccupancyROCBins_l2",  "rechit occupancy on ROCS - layer 2",
     72, -4.5,4.5,  58,-14.5,14.5);
  rechitOccupancyROCBins_l3  = new TH2D
    ("rechitOccupancyROCBins_l3",  "rechit occupancy on ROCS - layer 3",
     72, -4.5,4.5,  90,-22.5,22.5);
  rechitOccupancyROCBins_l4  = new TH2D
    ("rechitOccupancyROCBins_l4",  "rechit occupancy on ROCS - layer 4",
     72, -4.5,4.5, 130,-32.5,32.5);
  // FIXME
  disk1PropagationEtaNumhits    = new TH1D("disk1PropagationEtaNumhits", 
					   "disk1PropagationEtaNumhits",
					   100, -3.1415, 3.1415);
  disk1PropagationEtaEfficiency = new TH1D("disk1PropagationEtaEfficiency",
					   "disk1PropagationEtaEfficiency",
					   100, -3.1415, 3.1415);
#endif
}

void PhaseIPixelNtuplizer::endJob() 
{
  std::cout << "Ntuplizer endjob step started." << std::endl;
  std::cout << "Generating ROC efficiency tree for the missing events..." << std::endl;
  //generateROCEfficiencyTree();
  std::cout << "Done generating ROC efficiency tree." << std::endl;
  std::cout << "OutputFileName in the Ntuplizer endjob: " << ntupleOutputFilename_ << "\"." << std::endl;
  ntupleOutputFile_ -> cd();
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  constexpr int PHASE_SCENARIO = 1;
  gStyle -> SetPalette(1);
  gStyle -> SetNumberContours(999);
  const std::vector<TH2D*> histogramsToSave = 
    {
      simhitOccupancy_fwd,        simhitOccupancy_l1,        
      simhitOccupancy_l2,         simhitOccupancy_l3,        simhitOccupancy_l4,
      digiOccupancy_fwd,          digiOccupancy_l1,          
      digiOccupancy_l2,           digiOccupancy_l3,          digiOccupancy_l4,
      clustOccupancy_fwd,         clustOccupancy_l1,         
      clustOccupancy_l2,          clustOccupancy_l3,         clustOccupancy_l4, 
      rechitOccupancy_fwd,        rechitOccupancy_l1,        
      rechitOccupancy_l2,         rechitOccupancy_l3,        rechitOccupancy_l4,
      clustOccupancyROCBins_fwd,  clustOccupancyROCBins_l1,  
      clustOccupancyROCBins_l2,   clustOccupancyROCBins_l3,  clustOccupancyROCBins_l4,
      rechitOccupancyROCBins_fwd, rechitOccupancyROCBins_l1, 
      rechitOccupancyROCBins_l2,  rechitOccupancyROCBins_l3, rechitOccupancyROCBins_l4
    };
  for(auto histoIt = histogramsToSave.begin(); histoIt != histogramsToSave.end(); ++histoIt) {
    TCanvas* canvas = custom_can_((*histoIt), std::string((*histoIt) -> GetTitle()) + "_canvas", 0, 0, 800, 800, 80, 140); 
    canvas -> cd();
    (*histoIt) -> Draw("COLZ");
    std::string histoName = (*histoIt) -> GetName();
    if(histoName.find("ROC") != std::string::npos) {
      int layer = -1;
      if(histoName.find("_fwd") != std::string::npos) layer = 0;
      else {
	const size_t labelPosition = histoName.find("_l");
	if(labelPosition != std::string::npos) {
	  if(histoName.size() <= labelPosition + 2) {
	    std::cout << __PRETTY_FUNCTION__ <<": Error while processing the extra histogram names."
		      << "Histogram name should contain layer info after _l: " 
		      << histoName << std::endl;
	    continue;
	  }
	  layer = histoName.at(labelPosition + 2) - '0';
	}
      }
      dress_occup_plot((*histoIt), layer, PHASE_SCENARIO);
    }
    // (*histoIt) -> SetDirectory(ntupleOutputFile_);
    (*histoIt) -> Write();
    canvas     -> Write();
  }
  // FIXME
  const std::vector<TH1D*> disk1PropagationPlots =
    {
      disk1PropagationEtaNumhits,
      disk1PropagationEtaEfficiency
    };
  for(auto histoIt = disk1PropagationPlots.begin();
      histoIt != disk1PropagationPlots.end(); ++histoIt) {
    TCanvas* canvas = custom_can_((*histoIt),
				  std::string((*histoIt) -> GetTitle()) + "_canvas",
				  0, 0, 800, 800, 80, 140); 
    canvas -> cd();
    (*histoIt) -> Draw("");
    std::string histoName = (*histoIt) -> GetName();
    (*histoIt) -> Write();
    canvas     -> Write();
  }
#endif
  std::cout << "Writing plots to file: \"" << ntupleOutputFilename_ << "\"." << std::endl;
  ntupleOutputFile_ -> Write();
  std::cout << "Closing file: \"" << ntupleOutputFilename_ << "\"." << std::endl;
  ntupleOutputFile_ -> Close();
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {
  // iRun.getByToken(conditionsInRunBlockToken_,  conditionsInRunBlock_);
}

void PhaseIPixelNtuplizer::endRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {}

void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const& iLumi,
						edm::EventSetup const& iSetup) {}

void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const& iLumi,
					      edm::EventSetup const& iSetup) {
  lumi_.time = iLumi.beginTime().unixTime();
  ++nLumisection_;
  //if(nLumisection_ % efficiencyCalculationFrequency_ == 0)
  //{
  //  generateROCEfficiencyTree();
  //}
}

void PhaseIPixelNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  if (++nEvent_ % eventSaveDownscaling_ != 0) return;

  // std::cout << "Analysis: " << std::endl;
  // A message is printed every time a change in the data type occurs
  if (isEventFromMc_ != (iEvent.id().run() == 1)) {
    isEventFromMc_ = iEvent.id().run() == 1;
    std::cout << "Deduced data type: " 
	      << (isEventFromMc_ ? "MONTE-CARLO" : "REAL RAW DATA") << "." << std::endl;
  }
  LogDebug("step") << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;

  // FED errors
  federrors_ = NtuplizerHelpers::getFedErrors(iEvent, rawDataErrorToken_);

#if ADD_SIM_INFO > 0
  // Simhits and hit associator
  std::vector<edm::Handle<edm::PSimHitContainer>>
    simhitCollectionHandles(simhitCollectionTokens_.size());
  if (isEventFromMc_) {
    for(unsigned int numToken = 0; numToken < simhitCollectionTokens_.size(); ++numToken) {
      iEvent.getByToken(simhitCollectionTokens_[numToken], simhitCollectionHandles[numToken]);
    }
    //trackerHitAssociator_ = new TrackerHitAssociator(iEvent, trackerHitAssociatorConfig_);
    pixelHitAssociator_   = new PixelHitAssociator(iEvent);
  }
#endif
  
  // Digis
  edm::Handle<edm::DetSetVector<PixelDigi>> digiCollectionHandle;
  if (isEventFromMc_) {
    if(saveDigiTree_ || npixFromDigiCollection_) iEvent.getByToken(pixelDigiCollectionToken_, digiCollectionHandle);
  }

  // SimTracks
  edm::Handle<edm::SimTrackContainer> simTracksHandle;
#if ADD_SIM_INFO > 0
  if (isEventFromMc_) {
    iEvent.getByToken(simTrackToken_, simTracksHandle);
  }
#endif
  
  // Get vertices
  edm::Handle<reco::VertexCollection>      vertexCollectionHandle;
  iEvent.getByToken(primaryVerticesToken_, vertexCollectionHandle);

  // Get trigger info
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  iEvent.getByToken(triggerResultsToken_, triggerResultsHandle);

  // Get pileup info
  edm::Handle<std::vector<PileupSummaryInfo>> puInfoCollectionHandle;
  iEvent.getByToken(pileupSummaryToken_,      puInfoCollectionHandle);

  // Get cluster collection
  edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle;
  iEvent.getByToken(clustersToken_,                 clusterCollectionHandle);

  // Get Traj-Track Collection
  edm::Handle<TrajTrackAssociationCollection>  trajTrackCollectionHandle;
  iEvent.getByToken(trajTrackCollectionToken_, trajTrackCollectionHandle);

  // muons
  edm::Handle<reco::MuonCollection> muonCollectionHandle;
  iEvent.getByToken(muonCollectionToken_, muonCollectionHandle);

  // track builder for IPTools methods
  edm::ESHandle<TransientTrackBuilder> trackBuilderHandle;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilderHandle);

  // TrackerTopology for module informations
  edm::ESHandle<TrackerTopology> trackerTopologyHandle;
  iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
  trackerTopology_ = trackerTopologyHandle.product();

  // TrackerGeometry for module informations
  edm::ESHandle<TrackerGeometry> trackerGeometryHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(trackerGeometryHandle);
  trackerGeometry_ = trackerGeometryHandle.product();

  // Tracker propagator for propagating tracks to other layers
  edm::ESHandle<Propagator> propagatorHandle;
  iSetup.get<TrackingComponentsRecord>().get("PropagatorWithMaterial", propagatorHandle);
  std::unique_ptr<Propagator> propagatorUniquePtr(propagatorHandle.product() -> clone());
  trackerPropagator_ = propagatorUniquePtr.get();
  const_cast<Propagator*>(trackerPropagator_) -> setPropagationDirection(oppositeToMomentum);

  // Measurement Tracker Handle
  edm::ESHandle<MeasurementTracker> measurementTrackerHandle;
  iSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
  measurementTracker_ = measurementTrackerHandle.product();

  // Measurement Tracker event
  edm::Handle<MeasurementTrackerEvent> measurementTrackerEventHandle;
  iEvent.getByToken(measurementTrackerEventToken_, measurementTrackerEventHandle);
  measurementTrackerEvent_ = measurementTrackerEventHandle.product();

  // Measurement estimator
  edm::ESHandle<Chi2MeasurementEstimatorBase> chi2MeasurementEstimatorHandle;
  iSetup.get<TrackingComponentsRecord>().get("Chi2", chi2MeasurementEstimatorHandle);
  chi2MeasurementEstimator_ = chi2MeasurementEstimatorHandle.product();

  // Pixel Parameter estimator
  edm::ESHandle<PixelClusterParameterEstimator> pixelClusterParameterEstimatorHandle;
  iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", pixelClusterParameterEstimatorHandle);
  pixelClusterParameterEstimator_ = pixelClusterParameterEstimatorHandle.product();

#if CMSSW_VERSION > 110
  // Get CablingMap (used for ROC number)
  edm::ESHandle<SiPixelFedCablingMap> cablingMapHandle;
  iSetup.get<SiPixelFedCablingMapRcd>().get(cablingMapHandle);

  // Initialize the object used to calculate module geometric informations
  coord_.init(trackerTopology_, trackerGeometry_, cablingMapHandle.product());
#else
  coord_.init(iSetup);
#endif
  //std::cout << "Event summary informations: " << std::endl;
  //std::cout << "Vertices: " << (vertexCollectionHandle.isValid() ? std::to_string(vertexCollectionHandle -> size()) : "invalid") << " ";
  //if(saveDigiTree_) std::cout << "Digis: " << (digiCollectionHandle.isValid()) 
  //      		      << (digiCollectionHandle.isValid() ? 
  //      			  std::to_string(digiCollectionHandle -> size()) : "invalid")
  //      		      << " ";
  //
  //std::cout << "Clusters: " << (clusterCollectionHandle.isValid() ?
  //      			std::to_string(clusterCollectionHandle -> size()) : "invalid")
  //          << " ";
  //std::cout << "Tracks: " << (trajTrackCollectionHandle.isValid() ?
  //      		      std::to_string(trajTrackCollectionHandle -> size()) : "invalid")
  //          << " ";
  getEvtData(iEvent, vertexCollectionHandle, triggerResultsHandle,
	     puInfoCollectionHandle, digiCollectionHandle, clusterCollectionHandle, trajTrackCollectionHandle);
  if(saveDigiTree_ && digiCollectionHandle.isValid()) {
    //std::cout << "Saving digis and creating digi plots..." << std::endl;
    getDigiData(digiCollectionHandle);
  }

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  int areAllSimhitHandlesValid = 
    std::all_of(simhitCollectionHandles.begin(),
		simhitCollectionHandles.end(),
		[] (const edm::Handle<std::vector<PSimHit>>& handle) { return handle.isValid(); });
  if(areAllSimhitHandlesValid) {
    //std::cout << "Saving simhit plots..." << std::endl;
    getSimhitData(simhitCollectionHandles);
  } else {
    static int timesReported = 0;
    if(timesReported < 10) 
      std::cout << "Error in: " << __PRETTY_FUNCTION__
		<< ": One or more of the handles are invalid or missing! Skipping event." \
	" (Check the input products!)" << std::endl;
    if(++timesReported == 10) 
      std::cout << "Invalid handles were reported more than 10 times." \
	" Omitting further reports!" << std::endl;
    return;
  }
#endif

  //std::cout << "Saving clusters..." << std::endl;
  getClustData(clusterCollectionHandle);

  //std::cout << "Saving trajecectory measurements and track data..." << std::endl;
  if(isCosmicTracking_) {
    getTrajTrackDataCosmics(vertexCollectionHandle, clusterCollectionHandle, trajTrackCollectionHandle,
                            muonCollectionHandle, trackBuilderHandle, simTracksHandle);
  } else {
    getTrajTrackData(vertexCollectionHandle, clusterCollectionHandle, trajTrackCollectionHandle, 
                     muonCollectionHandle, trackBuilderHandle, simTracksHandle);
  }
  //std::cout << "The Phase1Ntuplizer data processing has been finished." << std::endl;

#if ADD_SIM_INFO > 0
  if (isEventFromMc_) {
    //delete trackerHitAssociator_;
    delete pixelHitAssociator_;
  }
#endif
}

void PhaseIPixelNtuplizer::setTriggerTable() {

  triggerNames_.clear();
  if(iConfig_.exists("triggerTag")) {
    triggerTag_ = iConfig_.getParameter<edm::InputTag>("triggerTag");
    std::cout << "NON-DEFAULT PARAMETER: triggerTag = " << triggerTag_ << std::endl;
  } else triggerTag_ = edm::InputTag("TriggerResults", "", "HLT"); 

  if(iConfig_.exists("triggerNames")) {
    triggerNames_ = iConfig_.getParameter<std::vector<std::string>>("triggerNames");
    std::cout << "NON-DEFAULT PARAMETER: triggerNames= ";
    for(size_t i = 0; i < triggerNames_.size(); i++) std::cout << triggerNames_[i] << " ";
    std::cout << std::endl;
  } else {
    triggerNames_.push_back("HLT_ZeroBias_v");
    triggerNames_.push_back("HLT_Random_v");
  }

}

void PhaseIPixelNtuplizer::getEvtData
(const edm::Event& iEvent,
 const edm::Handle<reco::VertexCollection>& vertexCollectionHandle,
 const edm::Handle<edm::TriggerResults>& triggerResultsHandle,
 const edm::Handle<std::vector<PileupSummaryInfo>>& puInfoCollectionHandle,
 const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollectionHandle,
 const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle,
 const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle) {

  // Event info
  // Set data holder object
  evt_.init();
  evt_.run          = iEvent.id().run();
  // if(conditionsInRunBlock_ -> isValid()) evt_.fill = conditionsInRunBlock_ -> lhcFillNumber;
  evt_.ls           = iEvent.luminosityBlock();
  evt_.orb          = iEvent.orbitNumber();
  evt_.bx           = iEvent.bunchCrossing();
  evt_.evt          = iEvent.id().event();
  evt_.trig         = getTriggerInfo(iEvent, triggerResultsHandle);
  evt_.pileup       = getPileupInfo(puInfoCollectionHandle);
  evt_.time         = lumi_.time;

  //std::cout << "Processing event of run: " << evt_.run << ", event: " << evt_.evt << ", orb: " << evt_.orb << ", bx: " << evt_.bx << "." << std::endl;

  // Loop on vertices
  evt_.nvtx    = 0;
  evt_.vtxntrk = 0;
  for(const auto &currentVertex: *vertexCollectionHandle) {

    // Invalid vertex
    if(!currentVertex.isValid()) continue;

    // Check if it is the best vertex (largest trk number, preferably in the middle
    if( (currentVertex.tracksSize()  >  static_cast<size_t>(evt_.vtxntrk)) ||
	(currentVertex.tracksSize()  == static_cast<size_t>(evt_.vtxntrk)  &&
	 std::abs(currentVertex.z()) <  std::abs(evt_.vtxZ)) ) {
      evt_.vtxntrk = currentVertex.tracksSize();
      evt_.vtxD0   = currentVertex.position().rho();
      evt_.vtxX    = currentVertex.x();
      evt_.vtxY    = currentVertex.y();
      evt_.vtxZ    = currentVertex.z();
      evt_.vtxndof = currentVertex.ndof();
      evt_.vtxchi2 = currentVertex.chi2();
      //primary_vtx         = static_cast<reco::VertexCollection::const_iterator>(&currentVertex);
    }

    // Counting the good vertices
    if( std::abs(currentVertex.z()) < 24.0 &&
	std::abs(currentVertex.position().rho()) < 2.0 &&
	currentVertex.ndof() >= 4 ) {
      evt_.nvtx++;
    }
  } // end vtx loop

  // Quick loop to get number of clusters on layers/disk
  std::fill(evt_.nclu, evt_.nclu + 7, 0);
  std::fill(evt_.npix, evt_.npix + 7, 0);
  if(clusterCollectionHandle.isValid()) for(const auto &clu_set: *clusterCollectionHandle) {
    int layDiskIndex = -1;
    DetId detId(clu_set.id());
    
    if(detId.subdetId() == PixelSubdetector::PixelBarrel)
      layDiskIndex = trackerTopology_ -> pxbLayer(detId.rawId()) - 1;
    else {
      if(detId.subdetId() == PixelSubdetector::PixelEndcap)
  layDiskIndex = trackerTopology_ -> pxfDisk(detId.rawId()) + 3;
      else continue;
    }

    evt_.nclu[layDiskIndex] += clu_set.size();
    if(!npixFromDigiCollection_)
    {
      for(const auto &clu: clu_set) evt_.npix[layDiskIndex] += clu.size();
    }
  }
  if(npixFromDigiCollection_)
  {
    for(const auto& digiDetSet: *digiCollectionHandle)
    {
      int layDiskIndex = -1;
      DetId detId(digiDetSet.detId());
      if(detId.subdetId() == PixelSubdetector::PixelBarrel)
        layDiskIndex = trackerTopology_ -> pxbLayer(detId.rawId()) - 1;
      else {
        if(detId.subdetId() == PixelSubdetector::PixelEndcap)
          layDiskIndex = trackerTopology_ -> pxfDisk(detId.rawId()) + 3;
      else continue;
      }
      evt_.npix[layDiskIndex] += digiDetSet.size();
    }
  }

  // Quick loop to get track numbers
  evt_.ntracks = 0;
  std::fill(evt_.ntrackFPix, evt_.ntrackFPix + 3, 0);
  std::fill(evt_.ntrackBPix, evt_.ntrackBPix + 4, 0);
  for(const auto &pair: *trajTrackCollectionHandle) {
    const edm::Ref<std::vector<Trajectory> > traj = pair.key;
    const reco::TrackRef track                    = pair.val;

    // Discarding tracks without pixel measurements
    if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;
    ++evt_.ntracks;

    for(const auto &measurement: traj -> measurements()) {

      if(!measurement.updatedState().isValid()) continue;
      DetId detId(measurement.recHit() -> geographicalId());

      if(detId.subdetId() == PixelSubdetector::PixelBarrel) {
	evt_.ntrackBPix[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
	if(measurement.recHit() -> getType() == TrackingRecHit::valid)
	  evt_.ntrackBPixvalid[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
      } else if(detId.subdetId() == PixelSubdetector::PixelEndcap) {
	evt_.ntrackFPix[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
	if(measurement.recHit() -> getType() == TrackingRecHit::valid)
	  evt_.ntrackFPixvalid[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
      }

    }
  }

  // Fill the tree
  eventTree_ -> Fill();

}

int PhaseIPixelNtuplizer::getTriggerInfo(const edm::Event& iEvent, const edm::Handle<edm::TriggerResults>& triggerResultsHandle) {
  
  if(!triggerResultsHandle.isValid()) {
    if(isEventFromMc_) return 1; // Assuming that ZeroBias is the first trigger bit
    else               return NOVAL_I;
  }

  // Get the trigger names found in the current event
  int trig = 0;
  const edm::TriggerNames& eventTriggerNames = iEvent.triggerNames(*triggerResultsHandle);
  for(size_t eventNumTrigger = 0; eventNumTrigger < eventTriggerNames.size(); eventNumTrigger++) {

    std::string eventTriggerNameToTest = eventTriggerNames.triggerNames()[eventNumTrigger];

    // Compare current trigger name to the ones found in the config 
    for(size_t configNumTrigger = 0; configNumTrigger < triggerNames_.size(); configNumTrigger++) {
      // If the name starts with the one specified in the configuration
      if(eventTriggerNameToTest.find(triggerNames_[configNumTrigger])) continue;
      // Check: Has the eventNumTrigger-th path accepted the event?
      if(triggerResultsHandle -> accept(eventNumTrigger) == 0) continue;
      trig |= (1 << configNumTrigger);
    }

  }
  return trig;
}

float PhaseIPixelNtuplizer::getPileupInfo
(const edm::Handle<std::vector<PileupSummaryInfo>>& puInfoCollectionHandle) {

  if (!isEventFromMc_) return NOVAL_F;

  // Abandon hope all ye who enter here

  static int reportNum = 0;
  if(!puInfoCollectionHandle.isValid()) {
    if(reportNum++ < 100)
      std::cout << "Warning: The provided pileup info is invalid." << std::endl;
    return NOVAL_F;
  }

  auto zerothPileup = 
    std::find_if(puInfoCollectionHandle -> rbegin(),
		 puInfoCollectionHandle -> rend(),
		 [] (const auto& puInfo) { return puInfo.getBunchCrossing() == 0; });

  if(zerothPileup == puInfoCollectionHandle -> rend()) {
    if(reportNum++ < 100)
      std::cout << "Error: Cannot find the in-time pileup info." << std::endl;
    return NOVAL_F;
  }

  return zerothPileup -> getTrueNumInteractions();

}

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
void PhaseIPixelNtuplizer::getSimhitData
(const std::vector<edm::Handle<edm::PSimHitContainer>>& simhitCollectionHandles) {
  
  int numSimHits = 0;
  for (const auto& simhitCollectionHandle: simhitCollectionHandles)
    numSimHits += simhitCollectionHandle -> size();

  std::cout << "Number of simhits present: " << numSimHits << std::endl;
  for(const auto& simhitCollectionHandle : simhitCollectionHandles) {

    for(const PSimHit& simhit : *simhitCollectionHandle) {

      DetId detId(simhit.detUnitId());
      unsigned int subdetId = detId.subdetId();
      const GeomDetUnit* geomDetUnit      = trackerGeometry_ -> idToDetUnit(detId);
      LocalPoint simhitLocalCoordinates   = simhit.localPosition();
      GlobalPoint simhitGlobalCoordinates = geomDetUnit -> toGlobal(simhitLocalCoordinates);

      if(subdetId == PixelSubdetector::PixelBarrel) {
	static int printCounter = 0; // Another static... Preposterous!
	int layer = trackerTopology_ -> pxbLayer(detId);

	if(printCounter++ < 20)
	  std::cout << __FUNCTION__ << ": Layer: " << layer << " --- " 
		    << simhitLocalCoordinates.x() << ", " 
		    << simhitLocalCoordinates.y() << ", " 
		    << simhitLocalCoordinates.z() << std::endl;

	if(printCounter == 20)
	  std::cout << __FUNCTION__ << "Predefined values for number of simhit reports exceeded." \
	    " Omitting further reports!" << std::endl;

	if(layer == 1) simhitOccupancy_l1 -> Fill(simhitGlobalCoordinates.z(),
						  atan2( simhitGlobalCoordinates.y(),
							 simhitGlobalCoordinates.x()));

	if(layer == 2) simhitOccupancy_l2 -> Fill(simhitGlobalCoordinates.z(),
						  atan2( simhitGlobalCoordinates.y(),
							 simhitGlobalCoordinates.x()));
	
	if(layer == 3) simhitOccupancy_l3 -> Fill(simhitGlobalCoordinates.z(), 
						  atan2( simhitGlobalCoordinates.y(),
							 simhitGlobalCoordinates.x()));

	if(layer == 4) simhitOccupancy_l4 -> Fill(simhitGlobalCoordinates.z(),
						  atan2( simhitGlobalCoordinates.y(),
							 simhitGlobalCoordinates.x()));
      } else if(subdetId == PixelSubdetector::PixelEndcap) {
	simhitOccupancy_fwd -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
      }
    } // end loop on simhits
  } // end loop on collections

}
#endif

void PhaseIPixelNtuplizer::getDigiData
(const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollectionHandle) {

  int digiIndexInEvent = 0;

  for(const auto& digiDetSet: *digiCollectionHandle) {

    DetId detId(digiDetSet.detId());

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
    unsigned int subdetId = detId.subdetId();
    const GeomDetUnit* geomDetUnit      = trackerGeometry_ -> idToDetUnit(detId);
#endif

    for(const auto& digi: digiDetSet)	{
      digi_.init();
      digi_.i   = digiIndexInEvent++;
      digi_.row = digi.row();
      digi_.col = digi.column();
      digi_.adc = digi.adc();
      
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
      LocalPoint digiLocalCoordinates(digi_.row, digi_.col, 0);
      GlobalPoint digiGlobalCoordinates = geomDetUnit -> toGlobal(digiLocalCoordinates);
      
      if(subdetId == PixelSubdetector::PixelBarrel) {
	int layer = trackerTopology_ -> pxbLayer(detId);

	if(layer == 1) digiOccupancy_l1 -> Fill(digiGlobalCoordinates.z(),
						atan2( digiGlobalCoordinates.y(),
						       digiGlobalCoordinates.x()));

	if(layer == 2) digiOccupancy_l2 -> Fill(digiGlobalCoordinates.z(),
						atan2( digiGlobalCoordinates.y(),
						       digiGlobalCoordinates.x()));

	if(layer == 3) digiOccupancy_l3 -> Fill(digiGlobalCoordinates.z(),
						atan2( digiGlobalCoordinates.y(),
						       digiGlobalCoordinates.x()));

	if(layer == 4) digiOccupancy_l4 -> Fill(digiGlobalCoordinates.z(),
						atan2( digiGlobalCoordinates.y(),
						       digiGlobalCoordinates.x()));

      } else if(subdetId == PixelSubdetector::PixelEndcap) {
	digiOccupancy_fwd -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
      }
#endif

      digiTree_ -> Fill();
    }
  }
}

void PhaseIPixelNtuplizer::getClustData
(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle) {

  // Cluster info
  // Trying to access the clusters
  clu_.init();

  if(!clusterCollectionHandle.isValid())
    handleDefaultError("data_access", "data_access", "Failed to fetch the clusters.");

  const edmNew::DetSetVector<SiPixelCluster>& currentClusterCollection = *clusterCollectionHandle;

  // Looping on clusters with the same location
  using clustCollIt_t = edmNew::DetSetVector<SiPixelCluster>::const_iterator;
  for( clustCollIt_t currentClusterSetIt = currentClusterCollection.begin();
       currentClusterSetIt != currentClusterCollection.end(); ++currentClusterSetIt) {

    const auto& currentClusterSet = *currentClusterSetIt;
    DetId detId(currentClusterSet.id());
    unsigned int subdetId = detId.subdetId();

    // Take only pixel clusters
    if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
      continue;

    using clustSetIt_t = edmNew::DetSet<SiPixelCluster>::const_iterator;
    for(clustSetIt_t currentClusterIt = currentClusterSet.begin();
	currentClusterIt != currentClusterSet.end(); ++currentClusterIt) {

      // The number of saved clusters can be downscaled to save space
      if(++nCluster_ % clusterSaveDownscaling_ != 0) continue;
      const auto& currentCluster = *currentClusterIt;

      // Serial num of cluster in the given module
      clu_.i = currentClusterIt - currentClusterSet.begin();
      // Set if there is a valid hits
      // clu_.edge;
      // clu_.badpix;
      // clu_.tworoc;

      // Module information
      getModuleData(clu_.mod,    0, detId);
      getModuleData(clu_.mod_on, 1, detId);
      getRocData   (clu_.mod,    0, detId, &currentCluster);
      getRocData   (clu_.mod_on, 1, detId, &currentCluster);

      const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);

      LocalPoint clustLocalCoordinates;
      std::tie(clustLocalCoordinates, std::ignore, std::ignore) = 
	pixelClusterParameterEstimator_ -> getParameters(currentCluster, *geomDetUnit);
      GlobalPoint clustGlobalCoordinates = geomDetUnit -> toGlobal(clustLocalCoordinates);

      // Position and size
      clu_.x     = currentCluster.x();
      clu_.y     = currentCluster.y();
      clu_.lx    = clustLocalCoordinates.x();
      clu_.ly    = clustLocalCoordinates.y();
      clu_.glx   = clustGlobalCoordinates.x();
      clu_.gly   = clustGlobalCoordinates.y();
      clu_.glz   = clustGlobalCoordinates.z();
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

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
      if(clu_.mod_on.det == 0) {
	const auto& layer = clu_.mod_on.layer;
	if(layer == 1) {
	  clustOccupancyROCBins_l1 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
	  clustOccupancy_fwd       -> Fill(clu_.glz, atan2(clu_.gly, clu_.glx));
	}
	if(layer == 2) {
	  clustOccupancyROCBins_l2 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
	  clustOccupancy_l1        -> Fill(clu_.glz, atan2(clu_.gly, clu_.glx));
	}
	if(layer == 3) {
	  clustOccupancyROCBins_l3 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
	  clustOccupancy_l2        -> Fill(clu_.glz, atan2(clu_.gly, clu_.glx));
	}
	if(layer == 4) {
	  clustOccupancyROCBins_l4 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
	  clustOccupancy_l3        -> Fill(clu_.glz, atan2(clu_.gly, clu_.glx));
	}
      }
      if(clu_.mod_on.det == 1) {
	clustOccupancyROCBins_fwd -> Fill(clu_.mod_on.disk_ring_coord, clu_.mod_on.blade_panel_coord);
	clustOccupancy_l4         -> Fill(clu_.glz, atan2(clu_.gly, clu_.glx));
      }
#endif

      clustTree_ -> Fill();
    }
  }
}


std::map<reco::TrackRef, TrackData> 
PhaseIPixelNtuplizer::getTrackData( const edm::Handle<reco::VertexCollection>& vertexCollectionHandle,
                                    const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle,
                                    const edm::Handle<reco::MuonCollection>& muonCollectionHandle,
                                    const edm::ESHandle<TransientTrackBuilder>& trackBuilderHandle) {
  
  std::map<reco::TrackRef, TrackData> trackDataCollection;
  int trackIndex = 0;

  nTrackSave_ = nTrack_;
  for(const auto& currentTrackKeypair: *trajTrackCollectionHandle) {

    const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
    const reco::TrackRef                    track = currentTrackKeypair.val;

    // Match global and tracker muon inner tracks
    bool saveMuon = false;
    reco::Muon muon;
    for (const reco::Muon& mu : *muonCollectionHandle) {
      if ((keepAllTrackerMuons_&&mu.isTrackerMuon()) || (keepAllGlobalMuons_&&mu.isGlobalMuon())) {
	if (NtuplizerHelpers::sameTrack(track, mu.innerTrack())) {
	  saveMuon = true;
	  muon = mu;
	}
      }
    }
    if (++nTrack_ % trackSaveDownscaling_ != 0 && !saveMuon) continue;

    TrackData* trackField;

    // Create new TrackData instance, when this trackRef is a new one,
    // set track properties and zero out counters
    auto trackFieldIt = trackDataCollection.find(track);

    if(trackFieldIt == trackDataCollection.end()) {

      // Initialize track data
      TrackData newTrackData;

      // Zeroing counters
      std::fill(newTrackData.pixhit,      newTrackData.pixhit      + 2, 0);
      std::fill(newTrackData.validpixhit, newTrackData.validpixhit + 2, 0);
      std::fill(newTrackData.fpix,        newTrackData.fpix        + 3, 0);
      std::fill(newTrackData.bpix,        newTrackData.bpix        + 4, 0);
      std::fill(newTrackData.validfpix,   newTrackData.validfpix   + 3, 0);
      std::fill(newTrackData.validbpix,   newTrackData.validbpix   + 4, 0);
      newTrackData.strip          = 0;
	
      // Closest vertex			
      reco::VertexCollection::const_iterator closestVtx =
	NtuplizerHelpers::findClosestVertexToTrack(track, vertexCollectionHandle, 10);

      if(closestVtx != vertexCollectionHandle -> end()) {
	newTrackData.fromVtxNtrk = closestVtx -> tracksSize();
	newTrackData.d0          = track -> dxy(closestVtx -> position()) * -1.0;
	newTrackData.dz          = track -> dz (closestVtx -> position());
      }

      // Basic track quantities
      newTrackData.i       = trackIndex++;
      newTrackData.quality = track -> qualityMask();
      newTrackData.pt      = track -> pt();
      newTrackData.p       = track -> p();
      newTrackData.eta     = track -> eta();
      newTrackData.theta   = track -> theta();
      newTrackData.phi     = track -> phi();

      // muon info
      // https://cmssdt.cern.ch/lxr/source/DataFormats/MuonReco/interface/Muon.h
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_Identification
      if (saveMuon) {
        // ID/ISO
        newTrackData.muon_global        = muon.isGlobalMuon();
        newTrackData.muon_intime        = muon.passed(reco::Muon::InTimeMuon);
        newTrackData.muon_id_soft       = muon.passed(reco::Muon::SoftCutBasedId);
        newTrackData.muon_id_loose      = muon.passed(reco::Muon::CutBasedIdLoose);
        newTrackData.muon_id_medium     = muon.passed(reco::Muon::CutBasedIdMedium);
        newTrackData.muon_id_tight      = muon.passed(reco::Muon::CutBasedIdTight);
        newTrackData.muon_id_mva_soft   = muon.passed(reco::Muon::SoftMvaId);
        newTrackData.muon_id_mva        = muon.passed(reco::Muon::MvaLoose);
        newTrackData.muon_id_mva       += muon.passed(reco::Muon::MvaMedium);
        newTrackData.muon_id_mva       += muon.passed(reco::Muon::MvaTight); 
        newTrackData.muon_id_trigger    = muon.passed(reco::Muon::TriggerIdLoose);
        newTrackData.muon_iso_pf        = muon.passed(reco::Muon::PFIsoVeryLoose);
        newTrackData.muon_iso_pf       += muon.passed(reco::Muon::PFIsoLoose);
        newTrackData.muon_iso_pf       += muon.passed(reco::Muon::PFIsoMedium);
        newTrackData.muon_iso_pf       += muon.passed(reco::Muon::PFIsoTight);
        newTrackData.muon_iso_pf       += muon.passed(reco::Muon::PFIsoVeryTight);
        newTrackData.muon_iso_pf       += muon.passed(reco::Muon::PFIsoVeryVeryTight);
        newTrackData.muon_iso_tk        = muon.passed(reco::Muon::TkIsoLoose);
        newTrackData.muon_iso_tk       += muon.passed(reco::Muon::TkIsoTight);
        newTrackData.muon_iso_mini      = muon.passed(reco::Muon::MiniIsoLoose);
        newTrackData.muon_iso_mini     += muon.passed(reco::Muon::MiniIsoMedium);
        newTrackData.muon_iso_mini     += muon.passed(reco::Muon::MiniIsoTight);
        newTrackData.muon_iso_mini     += muon.passed(reco::Muon::MiniIsoVeryTight);
        newTrackData.muon_iso_multi     = muon.passed(reco::Muon::MultiIsoLoose);
        newTrackData.muon_iso_multi    += muon.passed(reco::Muon::MultiIsoMedium);
        // stuff only added in latest releases
#if ADD_NEW_MUON_SELECTORS > 0
        newTrackData.muon_id_mva       += muon.passed(reco::Muon::MvaVTight);
        newTrackData.muon_id_mva       += muon.passed(reco::Muon::MvaVVTight);
        newTrackData.muon_id_mva_lowpt  = muon.passed(reco::Muon::LowPtMvaLoose);
        newTrackData.muon_id_mva_lowpt += muon.passed(reco::Muon::LowPtMvaMedium);
        newTrackData.muon_iso_puppi     = muon.passed(reco::Muon::PuppiIsoLoose);
        newTrackData.muon_iso_puppi    += muon.passed(reco::Muon::PuppiIsoMedium);
        newTrackData.muon_iso_puppi    += muon.passed(reco::Muon::PuppiIsoTight);
#endif
        // IP - Taken from PAT and apply it to the closest vertex
        // PV2D
        double dz  = std::abs(muon.muonBestTrack()->dz(closestVtx->position()));
        double dxy  = std::abs(muon.muonBestTrack()->dxy(closestVtx->position()));
        newTrackData.muon_ip_2d         = ((dz<0.5) && (dxy<0.2)) + ((dz<0.1) && (dxy<0.05));
        // This thing won't compile no matter what I try
        //reco::TransientTrack tt = trackBuilderHandle->build(track);
        //std::pair<bool, Measurement1D> ip2d = IPTools::signedTransverseImpactParameter(tt, GlobalVector(track->px(), track->py(), track->pz()), *closestVtx);
        //double dxy = ip2d.second.value();
        // PV3D
        //std::pair<bool, Measurement1D> ip3d = IPTools::signedImpactParameter3D(tt, direction, *closestVtx);
        //double dB3D  = std::abs(ip3d.second.value());
        //double edB3D = std::abs(ip3d.second.error());
        //double sip3D = edB3D > 0 ? dB3D / edB3D : 0.0;
        //newTrackData.muon_ip_3dsig      = (sip3D<8) + (sip3D<4);
        // Muon timing
        // https://cmssdt.cern.ch/lxr/source/DataFormats/MuonReco/interface/MuonTime.h
        // https://cmssdt.cern.ch/lxr/source/DataFormats/MuonReco/src/MuonSelectors.cc#1047
        newTrackData.muon_time_comb_dir   = muon.time().direction();
        newTrackData.muon_time_comb_ndof  = muon.time().nDof;
        newTrackData.muon_time_rpc_dir    = muon.rpcTime().direction();
        newTrackData.muon_time_rpc_ndof   = muon.rpcTime().nDof;
        if (muon.time().direction()==reco::MuonTime::OutsideIn) {
          newTrackData.muon_time_comb = muon.time().timeAtIpOutIn;
        } else if (muon.time().direction()==reco::MuonTime::InsideOut) {
          newTrackData.muon_time_comb = muon.time().timeAtIpInOut;
        }
        if (muon.rpcTime().nDof > 1 && std::abs(muon.rpcTime().timeAtIpInOutErr) < 0.001) {
          if (muon.time().direction()==reco::MuonTime::OutsideIn) {
            newTrackData.muon_time_rpc = muon.rpcTime().timeAtIpOutIn;
          } else if (muon.time().direction()==reco::MuonTime::InsideOut) {
            newTrackData.muon_time_rpc = muon.rpcTime().timeAtIpInOut;
          }
        }
      } // end muons

      trackFieldIt = trackDataCollection.insert({track, std::move(newTrackData)}).first;
    }

    trackField = &(trackFieldIt -> second);
    // Counted here:
    // barrel hits, valid barrel hits, forward hits, valid forward hits,
    // top of detector hits, top of detector hits, strip hits
    // Check measurement validity

    for(const TrajectoryMeasurement& measurement: traj -> measurements()) {

      if(!measurement.updatedState().isValid()) continue;

      const auto& hit = measurement.recHit();
	
      // Det id
      DetId detId = hit -> geographicalId();
      uint32_t subDetId = (detId.subdetId());

      // Counting the strip hits
      if(hit -> isValid()) {
	if( subDetId == StripSubdetector::TIB || subDetId == StripSubdetector::TOB ||
	    subDetId == StripSubdetector::TID || subDetId == StripSubdetector::TEC) {
	  ++trackField -> strip;
	}
      }

      // Looking for pixel hits
      if(!NtuplizerHelpers::detidIsOnPixel(detId)) continue;

      // Looking for valid and missing hits
      const int& validhit = hit -> getType() == TrackingRecHit::valid;

      // const int& missing  = hit -> getType() == TrackingRecHit::missing;
      // Module info
      ModuleData mod;
      getModuleData(mod, 0, detId);

      // Forward and barrel hits
      if(subDetId == PixelSubdetector::PixelBarrel) {
	trackField -> bpix[mod.layer - 1]++;
	if(validhit) trackField -> validbpix[mod.layer - 1]++;
      } else if(subDetId == PixelSubdetector::PixelEndcap) {
	trackField -> fpix[mod.disk - 1]++;
	if(validhit) trackField -> validfpix[mod.disk - 1]++;
      }

      // Fetch the hit
      const SiPixelRecHit* pixhit = static_cast<const SiPixelRecHit*>(hit -> hit());

      // Check hit qualty
      if(pixhit) {

	// Position measurements
	TrajectoryStateOnSurface trajStateOnSurface =
	  NtuplizerHelpers::getTrajectoryStateOnSurface(measurement);

	if(!trajStateOnSurface.isValid()) continue;

	auto globalPosition      = trajStateOnSurface.globalPosition();
	// const float& glx = globalPosition.x();
	const float& gly = globalPosition.y();
	// const float& glz = globalPosition.z();

	// Count top-of-detector tracks traj measurements
	if(0 <= gly && gly != NOVAL_F) {
	  trackField -> pixhit[0] += 1;
	  if(validhit) trackField -> validpixhit[0] += 1;
	}

	// Count bottom-of-detector tracks traj measurements
	if(gly < 0 && gly != NOVAL_F) {
	  trackField -> pixhit[1] += 1;
	  if(validhit) trackField -> validpixhit[1] += 1;
	}

      }

    } // end loop on measurements

  } // end loop on trajectories

  if(saveTrackTree_) {
    for(const auto& pair: trackDataCollection) {
      track_ = pair.second;
      trackTree_ -> Fill();
    }
  }

  return trackDataCollection;

}

void
PhaseIPixelNtuplizer::getTrajTrackData( const edm::Handle<reco::VertexCollection>& vertexCollectionHandle,
                                        const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle,
                                        const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle,
                                        const edm::Handle<reco::MuonCollection>& muonCollectionHandle,
                                        const edm::ESHandle<TransientTrackBuilder>& trackBuilderHandle,
                                        const edm::Handle<edm::SimTrackContainer>& simTracksHandle) {
  
  std::map<reco::TrackRef, TrackData> trackDataCollection
    (getTrackData(vertexCollectionHandle, trajTrackCollectionHandle, muonCollectionHandle, trackBuilderHandle));

  // Trajectory measurement loop
  unsigned long long int nTrack = nTrackSave_;
  for(const auto& currentTrackKeypair: *trajTrackCollectionHandle) {

    const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
    const reco::TrackRef                    track = currentTrackKeypair.val;

    // Match global and tracker muon inner tracks
    bool saveMuon = false;
    for (const reco::Muon& mu : *muonCollectionHandle) {
      if ((keepAllTrackerMuons_&&mu.isTrackerMuon()) || (keepAllGlobalMuons_&&mu.isGlobalMuon())) {
	if (NtuplizerHelpers::sameTrack(track, mu.innerTrack())) saveMuon = true;
      }
    }
    if (++nTrack % trackSaveDownscaling_ != 0 && !saveMuon) continue;

    // Discarding tracks without pixel measurements
    if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;

    track_ = trackDataCollection.at(track);
    const auto& trajectoryMeasurements = traj -> measurements();

    // First, save trajectory measurement data from the original track
    auto firstLayer1TrajMeasurementIt = trajectoryMeasurements.end();
    for(auto measurementIt = trajectoryMeasurements.begin();
	measurementIt != trajectoryMeasurements.end(); measurementIt++) {
      ModuleData mod; getModuleData(mod, 1, measurementIt->recHit() -> geographicalId());
      if (mod.det == 0 && mod.layer == 1) {
        if(saveNonPropagatedExtraTrajTree_) {
          // Save non-propagated L1 hits as an additional tree
          checkAndSaveTrajMeasurementData(*measurementIt, clusterCollectionHandle,
                                          trajTrackCollectionHandle, track,
                                          simTracksHandle, nonPropagatedExtraTrajTree_);
        }
        firstLayer1TrajMeasurementIt = measurementIt;
      } else {
        checkAndSaveTrajMeasurementData(*measurementIt, clusterCollectionHandle,
                                        trajTrackCollectionHandle, track,
                                        simTracksHandle, trajTree_);
      }
    }

    // Then, propagate hits from the previous Layer 2/Disk 1 valid hit to Layer 1

    // Check there are hits before the first layer 1 traj. measurement
    // Trajmeasurements go from outside in, last hit can only be Layer 1
    if(firstLayer1TrajMeasurementIt == trajectoryMeasurements.begin()) continue;
    auto lastNonLayer1TrajMeasurementIt = std::prev(firstLayer1TrajMeasurementIt);

    // Check if the last non-layer1 traj measurement is valid
    auto lastNonLayer1TrajMeasurementRecHit = lastNonLayer1TrajMeasurementIt -> recHit();
    if (lastNonLayer1TrajMeasurementRecHit == nullptr)
      std::cout << "Invalid rechit pointer." << std::endl;
    if(!(lastNonLayer1TrajMeasurementRecHit -> isValid())) continue;

    std::vector<TrajectoryMeasurement> extrapolatedHitsOnLayer1
      (getLayer1ExtrapolatedHitsFromMeas(*lastNonLayer1TrajMeasurementIt));

    // Save  all propagated hits
    //for(auto measurementIt = extrapolatedHitsOnLayer1.begin();
    //    measurementIt != extrapolatedHitsOnLayer1.end(); measurementIt++)
    //  checkAndSaveTrajMeasurementData(*measurementIt, clusterCollectionHandle,
    //    			      trajTrackCollectionHandle, track,
    //                                  simTracksHandle, trajTree_);
    
    // Save first hit along trajectory
    //std::cout<<"Nmeas="<<extrapolatedHitsOnLayer1.size()<<std::endl;
    if (!extrapolatedHitsOnLayer1.empty()) 
      checkAndSaveTrajMeasurementData(extrapolatedHitsOnLayer1.front(), clusterCollectionHandle,
                                      trajTrackCollectionHandle, track,
                                      simTracksHandle, trajTree_);
  } // end loop on trajectories
}

// currently unused
std::tuple<std::vector<TrajectoryMeasurement>::const_iterator, float>
PhaseIPixelNtuplizer::findMatchingTrajMeasurement
( const GlobalPoint& referencePoint, 
  const ModuleData& referenceModInfo,
  const std::vector<TrajectoryMeasurement>& possibleMatches)
{

  using NtuplizerHelpers::areIdenticalModules;
  auto isMeasurementOnReferenceModule = [&] (const TrajectoryMeasurement& measurement) {
    ModuleData mod; 
    getModuleData(mod, 1, measurement.recHit() -> geographicalId().rawId());
    return areIdenticalModules(mod, referenceModInfo);
  };

  auto bestMatch = std::find_if(possibleMatches.begin(), possibleMatches.end(), isMeasurementOnReferenceModule);
  if(bestMatch == possibleMatches.end()) return std::make_tuple(bestMatch, NOVAL_F);

  float closestDistance = trajMeasGlobalPointDistance(*bestMatch, referencePoint);
  for(auto measurementsIt = bestMatch + 1; 
      measurementsIt != possibleMatches.end(); ++measurementsIt) {

    if(!isMeasurementOnReferenceModule(*measurementsIt)) continue;

    float distance = trajMeasGlobalPointDistance(*measurementsIt, referencePoint);
    if(distance < closestDistance) {
      closestDistance = distance;
      bestMatch = measurementsIt;
    }

  }

  return std::make_tuple(bestMatch, closestDistance);

}

void
PhaseIPixelNtuplizer::getTrajTrackDataCosmics(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle,
                                              const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle,
                                              const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle,
                                              const edm::Handle<reco::MuonCollection>& muonCollectionHandle,
                                              const edm::ESHandle<TransientTrackBuilder>& trackBuilderHandle,
                                              const edm::Handle<edm::SimTrackContainer>& simTracksHandle) {
  
  std::map<reco::TrackRef, TrackData> trackDataCollection
    (getTrackData(vertexCollectionHandle, trajTrackCollectionHandle, muonCollectionHandle, trackBuilderHandle));

  // Trajectory measurement loop
  unsigned long long int nTrack = nTrackSave_;
  for(const auto& currentTrackKeypair: *trajTrackCollectionHandle) {

    const edm::Ref<std::vector<Trajectory> > traj  = currentTrackKeypair.key;
    const reco::TrackRef                     track = currentTrackKeypair.val;

    // Match global and tracker muon inner tracks
    bool saveMuon = false;
    for (const reco::Muon& mu : *muonCollectionHandle) {
      if ((keepAllTrackerMuons_&&mu.isTrackerMuon()) || (keepAllGlobalMuons_&&mu.isGlobalMuon())) {
	if (NtuplizerHelpers::sameTrack(track, mu.innerTrack())) saveMuon = true;
      }
    }
    if (++nTrack % trackSaveDownscaling_ != 0 && !saveMuon) continue;

    // Discarding tracks without pixel measurements
    if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;

    track_ = trackDataCollection.at(track);
    const std::vector<TrajectoryMeasurement>& trajectoryMeasurements = traj -> measurements();

    for(auto measurementIt = trajectoryMeasurements.begin();
	measurementIt != trajectoryMeasurements.end(); measurementIt++)
      checkAndSaveTrajMeasurementData(*measurementIt, clusterCollectionHandle,
				      trajTrackCollectionHandle, track,
                                      simTracksHandle, trajTree_);

  }
}

void PhaseIPixelNtuplizer::checkAndSaveTrajMeasurementData
( const TrajectoryMeasurement& measurement,
  const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle,
  const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle,
  const reco::TrackRef& track,
  const edm::Handle<edm::SimTrackContainer>& simTracksHandle,
  TTree* targetTree) {

  traj_.init();

  // Check if the measurement infos can be read
  if(!measurement.updatedState().isValid()) return;

  TransientTrackingRecHit::ConstRecHitPointer recHit = measurement.recHit();

  // Only looking for pixel hits
  DetId detId = recHit -> geographicalId();
  if(!NtuplizerHelpers::detidIsOnPixel(detId)) return;

  // Skipping hits with undeterminable positions
  TrajectoryStateOnSurface trajStateOnSurface = 
    NtuplizerHelpers::getTrajectoryStateOnSurface(measurement);

  if(!(trajStateOnSurface.isValid())) return;

  // Save module data
  getModuleData(traj_.mod,    0, detId);
  getModuleData(traj_.mod_on, 1, detId);

  // Position measurements
  // Looking for valid and missing hits
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
  traj_.validhit = recHit -> getType() == TrackingRecHit::valid;
  traj_.missing  = recHit -> getType() == TrackingRecHit::missing;
  traj_.inactive = recHit -> getType() == TrackingRecHit::inactive;
  
  // Distance of nearest other track
  traj_.dr_trk = 9999;
  for(const auto& pair : *trajTrackCollectionHandle) {
    const reco::TrackRef track2 = pair.val;
    if (track!=track2) {
      double deta = track2->eta()-track->eta();
      double dphi = std::abs(track2->phi()-track->phi());
      dphi = dphi<3.141592654 ? dphi : 6.283185307 - dphi;
      double dr = std::sqrt(deta*deta+dphi*dphi);
      if (dr<traj_.dr_trk) traj_.dr_trk = dr;
    }
  }

  // Sim hit matching and residuals
#if ADD_SIM_INFO > 0
  if (isEventFromMc_) {
    //std::vector<PSimHit> matched = trackerHitAssociator_->associateHit(*recHit);
    std::vector<PSimHit> matched = pixelHitAssociator_->  associateHit(*recHit, localPosition);
    if (matched.size()>0) {
      float mindist = 999999;
      PSimHit closest;
      unsigned int matchid = -9999;
      // Find the vector of SimHits matching this RecHit
      // Print out the SimHit positions and residuals
      for (auto const& m : matched) {
        //std::cout << " simtrack ID = " << m.trackId() << "                            Simhit Pos = " << m.localPosition() << std::endl;
        // Seek the smallest residual
        float dist = (localPosition - m.localPosition()).mag();
        if (dist < mindist) {
          mindist = dist;
          closest = m;
          matchid = m.trackId();
        }
      }
      //std::cout << " Closest Simhit = " << closest.localPosition();
      if (mindist != 999999) {
        traj_.dx_simhit = localPosition.x() - closest.localPosition().x();
        traj_.dy_simhit = localPosition.y() - closest.localPosition().y();
        //std::cout << ", diff(x,y) = (" << traj_.dx_simhit << ", " << traj_.dy_simhit << ")";
        //std::cout << ", |diff| = " << mindist << std::endl;
      }
      
      // Sim track (TrackingParticles) matching
      // https://github.com/cms-sw/cmssw/blob/master/Validation/RecoTrack/plugins/TrackingNtuple.cc#L3403-L3417
      for (const auto& simtrack : *(simTracksHandle.product())) if (simtrack.trackId()==matchid) {
        //std::cout << "sim track      pt=" << simtrack.momentum().pt() << " eta=" << simtrack.momentum().eta() << " phi=" << simtrack.momentum().phi() << std::endl;
        //std::cout << "--> reco track pt=" << track->pt() << " eta=" << track->eta() << " phi=" << track->phi() << std::endl<<std::endl<<std::endl;
        double deta = simtrack.momentum().eta()-track->eta();
        double dphi = std::abs(simtrack.momentum().phi()-track->phi());
        dphi = dphi<3.141592654 ? dphi : 6.283185307 - dphi;
        traj_.dr_simtrk = std::sqrt(deta*deta+dphi*dphi);
      }
    }
  }
#endif

  // Track local angles
  LocalTrajectoryParameters trajectoryParameters = trajStateOnSurface.localParameters();
  auto trajectoryMomentum = trajectoryParameters.momentum();
  LocalVector localTrackDirection = trajectoryMomentum / trajectoryMomentum.mag();
  traj_.alpha = atan2(localTrackDirection.z(), localTrackDirection.x());
  traj_.beta  = atan2(localTrackDirection.z(), localTrackDirection.y());

  // Roc informations
  if(traj_.validhit == 1) {
    getRocData(traj_.mod,    0, static_cast<const SiPixelRecHit*>(recHit -> hit()));
    getRocData(traj_.mod_on, 1, static_cast<const SiPixelRecHit*>(recHit -> hit()));
  }

  // Get correct coordinates for non-valid hits
  else {
    int row = std::max(0, std::min(159, static_cast<int>((localPosition.x() / 0.81 + 1) * 80)));
    int col = std::max(0, std::min(415, static_cast<int>((localPosition.y() / 0.81 + 4) * 52)));
    PixelDigi missing_hit(row, col, 0);
    getRocData(traj_.mod,    0, detId, &missing_hit);
    getRocData(traj_.mod_on, 1, detId, &missing_hit);
  }

  const SiPixelCluster* clust = nullptr;
  // Read associated cluster parameters
  if(recHit -> isValid() && recHit -> hit() != 0) {

    const SiPixelRecHit *hit = static_cast<const SiPixelRecHit*>(recHit -> hit());
    traj_.clu.edge   = hit -> isOnEdge() ? 1 : 0;
    traj_.clu.badpix = hit -> hasBadPixels() ? 1 : 0;
    traj_.clu.tworoc = hit -> spansTwoROCs() ? 1 : 0;
    clust = hit -> cluster().get();

  } else {

    if(clusterCollectionHandle.isValid()) {

      const edmNew::DetSetVector<SiPixelCluster>::const_iterator clustersOnDet =
	clusterCollectionHandle -> find(detId);

      if(clustersOnDet != clusterCollectionHandle -> end())
	clust = getClosestClusterOnDetSetToPoint(*clustersOnDet, localPosition);

    }

  }

  if(clust != nullptr) {

    const GeomDetUnit* geomDetUnit = recHit -> detUnit();
    LocalPoint clustLocalCoordinates;
    std::tie(clustLocalCoordinates, std::ignore, std::ignore) = 
      pixelClusterParameterEstimator_ -> getParameters(*clust, *geomDetUnit);
    GlobalPoint clustGlobalCoordinates = geomDetUnit -> toGlobal(clustLocalCoordinates);

    traj_.clu.charge = clust -> charge() / 1000.0f;
    traj_.clu.size   = clust -> size();
    traj_.clu.sizeX  = clust -> sizeX();
    traj_.clu.sizeY  = clust -> sizeY();
    traj_.clu.x      = clust -> x();
    traj_.clu.y      = clust -> y();
    traj_.clu.lx     = clustLocalCoordinates.x();
    traj_.clu.ly     = clustLocalCoordinates.y();
    traj_.clu.glx    = clustGlobalCoordinates.x();
    traj_.clu.gly    = clustGlobalCoordinates.y();
    traj_.clu.glz    = clustGlobalCoordinates.z();

    for(int i = 0; i < clust -> size() && i < 1000; i++) {
      traj_.clu.adc[i]    = static_cast<float>(clust -> pixelADC()[i]) / 1000.0f;
      traj_.clu.pix[i][0] = ((clust -> pixels())[i]).x;
      traj_.clu.pix[i][1] = ((clust -> pixels())[i]).y;
    }

    traj_.norm_charge = traj_.clu.charge * 
      sqrt(1.0f / (1.0f / pow(tan(traj_.alpha), 2) + 1.0f / pow(tan(traj_.beta), 2) + 1.0f));

    //traj_.dx_cl       = clustLocalCoordinates.x() - traj_.lx;
    //traj_.dy_cl       = clustLocalCoordinates.y() - traj_.ly;
    //traj_.d_cl        = std::sqrt(traj_.dx_cl * traj_.dx_cl + traj_.dy_cl * traj_.dy_cl);

    // Implement 2nd cluster distance same way as in TimingStudy
    float dx_cl[2] = {NOVAL_F, NOVAL_F};
    float dy_cl[2] = {NOVAL_F, NOVAL_F};
    float d_cl[2]  = {NOVAL_F, NOVAL_F};
    findClosestClusters(clusterCollectionHandle, recHit->geographicalId().rawId(),
			traj_.lx, traj_.ly, dx_cl, dy_cl);
    for (size_t i=0; i<2; i++)
      if (dx_cl[i]!=NOVAL_F)
	d_cl[i]=sqrt(dx_cl[i]*dx_cl[i]+dy_cl[i]*dy_cl[i]);

    traj_.dx_cl  = dx_cl[0];
    traj_.dy_cl  = dy_cl[0];
    traj_.d_cl   = d_cl[0];
    traj_.dx_cl2 = dx_cl[1];
    traj_.dy_cl2 = dy_cl[1];
    traj_.d_cl2  = d_cl[1];

  } else traj_.clu.init();

  // Get closest other traj measurement
  NtuplizerHelpers::getClosestTrajMeasDistance(measurement, track, trajTrackCollectionHandle, traj_.d_tr, traj_.dx_tr, traj_.dy_tr);
  traj_.hit_near = (traj_.d_tr < 0.5); // 5 mm
  traj_.clust_near = (traj_.d_cl != NOVAL_F && traj_.d_cl < HIT_CLUST_NEAR_CUT_VAL);

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
  if(traj_.mod_on.det == 0) {
    const auto& layer = traj_.mod_on.layer;
    if(layer == 1) {
      rechitOccupancyROCBins_l1 -> Fill(traj_.mod_on.module_coord, traj_.mod_on.ladder_coord);
      rechitOccupancy_l1        -> Fill(traj_.glz, atan2(traj_.gly, traj_.glx));
    }
    if(layer == 2) {
      rechitOccupancyROCBins_l2 -> Fill(traj_.mod_on.module_coord, traj_.mod_on.ladder_coord);
      rechitOccupancy_l2        -> Fill(traj_.glz, atan2(traj_.gly, traj_.glx));
    }
    if(layer == 3) {
      rechitOccupancyROCBins_l3 -> Fill(traj_.mod_on.module_coord, traj_.mod_on.ladder_coord);
      rechitOccupancy_l3        -> Fill(traj_.glz, atan2(traj_.gly, traj_.glx));
    }
    if(layer == 4) {
      rechitOccupancyROCBins_l4 -> Fill(traj_.mod_on.module_coord, traj_.mod_on.ladder_coord);
      rechitOccupancy_l4         -> Fill(traj_.glz, atan2(traj_.gly, traj_.glx));
    }
  } else if(traj_.mod_on.det == 1) {
    rechitOccupancyROCBins_fwd -> Fill(traj_.mod_on.disk_ring_coord, 
				       traj_.mod_on.blade_panel_coord);
    rechitOccupancy_fwd       -> Fill(traj_.glz, atan2(traj_.gly, traj_.glx));
  }
#endif

  traj_.pass_effcuts = getTrajMeasurementEfficiencyQualification(measurement) != EXCLUDED;

  // Filling the tree
  targetTree -> Fill();

}

std::vector<TrajectoryMeasurement> PhaseIPixelNtuplizer::getLayer1ExtrapolatedHitsFromMeas(const TrajectoryMeasurement& trajMeasurement)
{

  // Last layer 2 or disk 1 mesurement is to be propagated to layer 1 if possible
  // Only propagating valid measurements
  std::unique_ptr<LayerMeasurements> layerMeasurements
    (new LayerMeasurements(*measurementTracker_, *measurementTrackerEvent_));

  const DetLayer* pixelBarrelLayer1 = 
    measurementTracker_ -> geometricSearchTracker() -> pixelBarrelLayers().front();
  
  return layerMeasurements -> measurements(*pixelBarrelLayer1, trajMeasurement.updatedState(),
  					   *trackerPropagator_, *chi2MeasurementEstimator_);
}

PhaseIPixelNtuplizer::TrajectoryMeasurementEfficiencyQualification PhaseIPixelNtuplizer::getTrajMeasurementEfficiencyQualification(const TrajectoryMeasurement& t_measurement)
{
  // Nvtx Cut
  if(!(VERTEX_NUMTRACK_CUT_VAL < track_.fromVtxNtrk)) return EXCLUDED;
  // Zerobias cut
  if(!(evt_.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT)) return EXCLUDED;
  // Federr cut
  if(!(evt_.federrs_size == 0)) return EXCLUDED;
  // Hp cut
  if(!((track_.quality & TRACK_QUALITY_HIGH_PURITY_MASK) >> TRACK_QUALITY_HIGH_PURITY_BIT)) return EXCLUDED;
  // Pt cut
  if(!(TRACK_PT_CUT_VAL < track_.pt)) return EXCLUDED;
  // Nstrip cut
  if(!(TRACK_NSTRIP_CUT_VAL < track_.strip)) return EXCLUDED;
  // D0 cut
  if(traj_.mod_on.det == 0) if(!(std::abs(track_.d0) < TRACK_D0_CUT_BARREL_VAL[traj_.mod_on.layer - 1])) return EXCLUDED;
  if(traj_.mod_on.det == 1) if(!(std::abs(track_.d0) < TRACK_D0_CUT_FORWARD_VAL)) return EXCLUDED;
  // Dz cut
  if(traj_.mod_on.det == 0) if(!(std::abs(track_.dz) < TRACK_DZ_CUT_BARREL_VAL)) return EXCLUDED;
  if(traj_.mod_on.det == 1) if(!(std::abs(track_.dz) < TRACK_DZ_CUT_FORWARD_VAL)) return EXCLUDED;
  // Pixhit cut
  if(traj_.mod_on.det == 0)
  {
    if(traj_.mod_on.layer == 1) if(!(
      (track_.validbpix[1] > 0 && track_.validbpix[2] > 0 && track_.validbpix[3] > 0) ||
      (track_.validbpix[1] > 0 && track_.validbpix[2] > 0 && track_.validfpix[0] > 0) ||
      (track_.validbpix[1] > 0 && track_.validfpix[0] > 0 && track_.validfpix[1] > 0) ||
      (track_.validfpix[0] > 0 && track_.validfpix[2] > 0 && track_.validfpix[2] > 0))) return EXCLUDED;
    if(traj_.mod_on.layer == 2) if(!(
      (track_.validbpix[0] > 0 && track_.validbpix[2] > 0 && track_.validbpix[3] > 0) ||
      (track_.validbpix[0] > 0 && track_.validbpix[2] > 0 && track_.validfpix[0] > 0) ||
      (track_.validbpix[0] > 0 && track_.validfpix[0] > 0 && track_.validfpix[1] > 0))) return EXCLUDED;
    if(traj_.mod_on.layer == 3) if(!(
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validbpix[3] > 0) ||
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validfpix[0] > 0))) return EXCLUDED;
    if(traj_.mod_on.layer == 4) if(!(
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validbpix[2] > 0))) return EXCLUDED;
  }
  if(traj_.mod_on.det == 1)
  {
    if(std::abs(traj_.mod_on.disk) == 1) if(!(
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validbpix[2] > 0) ||
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validfpix[1] > 0) ||
      (track_.validbpix[0] > 0 && track_.validfpix[1] > 0 && track_.validfpix[2] > 0))) return EXCLUDED;
    if(std::abs(traj_.mod_on.disk) == 2) if(!(
      (track_.validbpix[0] > 0 && track_.validbpix[1] > 0 && track_.validfpix[0] > 0) ||
      (track_.validbpix[0] > 0 && track_.validfpix[0] > 0 && track_.validfpix[2] > 0))) return EXCLUDED;
    if(std::abs(traj_.mod_on.disk) == 3) if(!(
      (track_.validbpix[0] > 0 && track_.validfpix[0] > 0 && track_.validfpix[1] > 0))) return EXCLUDED;
  }
  // Fidicual cuts
  if(traj_.mod_on.det == 0)
  {
    if(!(std::abs(traj_.lx) < BARREL_MODULE_EDGE_X_CUT)) return EXCLUDED;
    if(!(std::abs(traj_.lx) < BARREL_MODULE_EDGE_Y_CUT)) return EXCLUDED;
  }
  // Hitsep cut
  if(traj_.d_tr < MEAS_HITSEP_CUT_VAL) return EXCLUDED;
  // Valmis cut
  if(traj_.missing)
  {
    if((0 < traj_.d_cl) && (traj_.d_cl < HIT_CLUST_NEAR_CUT_VAL)) return VALIDHIT;
    return MISSING;
  }
  else if(traj_.validhit)
  {
    return VALIDHIT;
  }
  return EXCLUDED;
}

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE

void PhaseIPixelNtuplizer::getDisk1PropagationData(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle) {

  int hitsDisk1WhenLayer1PropagationUsed      = 0;
  int validhitsDisk1WhenLayer1PropagationUsed = 0;

  for(const auto& currentTrackKeypair: *trajTrackCollectionHandle) {

    const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
    const reco::TrackRef                    track = currentTrackKeypair.val;

    // Discarding tracks without pixel measurements
    if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;

    const auto& trajectoryMeasurements = traj -> measurements();
    auto firstLayer1TrajMeasurementIt = 
      std::find_if(trajectoryMeasurements.begin(),
		   trajectoryMeasurements.end(),
		   [&] (const TrajectoryMeasurement& measurement) {
		     ModuleData mod;
		     getModuleData(mod, 1, measurement.recHit() -> geographicalId());
		     return mod.det == 0 && mod.layer == 1;
		   });

    // Check there are hits before the first layer 1 traj. measurement
    if(firstLayer1TrajMeasurementIt == trajectoryMeasurements.begin()) continue;

    auto lastNonLayer1TrajMeasurementIt = std::prev(firstLayer1TrajMeasurementIt);

    // Check if the last non-layer1 traj measurement is valid 
    auto lastNonLayer1TrajMeasurementRecHit = lastNonLayer1TrajMeasurementIt -> recHit();
    if(lastNonLayer1TrajMeasurementRecHit == nullptr)
      std::cout << "Invalid rechit pointer." << std::endl;

    if(!(lastNonLayer1TrajMeasurementRecHit -> isValid())) continue;

    std::vector<TrajectoryMeasurement> extrapolatedHitsOnLayer1
      (getLayer1ExtrapolatedHitsFromMeas(*lastNonLayer1TrajMeasurementIt));

    if(extrapolatedHitsOnLayer1.size() == 0) continue;

    bool trackHasHitOnDisk1      = false;
    bool trackHasValidHitOnDisk1 = false;
    bool trackHasValidHitOnDisk2 = false;
    bool trackHasValidHitOnDisk3 = false;

    for(auto measurementIt = trajectoryMeasurements.begin();
	measurementIt != firstLayer1TrajMeasurementIt; measurementIt++) {

      TransientTrackingRecHit::ConstRecHitPointer recHit = measurementIt -> recHit();
      DetId detId = recHit -> geographicalId();
      ModuleData mod;
      getModuleData(mod, 0, detId);

      if(mod.det != 1) continue;

      bool isValid   = recHit -> getType() == TrackingRecHit::valid;
      bool isMissing = recHit -> getType() == TrackingRecHit::missing;
      if(mod.disk == 1 && (isValid || isMissing)) trackHasHitOnDisk1 = true;
      if(mod.disk == 1 && isValid) trackHasValidHitOnDisk1 = true;
      if(mod.disk == 2 && isValid) trackHasValidHitOnDisk2 = true;
      if(mod.disk == 3 && isValid) trackHasValidHitOnDisk3 = true;

    }

    if(trackHasHitOnDisk1 && trackHasValidHitOnDisk2 && trackHasValidHitOnDisk3) {

      hitsDisk1WhenLayer1PropagationUsed++;
      disk1PropagationEtaNumhits -> Fill(track -> eta());

      if(trackHasValidHitOnDisk1) {
	validhitsDisk1WhenLayer1PropagationUsed++;
	disk1PropagationEtaEfficiency -> Fill(track -> eta());
      }

    }

  } // end loop on trajectories

  std::cout << "Number of hits on disk 1 when layer 1 propagation was used:       "
	    << hitsDisk1WhenLayer1PropagationUsed      << std::endl;
  std::cout << "Number of valid hits on disk 1 when layer 1 propagation was used: "
	    << validhitsDisk1WhenLayer1PropagationUsed << std::endl;

}
#endif

std::vector<TEfficiency> PhaseIPixelNtuplizer::getDetectorPartEfficienciesInTrajTreeEntryRange(const TrajMeasurement& t_trajField, const Long64_t& t_minEntry, const Long64_t& t_maxEntry)
{
  std::vector<TEfficiency> detectorPartEfficiencies;
  detectorPartEfficiencies.emplace_back("ROC eff. - forward", "ROC eff. - forward",  112, -3.5, 3.5, 140, -17.5, 17.5);
  detectorPartEfficiencies.emplace_back("ROC eff. - layer 1", "ROC eff. - layer 1",   72, -4.5, 4.5,  26,  -6.5,  6.5);
  detectorPartEfficiencies.emplace_back("ROC eff. - layer 2", "ROC eff. - layer 2",   72, -4.5, 4.5,  58, -14.5, 14.5);
  detectorPartEfficiencies.emplace_back("ROC eff. - layer 3", "ROC eff. - layer 3",   72, -4.5, 4.5,  90, -22.5, 22.5);
  detectorPartEfficiencies.emplace_back("ROC eff. - layer 4", "ROC eff. - layer 4",   72, -4.5, 4.5, 130, -32.5, 32.5);
  std::vector<std::ofstream> fillPrintouts;
  // static int i = 0;
  // fillPrintouts.emplace_back("entries_fwd" + std::to_string(i) + ".txt", std::ios::out | std::ios::app);
  // fillPrintouts.emplace_back("entries_l1" + std::to_string(i) + ".txt", std::ios::out | std::ios::app);
  // fillPrintouts.emplace_back("entries_l2" + std::to_string(i) + ".txt", std::ios::out | std::ios::app);
  // fillPrintouts.emplace_back("entries_l3" + std::to_string(i) + ".txt", std::ios::out | std::ios::app);
  // fillPrintouts.emplace_back("entries_l4" + std::to_string(i) + ".txt", std::ios::out | std::ios::app);
  // i++;
  for(Long64_t entryIndex = t_minEntry; entryIndex < t_maxEntry; ++entryIndex)
  {
    trajTree_ -> GetEntry(entryIndex);
    if(t_trajField.pass_effcuts == EXCLUDED) continue;
    int efficiencyHistogramIndex = t_trajField.mod_on.det == 1 ? 0 : t_trajField.mod_on.layer;
    int validMissing = 0;
    if(t_trajField.validhit || (t_trajField.missing && 0 < t_trajField.d_cl && (t_trajField.d_cl < HIT_CLUST_NEAR_CUT_VAL))) validMissing = 1;
    if(efficiencyHistogramIndex == 0) detectorPartEfficiencies[efficiencyHistogramIndex].Fill(validMissing, t_trajField.mod_on.module_coord, t_trajField.mod_on.ladder_coord);
    else                              detectorPartEfficiencies[efficiencyHistogramIndex].Fill(validMissing, t_trajField.mod_on.disk_ring_coord, t_trajField.mod_on.blade_panel_coord);
    // (fillPrintouts[efficiencyHistogramIndex]) << 
    //   "t_trajField.mod_on.module_coord: " << std::setw(4) << std::setprecision(3) << t_trajField.mod_on.module_coord << " " <<
    //   "t_trajField.mod_on.ladder_coord: " << std::setw(4) << std::setprecision(3) << t_trajField.mod_on.ladder_coord << " " <<
    //   "validMissing: " << std::setw(4) << std::setprecision(3) << validMissing  << std::endl;
  }
  // std::for_each(fillPrintouts.begin(), fillPrintouts.end(), [] (auto& e) { e.close(); });
  return detectorPartEfficiencies;
}

void PhaseIPixelNtuplizer::generateROCEfficiencyTree()
{
  Long64_t numEntriesWithAssociatedEfficiency = trajROCEfficiencyTree_ -> GetEntries();
  Long64_t trajTreeNumEntries = trajTree_ -> GetEntries();
  TrajMeasurement trajField;
  trajTree_ -> SetBranchAddress("traj",   &trajField);
  trajTree_ -> SetBranchAddress("mod_on", &trajField.mod_on);
  std::vector<TEfficiency> detectorPartEfficiencies { getDetectorPartEfficienciesInTrajTreeEntryRange(trajField, numEntriesWithAssociatedEfficiency, trajTreeNumEntries) };
  // for(const TEfficiency& efficiency: detectorPartEfficiencies)
  // {
  //   static int i = 0;
  //   TCanvas canvas;
  //   canvas.cd();
  //   const_cast<TH1*>(efficiency.GetPassedHistogram()) -> Draw("COLZ");
  //   canvas.SaveAs((std::to_string(i++) + ".eps").c_str());
  //   const_cast<TH1*>(efficiency.GetTotalHistogram()) -> Draw("COLZ");
  //   canvas.SaveAs((std::to_string(i++) + ".eps").c_str());
  // }
  for(Long64_t entryIndex = numEntriesWithAssociatedEfficiency; entryIndex < trajTreeNumEntries; ++entryIndex)
  {
    trajTree_ -> GetEntry(entryIndex);
    std::size_t efficiencyHistogramIndex = trajField.mod_on.det == 1 ? 0 : trajField.mod_on.layer;
    
    const Int_t globalROCBin = [&] () 
    {
      if(efficiencyHistogramIndex) return detectorPartEfficiencies[efficiencyHistogramIndex].GetGlobalBin(trajField.mod_on.disk_ring_coord, trajField.mod_on.blade_panel_coord);
      return detectorPartEfficiencies[efficiencyHistogramIndex].GetGlobalBin(trajField.mod_on.module_coord, trajField.mod_on.ladder_coord);
    } ();
    const Int_t& rowLengthBins = efficiencyHistogramIndex ? 112 : 72;
    const bool isOddHalf = (globalROCBin / rowLengthBins) % 2; // Make sure this is integer division
    const Int_t rowStartBin = globalROCBin - globalROCBin % 8;
    const Int_t moduleOtherRowStartBin = isOddHalf ? rowStartBin + rowLengthBins  : rowStartBin - rowLengthBins;
    Int_t halfmoduleEfficiencyNumerator   = 0;
    Int_t halfmoduleEfficiencyDenominator = 0;
    Int_t moduleEfficiencyNumerator       = 0;
    Int_t moduleEfficiencyDenominator     = 0;
    for(Int_t i = 0; i < 8; ++i)
    {
      halfmoduleEfficiencyNumerator   += detectorPartEfficiencies[efficiencyHistogramIndex].GetPassedHistogram() -> GetBinContent(rowStartBin + i);
      halfmoduleEfficiencyDenominator += detectorPartEfficiencies[efficiencyHistogramIndex].GetTotalHistogram()  -> GetBinContent(rowStartBin + i);
      moduleEfficiencyNumerator       += detectorPartEfficiencies[efficiencyHistogramIndex].GetPassedHistogram() -> GetBinContent(rowStartBin + i);
      moduleEfficiencyDenominator     += detectorPartEfficiencies[efficiencyHistogramIndex].GetTotalHistogram()  -> GetBinContent(rowStartBin + i);
      moduleEfficiencyNumerator       += detectorPartEfficiencies[efficiencyHistogramIndex].GetPassedHistogram() -> GetBinContent(moduleOtherRowStartBin + i);
      moduleEfficiencyDenominator     += detectorPartEfficiencies[efficiencyHistogramIndex].GetTotalHistogram()  -> GetBinContent(moduleOtherRowStartBin + i);
    }
    trajROCEff_.ROCEfficiency        = detectorPartEfficiencies[efficiencyHistogramIndex].GetEfficiency(globalROCBin);
    if(0 < halfmoduleEfficiencyDenominator) trajROCEff_.halfModuleEfficiency = static_cast<float>(halfmoduleEfficiencyNumerator) / static_cast<float>(halfmoduleEfficiencyDenominator);
    else                                    trajROCEff_.halfModuleEfficiency = 0.0f;
    if(0 < moduleEfficiencyDenominator)     trajROCEff_.moduleEfficiency     = static_cast<float>(moduleEfficiencyNumerator)     / static_cast<float>(moduleEfficiencyDenominator);
    else                                    trajROCEff_.moduleEfficiency     = 0.0f;

    // std::cout << "***" << std::endl;
    // std::cout << 
    //   "trajField.mod_on.det:             " << std::setw(5) << std::setprecision(3) << trajField.mod_on.det             << std::endl << 
    //   "trajField.mod_on.layer:           " << std::setw(5) << std::setprecision(3) << trajField.mod_on.layer           << std::endl << 
    //   "trajField.mod_on.module_coord:    " << std::setw(5) << std::setprecision(3) << trajField.mod_on.module_coord    << std::endl << 
    //   "trajField.mod_on.ladder_coord:    " << std::setw(5) << std::setprecision(3) << trajField.mod_on.ladder_coord    << std::endl << 
    //   "trajROCEff_.ROCEfficiency:        " << std::setw(5) << std::setprecision(3) << trajROCEff_.ROCEfficiency        << std::endl << 
    //   "trajROCEff_.halfModuleEfficiency: " << std::setw(5) << std::setprecision(3) << trajROCEff_.halfModuleEfficiency << std::endl << 
    //   "trajROCEff_.moduleEfficiency:     " << std::setw(5) << std::setprecision(3) << trajROCEff_.moduleEfficiency     << std::endl << 
    //   "halfmoduleEfficiencyNumerator:    " << std::setw(5) << std::setprecision(3) << halfmoduleEfficiencyNumerator    << std::endl << 
    //   "halfmoduleEfficiencyDenominator:  " << std::setw(5) << std::setprecision(3) << halfmoduleEfficiencyDenominator  << std::endl << 
    //   "moduleEfficiencyNumerator:        " << std::setw(5) << std::setprecision(3) << moduleEfficiencyNumerator        << std::endl << 
    //   "moduleEfficiencyDenominator:      " << std::setw(5) << std::setprecision(3) << moduleEfficiencyDenominator      << std::endl << 
    //   "moduleEfficiencyNumerator:        " << std::setw(5) << std::setprecision(3) << moduleEfficiencyNumerator        << std::endl << 
    //   "moduleEfficiencyDenominator:      " << std::setw(5) << std::setprecision(3) << moduleEfficiencyDenominator      << std::endl;
    trajROCEfficiencyTree_ -> Fill();
  }
  std::for_each(detectorPartEfficiencies.begin(), detectorPartEfficiencies.end(), [] (auto& e)
  {
    e.Delete();
  });
  detectorPartEfficiencies.clear();
}

//////////////////////////////
// Private member functions //
//////////////////////////////

////////////////////
// Error handling //
////////////////////

// BLUE:    "\03334[m"
// RED:     "\03331[m"
// DEFAULT: "\03339[m"

void PhaseIPixelNtuplizer::handleDefaultError
(const std::string& exception_type, const std::string& streamType, std::string msg)
{
  edm::LogError(streamType.c_str()) << "\03331[m" << msg << "\03339[m" << std::endl;
  printEvtInfo(streamType);
  throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::handleDefaultError
(const std::string& exception_type, const std::string& streamType, std::vector<std::string> msg)
{
  edm::LogError(streamType.c_str()) << "\03331[m";
  for(const auto& msg_part: msg)
    edm::LogError(streamType.c_str()) << msg_part;
  edm::LogError(streamType.c_str()) << "\03339[m" << std::endl;
  printEvtInfo(streamType);
  throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::printEvtInfo(const std::string& streamType)
{
  edm::LogError(streamType.c_str())
    << "\03334[m"
    << "Run: " << evt_.run << " Ls: " << evt_.ls << " Evt:" << evt_.evt
    << "\03339[m" << std::endl;
}

void PhaseIPixelNtuplizer::getModuleData(ModuleData &mod, bool online, const DetId &detId)
{

  mod.init();

  mod.det  = detId.subdetId() - 1;
  mod.shl  = coord_.quadrant(detId);
  mod.side = coord_.side(detId);

  if(detId.subdetId() == PixelSubdetector::PixelBarrel) {

    mod.sec     = coord_.sector(detId);
    mod.half    = coord_.half(detId);
    mod.layer   = coord_.layer(detId);
    mod.flipped = coord_.flipped(detId); // opposite of outer
    if(online) {
      mod.ladder = coord_.signed_ladder(detId);
      mod.module = coord_.signed_module(detId);
    } else {
      mod.ladder = coord_.ladder(detId);
      mod.module = coord_.module(detId);
    }

  } else if(detId.subdetId() == PixelSubdetector::PixelEndcap) {

    mod.ring   = coord_.ring(detId);
    mod.panel  = coord_.panel(detId);
    mod.module = coord_.module(detId);
    if(online) {
      mod.disk  = coord_.signed_disk(detId);
      mod.blade = coord_.signed_blade(detId);
    } else {
      mod.disk  = coord_.disk(detId);
      mod.blade = coord_.blade(detId);
    }

  }

  mod.rawid = detId.rawId();
  mod.fedid = coord_.fedid(detId);

  // FED error
  std::map<uint32_t, int>::const_iterator federrors_it =
    federrors_.find(detId.rawId());
  mod.federr = (federrors_it != federrors_.end()) ? federrors_it->second : 0;

}

void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const DetId &detId, const PixelDigi *digi)
{

  mod.channel = coord_.channel(detId, digi);
  mod.roc     = coord_.roc(detId, digi);

  if(detId.subdetId() == PixelSubdetector::PixelBarrel) {

    if(online) {
      mod.module_coord = coord_.signed_module_coord(detId, digi);
      mod.ladder_coord = coord_.signed_ladder_coord(detId, digi);
    } else {
      mod.module_coord = coord_.module_coord(detId, digi);
      mod.ladder_coord = coord_.ladder_coord(detId, digi);
    }

  } else if(detId.subdetId() == PixelSubdetector::PixelEndcap) {

    if(online) {
      mod.disk_coord        = coord_.signed_disk_coord(detId, digi);
      mod.disk_ring_coord   = coord_.signed_disk_ring_coord(detId, digi);
      mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(detId, digi);
    } else {
      mod.disk_coord        = coord_.disk_coord(detId, digi);
      mod.disk_ring_coord   = coord_.disk_ring_coord(detId, digi);
      mod.blade_panel_coord = coord_.blade_panel_coord(detId, digi);
    }

  }

}

void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const DetId &detId, const SiPixelCluster *cluster)
{

  mod.channel = coord_.channel(detId, cluster);
  mod.roc     = coord_.roc(detId, cluster);

  if(detId.subdetId() == PixelSubdetector::PixelBarrel) {

    if(online) {
      mod.module_coord = coord_.signed_module_coord(detId, cluster);
      mod.ladder_coord = coord_.signed_ladder_coord(detId, cluster);
    } else {
      mod.module_coord = coord_.module_coord(detId, cluster);
      mod.ladder_coord = coord_.ladder_coord(detId, cluster);
    }

  } else if(detId.subdetId() == PixelSubdetector::PixelEndcap) {

    if(online) {
      mod.disk_coord        = coord_.signed_disk_coord(detId, cluster);
      mod.disk_ring_coord   = coord_.signed_disk_ring_coord(detId, cluster);
      mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(detId, cluster);
    } else {
      mod.disk_coord        = coord_.disk_coord(detId, cluster);
      mod.disk_ring_coord   = coord_.disk_ring_coord(detId, cluster);
      mod.blade_panel_coord = coord_.blade_panel_coord(detId, cluster);
    }

  }

}

void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const SiPixelRecHit *rechit)
{

  mod.channel = coord_.channel(rechit);
  mod.roc     = coord_.roc(rechit);

  if(rechit->geographicalId().subdetId() == PixelSubdetector::PixelBarrel) {

    if(online) {
      mod.module_coord = coord_.signed_module_coord(rechit);
      mod.ladder_coord = coord_.signed_ladder_coord(rechit);
    } else {
      mod.module_coord = coord_.module_coord(rechit);
      mod.ladder_coord = coord_.ladder_coord(rechit);
    }

  } else if(rechit->geographicalId().subdetId() == PixelSubdetector::PixelEndcap) {

    if(online) {
      mod.disk_coord        = coord_.signed_disk_coord(rechit);
      mod.disk_ring_coord   = coord_.signed_disk_ring_coord(rechit);
      mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(rechit);
    } else {
      mod.disk_coord        = coord_.disk_coord(rechit);
      mod.disk_ring_coord   = coord_.disk_ring_coord(rechit);
      mod.blade_panel_coord = coord_.blade_panel_coord(rechit);
    }

  }

}

float PhaseIPixelNtuplizer::trajMeasGlobalPointDistance
(const TrajectoryMeasurement& trajMeasurement, const GlobalPoint& referencePoint)
{

  const GlobalPoint measurementPosition = trajMeasurement.updatedState().globalPosition();
  return std::sqrt((referencePoint - measurementPosition).mag2());

}

const SiPixelCluster* PhaseIPixelNtuplizer::getClosestClusterOnDetSetToPoint
(const edmNew::DetSet<SiPixelCluster>& clustersOnDet, const LocalPoint& referencePoint)
{

  if(clustersOnDet.empty()) return nullptr;

  const DetId detId = clustersOnDet.id();
  const SiPixelCluster* minDistanceCluster = clustersOnDet.begin();
  float currentMinValue = 
    clusterPointDistance(detId, *minDistanceCluster, referencePoint);

  for(const auto& cluster: clustersOnDet) {

    float currentDistance = clusterPointDistance(detId, cluster, referencePoint);
    if(currentDistance < currentMinValue) {
      currentMinValue = currentDistance;
      minDistanceCluster = &cluster;
    }

  }

  return minDistanceCluster;

}

float PhaseIPixelNtuplizer::clusterPointDistance
(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint)
{

  const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);
  LocalPoint clustLocalCoordinates;
  std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(cluster, *geomDetUnit);

  float xDist = clustLocalCoordinates.x() - referencePoint.x();
  float yDist = clustLocalCoordinates.y() - referencePoint.y();
  float zDist = clustLocalCoordinates.z() - referencePoint.z();

  return sqrt(xDist * xDist + yDist * yDist + zDist * zDist);

}

LocalPoint PhaseIPixelNtuplizer::clusterPointDistanceVector
(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint)
{

  const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);
  LocalPoint clustLocalCoordinates;
  std::tie(clustLocalCoordinates, std::ignore, std::ignore) = 
    pixelClusterParameterEstimator_ -> getParameters(cluster, *geomDetUnit);

  return LocalPoint(clustLocalCoordinates.x() - referencePoint.x(),
		    clustLocalCoordinates.y() - referencePoint.y(),
		    clustLocalCoordinates.z() - referencePoint.z());

}

void PhaseIPixelNtuplizer::printTrackCompositionInfo
( const edm::Ref<std::vector<Trajectory>>& trajectory,
  const reco::TrackRef& track,
  const edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle,
  const edm::Handle<reco::VertexCollection>& vertexCollectionHandle)
{

  const auto& trajMeasurements = trajectory -> measurements();
  size_t trajMeasCount   = trajMeasurements.size();

  size_t numValidHits    = 
    std::count_if(trajMeasurements.begin(),
		  trajMeasurements.end(),
		  [] (const auto& meas) { 
		    return meas.recHit() -> getType() == TrackingRecHit::valid; 
		  });

  size_t numMissingHits  = 
    std::count_if(trajMeasurements.begin(),
		  trajMeasurements.end(),
		  [] (const auto& meas) {
		    return meas.recHit() -> getType() == TrackingRecHit::missing; 
		  });

  float  trackPt = track -> pt();

  reco::VertexCollection::const_iterator closestVtx = 
    NtuplizerHelpers::findClosestVertexToTrack(track, vertexCollectionHandle, 10);

  float  trackD0 = NOVAL_F;
  float  trackDz = NOVAL_F;

  if(closestVtx != vertexCollectionHandle -> end()) {
    trackD0 = track -> dxy(closestVtx -> position()) * -1.0;
    trackDz = track -> dz (closestVtx -> position());	
  }

  std::cout << " --- Begin track informations --- " << std::endl;
  std::cout << "Informations on a track consisting of " << trajMeasCount
	    << " trajectory measurements: " << std::endl;
  std::cout << "Number of valid   hits: " << numValidHits   << std::endl;
  std::cout << "Number of missing hits: " << numMissingHits << std::endl;
  std::cout << "Track d0: " << trackD0 << std::endl;
  std::cout << "Track dz: " << trackDz << std::endl;
  std::cout << "Track pt: " << trackPt << std::endl;
  std::cout << "Trajectory measurement position list: " << std::endl;

  for(const auto& trajMeasurement: trajMeasurements) {

    ModuleData mod;

    TransientTrackingRecHit::ConstRecHitPointer hit = trajMeasurement.recHit();
    DetId trajMeasDetId(hit -> geographicalId().rawId());
    uint32_t subDetId              = trajMeasDetId.subdetId();

    const int& validhit            = hit -> getType() == TrackingRecHit::valid;
    const int& missing             = hit -> getType() == TrackingRecHit::missing;

    const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(trajMeasDetId);

    getModuleData(mod, 0, trajMeasDetId);
    TrajectoryStateOnSurface trajStateOnSurface =
      NtuplizerHelpers::getTrajectoryStateOnSurface(trajMeasurement);

    if(!(trajStateOnSurface.isValid())) return;

    GlobalPoint globalPosition     = trajStateOnSurface.globalPosition();
    LocalPoint  localPosition      = trajStateOnSurface.localPosition();
    LocalError  localPositionError = trajStateOnSurface.localError().positionError();

    const SiPixelCluster* closestCluster = nullptr;

    for(const auto& currentClusterSet: *clusterCollectionHandle) {

      DetId clusterDetSetDetId(currentClusterSet.id());

      if(clusterDetSetDetId!= trajMeasDetId) continue;

      closestCluster = getClosestClusterOnDetSetToPoint(currentClusterSet, localPosition);
      break;

    }

    std::string detectorPartName = "unknown detector part";
    if      (subDetId == PixelSubdetector::PixelBarrel) detectorPartName = "pixel barrel";
    else if (subDetId == PixelSubdetector::PixelEndcap) detectorPartName = "pixel forward";
    else if (subDetId == StripSubdetector::TIB) detectorPartName = "strip TIB";
    else if (subDetId == StripSubdetector::TOB) detectorPartName = "strip TOB";
    else if (subDetId == StripSubdetector::TID) detectorPartName = "strip TID";
    else if (subDetId == StripSubdetector::TEC) detectorPartName = "strip TEC";

    std::cout << "Trajectory measurement on " << detectorPartName << "." << std::endl;
    std::cout << "Validhit: " << validhit << " Missing:" << missing << std::endl;
    if(mod.det == 0) {
      std::cout << "Layer: " << mod.layer << ", ladder: " << mod.ladder << ", module: " << mod.module << std::endl;
    } else if(mod.det == 1) {
      std::cout << "Disk: " << mod.disk << ", ring number: " << mod.ring << ", blade: " << mod.blade << ", panel: " << mod.panel << std::endl;
    }
    std::cout << "Global position of the hit  ("  
	      << std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.x() << ", " 
	      << std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.y() << ", "
	      << std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.z() 
	      << ")." << std::endl;
    std::cout << "Local position of the hit   ("
	      << std::setw(8) << std::fixed << std::setprecision(2) << localPosition.x() << ",  "
	      << std::setw(8) << std::fixed << std::setprecision(2) << localPosition.y() << ", "
	      << std::setw(8) << std::fixed << std::setprecision(2) << localPosition.z() 
	      << ")." << std::endl;
    std::cout << "Local pos. error of the hit (" 
	      << std::setw(8) << std::fixed << std::setprecision(5) <<localPositionError.xx()<<", "
	      << std::setw(8) << std::fixed << std::setprecision(5) <<localPositionError.xy()<<", "
	      << std::setw(8) << std::fixed << std::setprecision(5) <<localPositionError.yy()
	      << ")." << std::endl;

    if(closestCluster) {

      LocalPoint clustLocalCoordinates;
      std::tie(clustLocalCoordinates, std::ignore, std::ignore) = 
	pixelClusterParameterEstimator_ -> getParameters(*closestCluster, *geomDetUnit);

      std::cout << "Local position of the closest cluster to predicted hit" \
	" on the same module: " << std::endl;
      std::cout << "("  << clustLocalCoordinates.x() << ",  "
		<< clustLocalCoordinates.y() << ",  " 
		<< clustLocalCoordinates.z() << ")." << std::endl;
      std::cout << "Distance from the predicted hit: " 
		<< clusterPointDistanceVector(trajMeasDetId, *closestCluster, localPosition)
		<< std::endl;
      std::cout << "Abs distance: " 
		<< clusterPointDistance(trajMeasDetId, *closestCluster, localPosition)
		<< std::endl;

    }

  }

  std::cout << " --- End track informations --- " << std::endl;

}

void PhaseIPixelNtuplizer::findClosestClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster> >& clusterCollectionHandle,
					       uint32_t rawId, float lx, float ly, float* dx_cl, float* dy_cl) {
  
  for (size_t i=0; i<2; i++) dx_cl[i]=dy_cl[i]=NOVAL_F;

  if (!clusterCollectionHandle.isValid()) return;

  const edmNew::DetSetVector<SiPixelCluster>& clusterCollection=*clusterCollectionHandle;
  edmNew::DetSetVector<SiPixelCluster>::const_iterator itClusterSet= clusterCollection.begin();

  float minD[2];
  minD[0]=minD[1]=10000.;

  for ( ; itClusterSet!=clusterCollection.end(); itClusterSet++) {
    
    DetId detId(itClusterSet->id());
    
    if (detId.rawId()!=rawId) continue;

    unsigned int subDetId=detId.subdetId();
    if (subDetId!=PixelSubdetector::PixelBarrel &&
	subDetId!=PixelSubdetector::PixelEndcap) {
      std::cout << "ERROR: not a pixel cluster!!!" << std::endl; // should not happen
      continue;
    }

    const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) trackerGeometry_->idToDetUnit(detId);

    edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=itClusterSet->begin();  
    for(; itCluster!=itClusterSet->end(); ++itCluster) {
      //const Surface* surface;
      //surface = &(tracker->idToDet(detId)->surface());
      //GlobalPoint gp;
      LocalPoint lp(itCluster->x(), itCluster->y(), 0.);
      PixelClusterParameterEstimator::ReturnType params=pixelClusterParameterEstimator_->getParameters(*itCluster,*pixdet);
      lp = std::get<0>(params);
      //else {
      //  LocalPoint lp(itCluster->x(), itCluster->y(), 0.);
      //  gp = surface->toGlobal(lp);
      //}
      //floatD=sqrt((gp.x()-glx)*(gp.x()-glx)+(gp.y()-gly)*(gp.y()-gly)+(gp.z()-glz)*(gp.z()-glz));
      float D = sqrt((lp.x()-lx)*(lp.x()-lx)+(lp.y()-ly)*(lp.y()-ly));
      if (D<minD[0]) {
	minD[1]=minD[0];
	dx_cl[1]=dx_cl[0];
	dy_cl[1]=dy_cl[0];
	minD[0]=D;
	dx_cl[0]=lp.x();
	dy_cl[0]=lp.y();
      } else if (D<minD[1]) {
	minD[1]=D;
	dx_cl[1]=lp.x();
	dy_cl[1]=lp.y();
      }
    } // loop on cluster sets
  }

  for (size_t i=0; i<2; i++) {
    if (minD[i]<9999.) {
      dx_cl[i]=fabs(dx_cl[i]-lx);
      dy_cl[i]=fabs(dy_cl[i]-ly);
    }
  }

}


namespace NtuplizerHelpers
{

  std::map<uint32_t, int> getFedErrors
  ( const edm::Event& iEvent, 
    const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& rawDataErrorToken)
  {

    std::map<uint32_t, int> federrors;

    edm::Handle<edm::DetSetVector<SiPixelRawDataError>> siPixelRawDataErrorCollectionHandle;
    iEvent.getByToken(rawDataErrorToken,                siPixelRawDataErrorCollectionHandle);

    // Return empty map if no fed error entry is available
    if(!siPixelRawDataErrorCollectionHandle.isValid()) return federrors;

    // Loop on errors
    for(const auto& pixel_error_set: *siPixelRawDataErrorCollectionHandle) {
      for(const auto& pixel_error: pixel_error_set) {
	if(pixel_error_set.detId() != 0xffffffff) {
	  DetId detId(pixel_error_set.detId());
	  int type = pixel_error.getType();
	  federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
	}
      }
    }

    return federrors;

  }

  bool detidIsOnPixel(const DetId& detid) {
    if (detid.det()!=DetId::Tracker) return false;
    if (detid.subdetId() == PixelSubdetector::PixelBarrel) return true;
    if (detid.subdetId() == PixelSubdetector::PixelEndcap) return true;
    return false;
  }

  bool areIdenticalModules(const ModuleData& lhs, const ModuleData& rhs) {
    if(lhs.det != rhs.det) return false;
    if(rhs.det == 0) {
      if( lhs.layer  != rhs.layer  ||
	  lhs.ladder != rhs.ladder || 
	  lhs.module != rhs.module) return false;
      return true;
    }
    if(rhs.det == 1) {
      if( lhs.disk  != rhs.disk  ||
	  lhs.ring != rhs.ring || 
	  lhs.blade != rhs.blade || 
	  lhs.panel != rhs.panel) return false;	
      return true;
    }
    std::cout << "Error in " << __PRETTY_FUNCTION__
	      << ": unidentified module comparison." << std::endl;
    return false;
  }

  int trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>& trajectory) {

    // Looping on the full track to check if we have pixel hits 
    // and to count the number of strip hits 

    for(auto& measurement: trajectory -> measurements()) {

      // Check measurement validity
      if(!measurement.updatedState().isValid()) continue;
      auto hit = measurement.recHit();

      // Check hit quality
      if(!hit -> isValid()) continue;

      DetId det_id = hit -> geographicalId();
      uint32_t subdetid = (det_id.subdetId());

      // For saving the pixel hits
      if(subdetid == PixelSubdetector::PixelBarrel) return 1;
      if(subdetid == PixelSubdetector::PixelEndcap) return 1;

    }

    return 0;

  }

  reco::VertexCollection::const_iterator findClosestVertexToTrack
  ( const reco::TrackRef& track,
    const edm::Handle<reco::VertexCollection>& vertexCollectionHandle,
    const unsigned int& minTracks) {

    auto isVertexGood        = 
      [&] (const auto& vertex) { 
      return vertex.isValid() && minTracks <= vertex.tracksSize();
    };

    auto trackVertexDistance = 
      [&] (const reco::VertexCollection::const_iterator& vertexIt) {
      double trkVtxD0 = track -> dxy(vertexIt -> position()) * -1.0;
      double trkVtxDz = track -> dz (vertexIt -> position());
      return sqrt(trkVtxD0 * trkVtxD0 + trkVtxDz * trkVtxDz);
    };

    reco::VertexCollection::const_iterator closestVtx = 
      std::find_if( vertexCollectionHandle -> begin(),
		    vertexCollectionHandle -> end(),
		    [&] (const auto& vertex) { 
		      return isVertexGood(vertex); 
		    });

    if(closestVtx == vertexCollectionHandle -> end()) return closestVtx;

    double minDistance = trackVertexDistance(closestVtx);
    for(reco::VertexCollection::const_iterator it = closestVtx;
	it != vertexCollectionHandle -> end(); ++it) {

      // Filter out invalid vertices
      if(!isVertexGood(*it)) continue;

      // Comparing squareroots should be quick enough
      // if required, change this to a comparison of squares
      double trkVtxDist = trackVertexDistance(it);
      if(trkVtxDist < minDistance) {
	minDistance = std::move(trkVtxDist);
	closestVtx  = it;
      }

    }

    return closestVtx;

  }

  TrajectoryStateOnSurface getTrajectoryStateOnSurface(const TrajectoryMeasurement& measurement) {

    static TrajectoryStateCombiner trajStateCombiner;

    const auto& forwardPredictedState  = measurement.forwardPredictedState();
    const auto& backwardPredictedState = measurement.backwardPredictedState();

    if(forwardPredictedState.isValid() && backwardPredictedState.isValid())
      return trajStateCombiner(forwardPredictedState, backwardPredictedState);

    else if(backwardPredictedState.isValid())
      return backwardPredictedState;

    else if(forwardPredictedState.isValid())
      return forwardPredictedState;

    std::cout << "Error saving traj. measurement data." \
      " Trajectory state on surface cannot be determined." << std::endl;

    return TrajectoryStateOnSurface();

  }

  std::pair<float, float> getLocalXY(const TrajectoryMeasurement& measurement) {

    std::pair<float, float> returnValue;
    TrajectoryStateOnSurface trajStateOnSurface = getTrajectoryStateOnSurface(measurement);

    if(!(trajStateOnSurface.isValid())) return std::make_pair<float, float>(NOVAL_F, NOVAL_F);

    LocalPoint localPosition = trajStateOnSurface.localPosition();
    returnValue.first  = localPosition.x();
    returnValue.second = localPosition.y();

    return returnValue;

  }

  float trajMeasurementDistance
  (const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs) {

    std::pair<float, float> lhsLocalXY = getLocalXY(lhs);
    std::pair<float, float> rhsLocalXY = getLocalXY(rhs);

    float dxHit = lhsLocalXY.first  - rhsLocalXY.first;
    float dyHit = lhsLocalXY.second - rhsLocalXY.second;

    return std::sqrt(dxHit * dxHit + dyHit * dyHit);
  }

  void trajMeasurementDistance
  ( const TrajectoryMeasurement& lhs,
    const TrajectoryMeasurement& rhs,
    float& distance, float& dx, float& dy) {

    std::pair<float, float> lhsLocalXY = getLocalXY(lhs);
    std::pair<float, float> rhsLocalXY = getLocalXY(rhs);

    dx = lhsLocalXY.first  - rhsLocalXY.first;
    dy = lhsLocalXY.second - rhsLocalXY.second;
    distance = std::sqrt(dx*dx + dy*dy);
  }

  bool sameTrack(const reco::TrackRef& one, const reco::TrackRef& two) {
    return (std::abs(one->px() - two->px()) < 1e-2 &&
            std::abs(one->py() - two->py()) < 1e-2 &&
            std::abs(one->pz() - two->pz()) < 1e-2 &&
            std::abs(one->vx() - two->vx()) < 1e-2 &&
            std::abs(one->vy() - two->vy()) < 1e-2 &&
          std::abs(one->vz() - two->vz()) < 1e-2);
  }

  void getClosestTrajMeasDistance(const TrajectoryMeasurement& measurement,
                              const reco::TrackRef& track,
                              const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle,
                              float& distance, float& dx, float& dy) {

    dx = -NOVAL_F;
    dy = -NOVAL_F;
    distance = -NOVAL_F;

    DetId detId = measurement.recHit() -> geographicalId();
    double closestDistance = 9999;
    for(const auto& otherTrajTrackPair: *trajTrackCollectionHandle) {

      const reco::TrackRef& otherTrack = otherTrajTrackPair.val;
      if (sameTrack(track, otherTrack)) continue;

      const edm::Ref<std::vector<Trajectory>> otherTraj = otherTrajTrackPair.key;

      for(auto otherTrajMeasurementIt = otherTraj -> measurements().begin();
	  otherTrajMeasurementIt != otherTraj -> measurements().end(); ++otherTrajMeasurementIt) {

	if(otherTrajMeasurementIt -> recHit() -> geographicalId() != detId) continue;

	float otherDistance =  trajMeasurementDistance(measurement, *otherTrajMeasurementIt);
	if (otherDistance < closestDistance) {
          closestDistance = otherDistance;
          trajMeasurementDistance(measurement, *otherTrajMeasurementIt, distance, dx, dy);
        }
      }
    }
  }
  
} // NtuplizerHelpers


DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);
