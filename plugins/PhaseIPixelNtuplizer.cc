#include "PhaseIPixelNtuplizer.h"

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig) :
  ntupleOutputFilename_(iConfig.getUntrackedParameter<std::string>("outputFileName", "Ntuple.root"))
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
	,
	simhitOccupancy_fwd       (new TH2D("simhitOccupancy_fwd",        "simhit occupancy - forward",          112, -52.15, 52.15,  50,   0.0, 3.14159)),
	simhitOccupancy_l1        (new TH2D("simhitOccupancy_l1",         "simhit occupancy - layer 1",          72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	simhitOccupancy_l2        (new TH2D("simhitOccupancy_l2",         "simhit occupancy - layer 2",          72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	simhitOccupancy_l3        (new TH2D("simhitOccupancy_l3",         "simhit occupancy - layer 3",          72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	simhitOccupancy_l4        (new TH2D("simhitOccupancy_l4",         "simhit occupancy - layer 4",          72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	digiOccupancy_fwd         (new TH2D("digiOccupancy_fwd",          "digi occupancy - forward",            112, -52.15, 52.15,  50,   0.0, 3.14159)),
	digiOccupancy_l1          (new TH2D("digiOccupancy_l1",           "digi occupancy - layer 1",            72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	digiOccupancy_l2          (new TH2D("digiOccupancy_l2",           "digi occupancy - layer 2",            72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	digiOccupancy_l3          (new TH2D("digiOccupancy_l3",           "digi occupancy - layer 3",            72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	digiOccupancy_l4          (new TH2D("digiOccupancy_l4",           "digi occupancy - layer 4",            72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	rechitOccupancy_fwd       (new TH2D("rechitOccupancy_fwd",        "cluster occupancy - forward",         112, -52.15, 52.15,  50,   0.0, 3.14159)),
	rechitOccupancy_l1        (new TH2D("rechitOccupancy_l1",         "cluster occupancy - layer 1",         72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	rechitOccupancy_l2        (new TH2D("rechitOccupancy_l2",         "cluster occupancy - layer 2",         72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	rechitOccupancy_l3        (new TH2D("rechitOccupancy_l3",         "cluster occupancy - layer 3",         72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	rechitOccupancy_l4        (new TH2D("rechitOccupancy_l4",         "cluster occupancy - layer 4",         72,  -26.7,  26.7,   50,   0.0, 3.14159)),
	clustOccupancyROCBins_fwd (new TH2D("clustOccupancyROCBins_fwd",  "cluster occupancy on ROCs - forward", 112,  -3.5,   3.5,  140, -17.5, 17.5   )),
	clustOccupancyROCBins_l1  (new TH2D("clustOccupancyROCBins_l1",   "cluster occupancy on ROCs - layer 1", 72,   -4.5,   4.5,   26,  -6.5,  6.5   )),
	clustOccupancyROCBins_l2  (new TH2D("clustOccupancyROCBins_l2",   "cluster occupancy on ROCs - layer 2", 72,   -4.5,   4.5,   58, -14.5, 14.5   )),
	clustOccupancyROCBins_l3  (new TH2D("clustOccupancyROCBins_l3",   "cluster occupancy on ROCs - layer 3", 72,   -4.5,   4.5,   90, -22.5, 22.5   )),
	clustOccupancyROCBins_l4  (new TH2D("clustOccupancyROCBins_l4",   "cluster occupancy on ROCs - layer 4", 72,   -4.5,   4.5,  130, -32.5, 32.5   )),
	rechitOccupancyROCBins_fwd(new TH2D("rechitOccupancyROCBins_fwd", "rechit occupancy on ROCS - forward",  112,  -3.5,   3.5,  140, -17.5, 17.5   )),
	rechitOccupancyROCBins_l1 (new TH2D("rechitOccupancyROCBins_l1",  "rechit occupancy on ROCS - layer 1",  72,   -4.5,   4.5,   26,  -6.5,  6.5   )),
	rechitOccupancyROCBins_l2 (new TH2D("rechitOccupancyROCBins_l2",  "rechit occupancy on ROCS - layer 2",  72,   -4.5,   4.5,   58, -14.5, 14.5   )),
	rechitOccupancyROCBins_l3 (new TH2D("rechitOccupancyROCBins_l3",  "rechit occupancy on ROCS - layer 3",  72,   -4.5,   4.5,   90, -22.5, 22.5   )),
	rechitOccupancyROCBins_l4 (new TH2D("rechitOccupancyROCBins_l4",  "rechit occupancy on ROCS - layer 4",  72,   -4.5,   4.5,  130, -32.5, 32.5   ))
#endif
{
	iConfig_                = iConfig;
	clusterSaveDownscaling_ = 1;
	if(iConfig_.exists("triggerTag"))
	{
		triggerTag_ = iConfig_.getParameter<edm::InputTag>("triggerTag");
		std::cout << "NON-DEFAULT PARAMETER: triggerTag = " << triggerTag_ << std::endl;
	}
	else { triggerTag_ = edm::InputTag("TriggerResults", "", "HLT"); }
	if(iConfig_.exists("triggerNames"))
	{
		triggerNames_ = iConfig_.getParameter<std::vector<std::string>>("triggerNames");
		std::cout << "NON-DEFAULT PARAMETER: triggerNames= ";
		for(size_t i = 0; i < triggerNames_.size(); i++) std::cout << triggerNames_[i] << " ";
		std::cout << std::endl;
	}
	else
	{
		triggerNames_.clear();
		triggerNames_.push_back("HLT_ZeroBias");
		triggerNames_.push_back("HLT_Random");
	}
	// Tokens
	rawDataErrorToken_        = consumes<edm::DetSetVector<SiPixelRawDataError>>(edm::InputTag("siPixelDigis"));
	primaryVerticesToken_     = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
	triggerResultsToken_      = consumes<edm::TriggerResults>(triggerTag_);
	clustersToken_            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
	trajTrackCollectionToken_ = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
	pixelDigiCollectionToken_ = consumes<edm::DetSetVector<PixelDigi>>(edm::InputTag("simSiPixelDigis"));
	simhitCollectionTokens_.push_back(consumes<std::vector<PSimHit>>(edm::InputTag("g4SimHits", "TrackerHitsPixelBarrelHighTof")));
	simhitCollectionTokens_.push_back(consumes<std::vector<PSimHit>>(edm::InputTag("g4SimHits", "TrackerHitsPixelBarrelLowTof")));
	simhitCollectionTokens_.push_back(consumes<std::vector<PSimHit>>(edm::InputTag("g4SimHits", "TrackerHitsPixelEndcapHighTof")));
	simhitCollectionTokens_.push_back(consumes<std::vector<PSimHit>>(edm::InputTag("g4SimHits", "TrackerHitsPixelEndcapLowTof")));
#endif	
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer() {}

void PhaseIPixelNtuplizer::beginJob()
{
	if(iConfig_.exists("fileName")) ntupleOutputFilename_ = iConfig_.getParameter<std::string>("filename");
	// Create output file
	ntupleOutputFile_ = new TFile(ntupleOutputFilename_.c_str(), "RECREATE");
	if(!(ntupleOutputFile_ -> IsOpen()))
	{
		handleDefaultError("file_operations", "file_operations", { "Failed to open output file: ", ntupleOutputFilename_ });
	}
	LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename_ << "\" created." << std::endl;
	// Tree definitions
	eventTree_ = new TTree("eventTree", "The event.");
	clustTree_ = new TTree("clustTree", "Pixel clusters.");
	trackTree_ = new TTree("trackTree", "The track in the event.");
	trajTree_  = new TTree("trajTree",   "Trajectory measurements in the Pixel detector.");
	// Event tree
	eventTree_ -> Branch("event",     &evt_,         evt_        .list.c_str());
	// Cluster tree
	clustTree_ -> Branch("event",     &evt_,         evt_        .list.c_str());
	clustTree_ -> Branch("mod_on",    &clu_.mod_on,  clu_.mod_on .list.c_str());
	clustTree_ -> Branch("mod",       &clu_.mod,     clu_.mod    .list.c_str());
	clustTree_ -> Branch("clust",     &clu_,         clu_        .list.c_str());
	clustTree_ -> Branch("clust_adc", &clu_.adc,     "adc[size]/F");
	clustTree_ -> Branch("clust_pix", &clu_.pix,     "pix[size][2]/F");
	// Track treex
	trackTree_ -> Branch("event",     &evt_,         evt_        .list.c_str());
	trackTree_ -> Branch("track",     &track_,       track_      .list.c_str());
	// Trajectory tree
	trajTree_  -> Branch("event",     &evt_,          evt_        .list.c_str());
	trajTree_  -> Branch("mod_on",    &traj_.mod_on,  traj_.mod_on.list.c_str());
	trajTree_  -> Branch("mod",       &traj_.mod,     traj_.mod   .list.c_str());
	trajTree_  -> Branch("clust",     &traj_.clu,     traj_.clu   .list.c_str());
	trajTree_  -> Branch("clust_adc", &traj_.clu.adc, "adc[size]/F");
	trajTree_  -> Branch("clust_pix", &traj_.clu.pix, "pix[size][2]/F");
	trajTree_  -> Branch("track",     &track_,        track_      .list.c_str());
	trajTree_  -> Branch("traj",      &traj_,         traj_       .list.c_str());
}

void PhaseIPixelNtuplizer::endJob() 
{
	LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename_ << "\"." << std::endl;
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
	constexpr int PHASE_SCENARIO = 1;
	gStyle -> SetPalette(1);
	gStyle -> SetNumberContours(999);
	const std::vector<TH2D*> histogramsToSave = 
	{
		simhitOccupancy_fwd,        simhitOccupancy_l1,        simhitOccupancy_l2,        simhitOccupancy_l3,        simhitOccupancy_l4,
		digiOccupancy_fwd,          digiOccupancy_l1,          digiOccupancy_l2,          digiOccupancy_l3,          digiOccupancy_l4,
		rechitOccupancy_fwd,        rechitOccupancy_l1,        rechitOccupancy_l2,        rechitOccupancy_l3,        rechitOccupancy_l4,
		clustOccupancyROCBins_fwd,  clustOccupancyROCBins_l1,  clustOccupancyROCBins_l2,  clustOccupancyROCBins_l3,  clustOccupancyROCBins_l4,
		rechitOccupancyROCBins_fwd, rechitOccupancyROCBins_l1, rechitOccupancyROCBins_l2, rechitOccupancyROCBins_l3, rechitOccupancyROCBins_l4,

	};
	for(auto histoIt = histogramsToSave.begin(); histoIt != histogramsToSave.end(); ++histoIt)
	{
		TCanvas* canvas = custom_can_((*histoIt), std::string((*histoIt) -> GetTitle()) + "_canvas", 0, 0, 800, 800, 80, 140); 
		(*histoIt) -> Draw("COLZ");
		canvas -> cd();
		std::string histoName = (*histoIt) -> GetName();
		if(histoName.find("ROC") != std::string::npos)
		{
			int layer = -1;
			if(histoName.find("_fwd") != std::string::npos) layer = 0;
			if(const size_t pos = histoName.find("_l") != std::string::npos)
			{
				try { layer = histoName.at(pos + 2) - '0'; }
				catch(const std::out_of_range &e) { std::cout << __PRETTY_FUNCTION__ << ": Error while processing the extra histogram names." << std::endl; }
			}
			dress_occup_plot((*histoIt), layer, PHASE_SCENARIO);
		}
		(*histoIt) -> Write();
		canvas     -> Write();
	}
#endif
	ntupleOutputFile_ -> Write();
	ntupleOutputFile_ -> Close();
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizer::endRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {}
void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {}

// BLUE:    "\03334[m"
// RED:     "\03331[m"
// DEFAULT: "\03339[m"

void PhaseIPixelNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	// std::cout << "Analysis: " << std::endl;
	if(isEventFromMc_ != (iEvent.id().run() == 1))
	{
		isEventFromMc_ = iEvent.id().run() == 1;
		std::cout << "Deduced data type: " << (isEventFromMc_ ? "MONTE-CARLO" : "REAL RAW DATA") << "." << std::endl;
	}
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;
	// FED errors
	std::map<uint32_t, int> federrors = NtuplizerHelpers::getFedErrors(iEvent, rawDataErrorToken_);
	// Get vertices
	edm::Handle<reco::VertexCollection>      vertexCollectionHandle;
	iEvent.getByToken(primaryVerticesToken_, vertexCollectionHandle);
	// Get trigger info
	edm::Handle<edm::TriggerResults> triggerResultsHandle;
	iEvent.getByToken(triggerResultsToken_, triggerResultsHandle);
	// Get cluster collection
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle;
	iEvent.getByToken(clustersToken_,                 clusterCollectionHandle);
	// Get Traj-Track Collection
	edm::Handle<TrajTrackAssociationCollection>  trajTrackCollectionHandle;
	iEvent.getByToken(trajTrackCollectionToken_, trajTrackCollectionHandle);
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
	// Pixel Parameter estimator
	edm::ESHandle<PixelClusterParameterEstimator> pixelClusterParameterEstimatorHandle;
	iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", pixelClusterParameterEstimatorHandle);
	pixelClusterParameterEstimator_ = pixelClusterParameterEstimatorHandle.product();
	coord_.init(iSetup);
	getEvtData(iEvent, vertexCollectionHandle, triggerResultsHandle, clusterCollectionHandle, trajTrackCollectionHandle);
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
	std::cout << "Handling simhits." << std::endl;
	std::vector<edm::Handle<edm::PSimHitContainer>> simhitCollectionHandles(simhitCollectionTokens_.size());
	for(unsigned int numToken = 0; numToken < simhitCollectionTokens_.size(); ++numToken)
	{
		std::cout << "NumToken: " << numToken << std::endl;
		// try {iEvent.getByLabel(l1GTReadoutRecTag_,L1GTRR);} catch (...) {;}
		iEvent.getByToken(simhitCollectionTokens_[numToken], simhitCollectionHandles[numToken]);
	}
	std::cout << "Tokens fetched." << std::endl;
	edm::Handle<edm::DetSetVector<PixelDigi>> digiCollectionHandle;
	iEvent.getByToken(pixelDigiCollectionToken_, digiCollectionHandle);
	// std::cout << "Digis: " << std::endl;
	getDigiData(digiCollectionHandle, federrors);
	// std::cout << "Simhits: " << std::endl;
	getSimhitData(simhitCollectionHandles);
#endif
	for(const auto& currentTrackKeypair: *trajTrackCollectionHandle)
	{
		static int numPrintout = 0;
		const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
		const reco::TrackRef                    track = currentTrackKeypair.val;
		if(!(track -> qualityMask() & 4))continue;
		printTrackCompositionInfo(traj, track, clusterCollectionHandle, vertexCollectionHandle, federrors);
		if(numPrintout++ == 1000)
		{
			std::cout << "1000 printouts exceeded." << std::endl;
			break;
		}
	}
	// std::cout << "Clusters: " << std::endl;
	getClustData(clusterCollectionHandle, federrors);
	// std::cout << "TrajTrackData: " << std::endl;
	getTrajTrackData(vertexCollectionHandle, trajTrackCollectionHandle, federrors);
	// std::cout << "end " << std::endl;
}

void PhaseIPixelNtuplizer::getEvtData(const edm::Event& iEvent, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<edm::TriggerResults>& triggerResultsHandle, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle)
{
	// Event info
	// Set data holder object
	evt_.init();
	evt_.fill         = 0; // FIXME
	evt_.run          = iEvent.id().run();
	evt_.ls           = iEvent.luminosityBlock();
	evt_.orb          = iEvent.orbitNumber();
	evt_.bx           = iEvent.bunchCrossing();
	evt_.evt          = iEvent.id().event();
	if(triggerResultsHandle.isValid())
	{
		evt_.trig = 0;
		// Get the trigger names found in the current event
		const edm::TriggerNames& eventTriggerNames = iEvent.triggerNames(*triggerResultsHandle);
		for(size_t eventNumTrigger = 0; eventNumTrigger < eventTriggerNames.size(); eventNumTrigger++)
		{
			std::string eventTriggerNameToTest = eventTriggerNames.triggerNames()[eventNumTrigger];
			// Compare current trigger name to the ones found in the config 
			for(size_t configNumTrigger = 0; configNumTrigger < triggerNames_.size(); configNumTrigger++)
			{
				// If the name starts with the one specified in the configuration
				if(eventTriggerNameToTest.find(triggerNames_[configNumTrigger])) continue;
				/// Check: Has the eventNumTrigger-th path accepted the event?
				if(triggerResultsHandle -> accept(eventNumTrigger) == 0) continue;
				evt_.trig |= (1 << configNumTrigger);
			}
		}
	}
	else
	{
		if(isEventFromMc_)
		{
			evt_.trig = 1; // Assuming that ZeroBias is the first trigger bit
		}
	}
	// Loop on vertices
	evt_.nvtx    = 0;
	evt_.vtxntrk = 0;
	for(const auto &currentVertex: *vertexCollectionHandle)
	{
		// Invalid vertex
		if(!currentVertex.isValid()) continue;
		// Check if it is the best vertex (largest trk number, preferably in the middle
		if(
			(currentVertex.tracksSize() > static_cast<size_t>(evt_.vtxntrk)) ||
			(currentVertex.tracksSize() == static_cast<size_t>(evt_.vtxntrk) &&
			 std::abs(currentVertex.z()) < std::abs(evt_.vtxZ)))
		{
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
		if(
			std::abs(currentVertex.z()) < 24.0 &&
			std::abs(currentVertex.position().rho()) < 2.0 &&
			currentVertex.ndof() >= 4)
		{
			evt_.nvtx++;
		}
	}
	// Quick loop to get number of clusters on layers/disk
	std::fill(evt_.nclu, evt_.nclu + 7, 0);
	std::fill(evt_.npix, evt_.npix + 7, 0);
	if(clusterCollectionHandle.isValid())
	{
		for(const auto &clu_set: *clusterCollectionHandle)
		{
			int layDiskIndex = -1;
			DetId detId(clu_set.id());
			if(detId.subdetId() == PixelSubdetector::PixelBarrel)
				layDiskIndex = trackerTopology_->pxbLayer(detId.rawId()) - 1;
			else
			{
				if(detId.subdetId() == PixelSubdetector::PixelEndcap)
					layDiskIndex = trackerTopology_->pxfDisk(detId.rawId()) + 3;
				else
				{
					continue;
				}
			}
			evt_.nclu[layDiskIndex] += clu_set.size();
			for(const auto &clu: clu_set)
			{
				evt_.npix[layDiskIndex] += clu.size();
			}
		}
	}
	// Quick loop to get track numbers
	evt_.ntracks = 0;
	std::fill(evt_.ntrackFPix, evt_.ntrackFPix + 3, 0);
	std::fill(evt_.ntrackBPix, evt_.ntrackBPix + 4, 0);
	for(const auto &pair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj = pair.key;
		const reco::TrackRef track                   = pair.val;
		// Discarding tracks without pixel measurements
		if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;
		++evt_.ntracks;
		for(const auto &measurement: traj->measurements())
		{
			if(!measurement.updatedState().isValid()) continue;
			DetId detId(measurement.recHit() -> geographicalId());
			if(detId.subdetId() == PixelSubdetector::PixelBarrel)
			{
				evt_.ntrackBPix[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
				if(measurement.recHit() -> getType() == TrackingRecHit::valid)
				{
					evt_.ntrackBPixvalid[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
				}
			}
			else
			{
				if(detId.subdetId() == PixelSubdetector::PixelEndcap)
				{
					evt_.ntrackFPix[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
					if(measurement.recHit() -> getType() == TrackingRecHit::valid)
					{
						evt_.ntrackFPixvalid[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
					}
				}
			}
		}
	}
	eventTree_->Fill();
}

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
void PhaseIPixelNtuplizer::getSimhitData(const std::vector<edm::Handle<edm::PSimHitContainer>>& simhitCollectionHandles)
{
	int invalidHandlesPresent = std::any_of(simhitCollectionHandles.begin(), simhitCollectionHandles.end(), [] (const edm::Handle<std::vector<PSimHit>>& handle) { return !(handle.isValid()); } );
	if(invalidHandlesPresent)
	{
		static int timesReported = 0;
		if(timesReported < 10) std::cout << "Error in: " << __PRETTY_FUNCTION__ << ": One or more of the handles are invalid or missing! Skipping event. (Check the input products!)" << std::endl;
		if(++timesReported == 10) std::cout << "Invalid handles were reported more than 10 times. Omitting further reports!" << std::endl;
		return;
	}
	int numSimHits = 0;
	for(const auto& simhitCollectionHandle: simhitCollectionHandles) numSimHits += simhitCollectionHandle -> size();
	std::cout << "Number of simhits present: " << numSimHits << std::endl;
	for(const auto& simhitCollectionHandle: simhitCollectionHandles)
	{
		for(const PSimHit& simhit : *simhitCollectionHandle)
		{
			DetId detId(simhit.detUnitId());
			unsigned int subdetId = detId.subdetId();
			const GeomDetUnit* geomDetUnit      = trackerGeometry_ -> idToDetUnit(detId);
			LocalPoint simhitLocalCoordinates   = simhit.localPosition();
			GlobalPoint simhitGlobalCoordinates = geomDetUnit -> toGlobal(simhitLocalCoordinates);
			if(subdetId == PixelSubdetector::PixelBarrel)
			{
				static int printCounter = 0;
				int layer = trackerTopology_ -> pxbLayer(detId);
				printCounter++;
				if(printCounter < 20)
					std::cout << __FUNCTION__ << ": Layer: " << layer << " --- " << simhitLocalCoordinates.x() << ", " << simhitLocalCoordinates.y() << ", " << simhitLocalCoordinates.z() << std::endl;
				if(layer == 1) simhitOccupancy_l1 -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
				if(layer == 2) simhitOccupancy_l2 -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
				if(layer == 3) simhitOccupancy_l3 -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
				if(layer == 4) simhitOccupancy_l4 -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
			}
			if(subdetId == PixelSubdetector::PixelEndcap)
			{
				simhitOccupancy_fwd -> Fill(simhitGlobalCoordinates.z(), atan2(simhitGlobalCoordinates.y(), simhitGlobalCoordinates.x()));
			}
		}
	}
}
#endif

#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
void PhaseIPixelNtuplizer::getDigiData(const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	for(const auto& digiDetSet: *digiCollectionHandle)
	{
		DetId detId(digiDetSet.detId());
		unsigned int subdetId = detId.subdetId();
		const GeomDetUnit* geomDetUnit      = trackerGeometry_ -> idToDetUnit(detId);
		for(const auto& digi: digiDetSet)
		{
			int x = digi.column();
			int y = digi.row();
			LocalPoint digiLocalCoordinates(x, y, 0);
			GlobalPoint digiGlobalCoordinates = geomDetUnit -> toGlobal(digiLocalCoordinates);
			if(subdetId == PixelSubdetector::PixelBarrel)
			{
				// constexpr pixelXWidth = 
				// constexpr pixelYWidth = 
				int layer = trackerTopology_ -> pxbLayer(detId);
				if(layer == 1) digiOccupancy_l1 -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
				if(layer == 2) digiOccupancy_l2 -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
				if(layer == 3) digiOccupancy_l3 -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
				if(layer == 4) digiOccupancy_l4 -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
			}
			if(subdetId == PixelSubdetector::PixelEndcap)
			{
				digiOccupancy_fwd -> Fill(digiGlobalCoordinates.z(), atan2(digiGlobalCoordinates.y(), digiGlobalCoordinates.x()));
			}			
		}
	}
}
#endif

void PhaseIPixelNtuplizer::getClustData(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	// Cluster info
	// Trying to access the clusters
	clu_.init();
	if(!clusterCollectionHandle.isValid()) handleDefaultError("data_access", "data_access", "Failed to fetch the clusters.");
	int clusterCounter = 0;
	const edmNew::DetSetVector<SiPixelCluster>& currentClusterCollection = *clusterCollectionHandle;
	// Looping on clusters with the same location
	using clustCollIt_t = edmNew::DetSetVector<SiPixelCluster>::const_iterator;
	for(clustCollIt_t currentClusterSetIt = currentClusterCollection.begin(); currentClusterSetIt != currentClusterCollection.end(); ++currentClusterSetIt)
	{
		const auto& currentClusterSet = *currentClusterSetIt;
		DetId detId(currentClusterSet.id());
		unsigned int subdetId = detId.subdetId();
		// Take only pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap) continue;
		using clustSetIt_t = edmNew::DetSet<SiPixelCluster>::const_iterator;
		for(clustSetIt_t currentClusterIt = currentClusterSet.begin(); currentClusterIt != currentClusterSet.end(); ++currentClusterIt)
		{
			const auto& currentCluster = *currentClusterIt;
			// Serial num of cluster in the given module
			clu_.i = currentClusterIt - currentClusterSet.begin();
			// Set if there is a valid hits
			// clu_.edge;
			// clu_.badpix;
			// clu_.tworoc;
			// Module information
			getModuleData(clu_.mod,    0, detId, federrors);
			getModuleData(clu_.mod_on, 1, detId, federrors);
			getRocData   (clu_.mod,    0, detId, &currentCluster);
			getRocData   (clu_.mod_on, 1, detId, &currentCluster);
#ifdef ADD_CHECK_PLOTS_TO_NTUPLE
			if(clu_.mod_on.det == 0) switch(clu_.mod_on.layer)
			{
				case 1:
					clustOccupancyROCBins_l1 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
					break;
				case 2:
					clustOccupancyROCBins_l2 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
					break;
				case 3:
					clustOccupancyROCBins_l3 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
					break;
				case 4:
					clustOccupancyROCBins_l4 -> Fill(clu_.mod_on.module_coord, clu_.mod_on.ladder_coord);
					break;
			}
			if(clu_.mod_on.det == 1) clustOccupancyROCBins_fwd -> Fill(clu_.mod_on.disk_ring_coord, clu_.mod_on.blade_panel_coord);
#endif
			const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);
			LocalPoint clustLocalCoordinates;
			std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(currentCluster, *geomDetUnit);
			GlobalPoint clustGlobalCoordinates = geomDetUnit -> toGlobal(clustLocalCoordinates);
			// Position and size
			clu_.x     = currentCluster.x();
			clu_.y     = currentCluster.y();
			clu_.lx    = clustLocalCoordinates.x();
			clu_.lx    = clustLocalCoordinates.y();
			clu_.glx   = clustGlobalCoordinates.x();
			clu_.gly   = clustGlobalCoordinates.y();
			clu_.glz   = clustGlobalCoordinates.z();
			clu_.sizeX = currentCluster.sizeX();
			clu_.sizeY = currentCluster.sizeY();
			clu_.size  = currentCluster.size();
			// Charge
			clu_.charge = currentCluster.charge();
			// Misc.
			for(int i = 0; i < clu_.size && i < 1000; ++i)
			{
				const auto& currentPixels = currentCluster.pixels();
				clu_.adc[i]    = currentCluster.pixelADC()[i] / 1000.0;
				clu_.pix[i][0] = currentPixels[i].x;
				clu_.pix[i][1] = currentPixels[i].y;
			}
			// The number of saved clusters can be downscaled to save space
			if(clusterCounter++ % clusterSaveDownscaling_ != 0) continue;
			clustTree_ -> Fill();
		}
	}
}

std::map<reco::TrackRef, TrackData> PhaseIPixelNtuplizer::getTrackData(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	std::map<reco::TrackRef, TrackData> trackDataCollection;
	int trackIndex = 0;
	for(const auto& currentTrackKeypair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
		const reco::TrackRef                    track = currentTrackKeypair.val;
		TrackData* trackField;
		// Create new TrackData instance, when this trackRef is a new one,
		// set track properties and zero out counters
		try { trackField = &(trackDataCollection.at(track)); }
		catch(std::out_of_range e)
		{
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
			reco::VertexCollection::const_iterator closestVtx = NtuplizerHelpers::findClosestVertexToTrack(track, vertexCollectionHandle, 10);
			if(closestVtx != vertexCollectionHandle -> end())
			{
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
			trackDataCollection.insert({track, std::move(newTrackData)});
			trackField = &(trackDataCollection.at(track));
		}
		// Counted here:
		// barrel hits, valid barrel hits, forward hits, valid forward hits,
		// top of detector hits, top of detector hits, strip hits
		// Check measurement validity
		for(const TrajectoryMeasurement& measurement: traj -> measurements())
		{
			if(!measurement.updatedState().isValid()) continue;
			const auto& hit = measurement.recHit();
			// Det id
			DetId detId = hit -> geographicalId();
			uint32_t subDetId = (detId.subdetId());
			// Counting the strip hits
			if(hit -> isValid())
			{
				if(
					subDetId == StripSubdetector::TIB || subDetId == StripSubdetector::TOB ||
					subDetId == StripSubdetector::TID || subDetId == StripSubdetector::TEC)
					{
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
			getModuleData(mod, 0, detId, federrors);
			// Forward and barrel hits
			if(subDetId == PixelSubdetector::PixelBarrel)
			{
				trackField -> bpix[mod.layer - 1]++;
				if(validhit) trackField -> validbpix[mod.layer - 1]++;
			}
			if(subDetId == PixelSubdetector::PixelEndcap)
			{
				trackField -> fpix[mod.disk - 1]++;
				if(validhit) trackField -> validfpix[mod.disk - 1]++;
			}
			// Fetch the hit
			const SiPixelRecHit* pixhit = static_cast<const SiPixelRecHit*>(hit -> hit());
			// Check hit qualty
			if(pixhit)
			{
				// Position measurements
				TrajectoryStateCombiner  trajStateComb;
				TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
				auto globalPosition      = trajStateOnSurface.globalPosition();
				// const float& glx = globalPosition.x();
				const float& gly = globalPosition.y();
				// const float& glz = globalPosition.z();
				// Count top-of-detector tracks traj measurements
				if(0 <= gly && gly != NOVAL_F)
				{
					trackField -> pixhit[0] += 1;
					if(validhit) trackField -> validpixhit[0] += 1;
				}
				// Count bottom-of-detector tracks traj measurements
				if(gly < 0 && gly != NOVAL_F)
				{
					trackField -> pixhit[1] += 1;
					if(validhit) trackField -> validpixhit[1] += 1;
				}
			}
		}
	}
	for(const auto& pair: trackDataCollection)
	{
		track_ = pair.second;
		trackTree_ -> Fill();
	}
	return trackDataCollection;
}

void PhaseIPixelNtuplizer::getTrajTrackData(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	std::map<reco::TrackRef, TrackData> trackDataCollection(getTrackData(vertexCollectionHandle, trajTrackCollectionHandle, federrors));
	// Trajectory measurement loop
	for(const auto& currentTrackKeypair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
		const reco::TrackRef                    track = currentTrackKeypair.val;
		// Discarding tracks without pixel measurements
		if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;
		// Trajectory info
		traj_.init();
		track_ = trackDataCollection.at(track);
		for(const TrajectoryMeasurement& measurement: traj -> measurements())
		{
			// Check measurement validity
			if(!measurement.updatedState().isValid()) continue;
			TransientTrackingRecHit::ConstRecHitPointer recHit = measurement.recHit();
			// Only looking for pixel hits
			DetId detId = recHit -> geographicalId();
			if(!NtuplizerHelpers::detidIsOnPixel(detId)) continue;
			// We could match recHits to simhits, here, using recipe:
			//   https://github.com/cms-analysis/DPGAnalysis-SiPixelTools/blob/master/PixelHitAssociator/test/SiPixelRecHitsValid_pix.cc#L573-L632
			//TransientTrackingRecHit::ConstRecHitPointer hit = tmeasIt -> recHit();
			//if (detId == hit -> geographicalId()) matchToSimHits(associate, &(*hit), detId, theGeomDet,trackerTopology_);
			// Looking for valid and missing hits
			traj_.validhit = recHit -> getType() == TrackingRecHit::valid;
			traj_.missing  = recHit -> getType() == TrackingRecHit::missing;
			// Save module data
			getModuleData(traj_.mod,    0, detId, federrors);
			getModuleData(traj_.mod_on, 1, detId, federrors);
			getRocData   (traj_.mod,    0, static_cast<const SiPixelRecHit*>(recHit->hit()));
			getRocData   (traj_.mod_on, 1, static_cast<const SiPixelRecHit*>(recHit->hit()));
			if(traj_.mod_on.layer == 2 || traj_.mod_on.disk == 1)
			{
				// std::vector<TrajMeasurement> measurementsFromTracksPropagatedToLayer1 = propagateTrackToLayer1();
			}
			// Position measurements
			const GeomDetUnit* geomDetUnit = recHit -> detUnit();
			static TrajectoryStateCombiner trajStateComb; // operator () should be const, so this is fine as a static variable
			TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
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
			if(recHit -> isValid() && recHit -> hit() != 0)
			{
				const SiPixelRecHit *hit = (const SiPixelRecHit*)recHit -> hit();
				SiPixelRecHit::ClusterRef const& clust = hit -> cluster();
				if(clust.isNonnull())
				{
					LocalPoint clustLocalCoordinates;
					std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(*clust, *geomDetUnit);
					GlobalPoint clustGlobalCoordinates = geomDetUnit -> toGlobal(clustLocalCoordinates);
					traj_.clu.charge = clust -> charge() / 1000.0f;
					traj_.clu.size   = clust -> size();
					traj_.clu.edge   = hit -> isOnEdge() ? 1 : 0;
					traj_.clu.badpix = hit -> hasBadPixels() ? 1 : 0;
					traj_.clu.tworoc = hit -> spansTwoROCs() ? 1 : 0;
					traj_.clu.sizeX  = clust -> sizeX();
					traj_.clu.sizeY  = clust -> sizeY();
					traj_.clu.x      = clust -> x();
					traj_.clu.y      = clust -> y();
					traj_.clu.lx     = clustLocalCoordinates.x();
					traj_.clu.lx     = clustLocalCoordinates.y();
					traj_.clu.glx    = clustGlobalCoordinates.x();
					traj_.clu.gly    = clustGlobalCoordinates.y();
					traj_.clu.glz    = clustGlobalCoordinates.z();
					for(int i = 0; i < clust -> size() && i < 1000; i++)
					{
						traj_.clu.adc[i]    = static_cast<float>(clust -> pixelADC()[i]) / 1000.0f;
						traj_.clu.pix[i][0] = ((clust -> pixels())[i]).x;
						traj_.clu.pix[i][1] = ((clust -> pixels())[i]).y;
					}
					traj_.norm_charge = traj_.clu.charge * sqrt(1.0f / (1.0f / pow(tan(traj_.alpha), 2) + 1.0f / pow(tan(traj_.beta), 2) + 1.0f));
					traj_.dx_cl       = std::abs(clustLocalCoordinates.x() - traj_.lx);
					traj_.dy_cl       = std::abs(clustLocalCoordinates.y() - traj_.ly);
					traj_.d_cl        = sqrt(traj_.dx_cl * traj_.dx_cl + traj_.dy_cl * traj_.dy_cl);
				}
			}
			// Get closest other traj measurement
			NtuplizerHelpers::getClosestOtherTrackDistanceByLooping(measurement, trajTrackCollectionHandle, traj_.d_tr, traj_.dx_tr, traj_.dy_tr);
			traj_.hit_near = (traj_.d_tr < 0.5); // 5 mm
			traj_.clust_near = (traj_.d_cl != NOVAL_F && traj_.d_cl < 0.05); // 500 um
			// Filling the tree
			trajTree_ -> Fill();
		}
	}
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

void PhaseIPixelNtuplizer::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::string msg)
{
	edm::LogError(streamType.c_str()) << "\03331[m" << msg << "\03339[m" << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::vector<std::string> msg)
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

void PhaseIPixelNtuplizer::getModuleData(ModuleData &mod, bool online, const DetId &detId, const std::map<uint32_t, int> &federrors)
{
	mod.init();
	mod.det  = detId.subdetId() - 1;
	mod.shl  = coord_.quadrant(detId);
	mod.side = coord_.side(detId);
	if(detId.subdetId() == PixelSubdetector::PixelBarrel)
	{
		mod.sec     = coord_.sector(detId);
		mod.half    = coord_.half(detId);
		mod.layer   = coord_.layer(detId);
		mod.flipped = coord_.flipped(detId); // opposite of outer
		if(online)
		{
			mod.ladder = coord_.signed_ladder(detId);
			mod.module = coord_.signed_module(detId);
		}
		else
		{
			mod.ladder = coord_.ladder(detId);
			mod.module = coord_.module(detId);
		}
	}
	else if(detId.subdetId() == PixelSubdetector::PixelEndcap)
	{
		mod.ring   = coord_.ring(detId);
		mod.panel  = coord_.panel(detId);
		mod.module = coord_.module(detId);
		if(online)
		{
			mod.disk  = coord_.signed_disk(detId);
			mod.blade = coord_.signed_blade(detId);
		}
		else
		{
			mod.disk  = coord_.disk(detId);
			mod.blade = coord_.blade(detId);
		}
	}
	mod.rawid = detId.rawId();
	mod.fedid = coord_.fedid(detId);
	// FED error
	std::map<uint32_t, int>::const_iterator federrors_it =
		federrors.find(detId.rawId());
	mod.federr = (federrors_it != federrors.end()) ? federrors_it->second : 0;
}
void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const DetId &detId, const PixelDigi *digi)
{
	mod.channel = coord_.channel(detId, digi);
	mod.roc     = coord_.roc(detId, digi);
	if(detId.subdetId() == PixelSubdetector::PixelBarrel)
	{
		if(online)
		{
			mod.module_coord = coord_.signed_module_coord(detId, digi);
			mod.ladder_coord = coord_.signed_ladder_coord(detId, digi);
		}
		else
		{
			mod.module_coord = coord_.module_coord(detId, digi);
			mod.ladder_coord = coord_.ladder_coord(detId, digi);
		}
	}
	else if(detId.subdetId() == PixelSubdetector::PixelEndcap)
	{
		if(online)
		{
			mod.disk_ring_coord   = coord_.signed_disk_ring_coord(detId, digi);
			mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(detId, digi);
		}
		else
		{
			mod.disk_ring_coord   = coord_.disk_ring_coord(detId, digi);
			mod.blade_panel_coord = coord_.blade_panel_coord(detId, digi);
		}
	}
}
void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const DetId &detId, const SiPixelCluster *cluster)
{
	mod.channel = coord_.channel(detId, cluster);
	mod.roc     = coord_.roc(detId, cluster);
	if(detId.subdetId() == PixelSubdetector::PixelBarrel)
	{
		if(online)
		{
			mod.module_coord = coord_.signed_module_coord(detId, cluster);
			mod.ladder_coord = coord_.signed_ladder_coord(detId, cluster);
		}
		else
		{
			mod.module_coord = coord_.module_coord(detId, cluster);
			mod.ladder_coord = coord_.ladder_coord(detId, cluster);
		}
	}
	else if(detId.subdetId() == PixelSubdetector::PixelEndcap)
	{
		if(online)
		{
			mod.disk_ring_coord   = coord_.signed_disk_ring_coord(detId, cluster);
			mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(detId, cluster);
		}
		else
		{
			mod.disk_ring_coord   = coord_.disk_ring_coord(detId, cluster);
			mod.blade_panel_coord = coord_.blade_panel_coord(detId, cluster);
		}
	}
}

void PhaseIPixelNtuplizer::getRocData(ModuleData &mod, bool online, const SiPixelRecHit *rechit)
{
	mod.channel = coord_.channel(rechit);
	mod.roc     = coord_.roc(rechit);
	if(rechit->geographicalId().subdetId() == PixelSubdetector::PixelBarrel)
	{
		if(online)
		{
			mod.module_coord = coord_.signed_module_coord(rechit);
			mod.ladder_coord = coord_.signed_ladder_coord(rechit);
		}
		else
		{
			mod.module_coord = coord_.module_coord(rechit);
			mod.ladder_coord = coord_.ladder_coord(rechit);
		}
	}
	else if(rechit->geographicalId().subdetId() == PixelSubdetector::PixelEndcap)
	{
		if(online)
		{
			mod.disk_ring_coord   = coord_.signed_disk_ring_coord(rechit);
			mod.blade_panel_coord = coord_.signed_shifted_blade_panel_coord(rechit);
		}
		else
		{
			mod.disk_ring_coord   = coord_.disk_ring_coord(rechit);
			mod.blade_panel_coord = coord_.blade_panel_coord(rechit);
		}
	}
}

void PhaseIPixelNtuplizer::propagateTrackToLayer1(const edm::Ref<std::vector<Trajectory>>& trajectory, const reco::TrackRef track, const std::map<uint32_t, int>& federrors)
{
	std::vector<TrajectoryMeasurement> expectedTrajMeasurements;
	for(const TrajectoryMeasurement& trajMeasurement: trajectory -> measurements())
	{
		TransientTrackingRecHit::ConstRecHitPointer predictedRecHit = trajMeasurement.recHit();
		ModuleData predictedHitMod;
		getModuleData(predictedHitMod, 1, predictedRecHit -> geographicalId().rawId(), federrors);
		static TrajectoryStateCombiner trajStateComb;
		TrajectoryStateOnSurface predictedTrajState = trajStateComb(trajMeasurement.forwardPredictedState(), trajMeasurement.backwardPredictedState());
		GlobalPoint predictedTrajStateGlobalPosition = predictedTrajState.globalPosition();
		// float predictedX = predictedTrajState.globalPosition().x();
		// float predictedY = predictedTrajState.globalPosition().y();
		// float predictedZ = predictedTrajState.globalPosition().z();
		// LocalPoint predictedTrajStateLocalPosition = predictedTrajState.localPosition();
		if(predictedHitMod.det == 0 && predictedHitMod.layer == 1)
		{
			std::vector<TrajectoryMeasurement>::const_iterator closestMeasurementIt;
			float closestMeasurementDistance;
			std::tie(closestMeasurementIt, closestMeasurementDistance) = findMatchingTrajMeasurement(predictedTrajStateGlobalPosition, predictedHitMod, expectedTrajMeasurements, federrors);
			if(closestMeasurementIt != expectedTrajMeasurements.end())
			{
				expectedTrajMeasurements.erase(closestMeasurementIt);
			}
		}
		if((predictedHitMod.det == 0 && predictedHitMod.layer == 2) || (predictedHitMod.det == 1 && predictedHitMod.disk == 1))
		{
			if(predictedRecHit -> isValid())
			{

			}
		}

	}
}

std::tuple<std::vector<TrajectoryMeasurement>::const_iterator, float>
	PhaseIPixelNtuplizer::findMatchingTrajMeasurement(const GlobalPoint& referencePoint, const ModuleData& referenceModInfo, const std::vector<TrajectoryMeasurement>& possibleMatches, const std::map<uint32_t, int>& federrors)
{
	using NtuplizerHelpers::areIdenticalModules;
	auto isMeasurementOnReferenceModule = [&] (const TrajectoryMeasurement& measurement)
	{
		ModuleData mod; 
		getModuleData(mod, 1, measurement.recHit() -> geographicalId().rawId(), federrors);
		return areIdenticalModules(mod, referenceModInfo);
	};
	auto bestMatch = std::find_if(possibleMatches.begin(), possibleMatches.end(), isMeasurementOnReferenceModule);
	if(bestMatch == possibleMatches.end()) return std::make_tuple(bestMatch, NOVAL_F);
	float closestDistanceSquared = trajMeasGlobalPointDistanceSquared(*bestMatch, referencePoint);
	for(auto measurementsIt = bestMatch + 1; measurementsIt != possibleMatches.end(); ++measurementsIt)
	{
		if(!isMeasurementOnReferenceModule(*measurementsIt)) continue;
		float distanceSquared = trajMeasGlobalPointDistanceSquared(*measurementsIt, referencePoint);
		if(distanceSquared < closestDistanceSquared)
		{
			closestDistanceSquared = std::move(distanceSquared);
			bestMatch = measurementsIt;
		}
	}
	return std::make_tuple(bestMatch, std::sqrt(closestDistanceSquared));
}

float PhaseIPixelNtuplizer::trajMeasGlobalPointDistanceSquared(const TrajectoryMeasurement& trajMeasurement, const GlobalPoint& referencePoint)
{
	const GlobalPoint measurementPosition = trajMeasurement.updatedState().globalPosition();
	return (referencePoint - measurementPosition).mag2();
}

const SiPixelCluster* PhaseIPixelNtuplizer::getClosestClusterOnDetSetToPoint(const edmNew::DetSet<SiPixelCluster>& clustersOnDet, const LocalPoint& referencePoint)
{
	if(clustersOnDet.empty()) return nullptr;
	const DetId detId = clustersOnDet.id();
	const SiPixelCluster* minDistanceCluster = clustersOnDet.begin();
	float currentMinValueSquared = clusterPointDistanceSquared(detId, *minDistanceCluster, referencePoint);
	for(const auto& cluster: clustersOnDet)
	{
        float currentDistanceSquared = clusterPointDistanceSquared(detId, cluster, referencePoint);
        if(currentDistanceSquared < currentMinValueSquared)
        {
        	currentMinValueSquared = std::move(currentDistanceSquared);
        	minDistanceCluster = &cluster;
        }
	}
	return minDistanceCluster;
}

float PhaseIPixelNtuplizer::clusterPointDistanceSquared(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint)
{
	const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);
	LocalPoint clustLocalCoordinates;
	std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(cluster, *geomDetUnit);
	float xDist = clustLocalCoordinates.x() - referencePoint.x();
	float yDist = clustLocalCoordinates.y() - referencePoint.y();
	float zDist = clustLocalCoordinates.z() - referencePoint.z();
	return xDist * xDist + yDist * yDist + zDist * zDist;
}

LocalPoint PhaseIPixelNtuplizer::clusterPointDistanceVector(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint)
{
	const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(detId);
	LocalPoint clustLocalCoordinates;
	std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(cluster, *geomDetUnit);
	return LocalPoint(clustLocalCoordinates.x() - referencePoint.x(), clustLocalCoordinates.y() - referencePoint.y(), clustLocalCoordinates.z() - referencePoint.z());
}

float PhaseIPixelNtuplizer::clusterPointDistance(const DetId& detId, const SiPixelCluster& cluster, const LocalPoint& referencePoint)
{
	return std::sqrt(clusterPointDistanceSquared(detId, cluster, referencePoint));
}

void PhaseIPixelNtuplizer::printTrackCompositionInfo(const edm::Ref<std::vector<Trajectory>>& trajectory, const reco::TrackRef& track, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	const auto& trajMeasurements = trajectory -> measurements();
	size_t trajMeasCount   = trajMeasurements.size();
	size_t numValidHits    = std::count_if(trajMeasurements.begin(), trajMeasurements.end(), [] (const auto& meas) { return meas.recHit() -> getType() == TrackingRecHit::valid;   });
	size_t numMissingHits  = std::count_if(trajMeasurements.begin(), trajMeasurements.end(), [] (const auto& meas) { return meas.recHit() -> getType() == TrackingRecHit::missing; });
	float  trackPt = track -> pt();
	reco::VertexCollection::const_iterator closestVtx = NtuplizerHelpers::findClosestVertexToTrack(track, vertexCollectionHandle, 10);
	float  trackD0 = NOVAL_F;
	float  trackDz = NOVAL_F;
	if(closestVtx != vertexCollectionHandle -> end())
	{
		trackD0 = track -> dxy(closestVtx -> position()) * -1.0;
		trackDz = track -> dz (closestVtx -> position());	
	}
	std::cout << " --- Begin track informations --- " << std::endl;
	std::cout << "Informations on a track consisting of " << trajMeasCount << " trajectory measurements: " << std::endl;
	std::cout << "Number of valid   hits: " << numValidHits   << std::endl;
	std::cout << "Number of missing hits: " << numMissingHits << std::endl;
	std::cout << "Track d0: " << trackD0 << std::endl;
	std::cout << "Track dz: " << trackDz << std::endl;
	std::cout << "Track pt: " << trackPt << std::endl;
	std::cout << "Trajectory measurement position list: " << std::endl;
	for(const auto& trajMeasurement: trajMeasurements)
	{
		ModuleData mod;
		DetId trajMeasDetId(trajMeasurement.recHit() -> geographicalId().rawId());
		uint32_t subDetId = (trajMeasDetId.subdetId());
		const GeomDetUnit* geomDetUnit = trackerGeometry_ -> idToDetUnit(trajMeasDetId);
		getModuleData(mod, 0, trajMeasDetId, federrors);
		static TrajectoryStateCombiner trajStateComb;
		TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(trajMeasurement.forwardPredictedState(), trajMeasurement.backwardPredictedState());
		GlobalPoint globalPosition     = trajStateOnSurface.globalPosition();
		LocalPoint  localPosition      = trajStateOnSurface.localPosition();
		LocalError  localPositionError = trajStateOnSurface.localError().positionError();
		const SiPixelCluster* closestCluster = nullptr;
		for(const auto& currentClusterSet: *clusterCollectionHandle)
		{
			DetId clusterDetSetDetId(currentClusterSet.id());
			if(clusterDetSetDetId!= trajMeasDetId) continue;
			closestCluster = getClosestClusterOnDetSetToPoint(currentClusterSet, localPosition);
			break;
		}
		std::string detectorPartName = "unknown detector part";
		if(subDetId == PixelSubdetector::PixelBarrel) detectorPartName = "pixel barrel";
		if(subDetId == PixelSubdetector::PixelEndcap) detectorPartName = "pixel forward";
		if(subDetId == StripSubdetector::TIB) detectorPartName = "strip TIB";
		if(subDetId == StripSubdetector::TOB) detectorPartName = "strip TOB";
		if(subDetId == StripSubdetector::TID) detectorPartName = "strip TID";
		if(subDetId == StripSubdetector::TEC) detectorPartName = "strip TEC";
		std::cout << "Trajectory measurement on " << detectorPartName << "." << std::endl;
		if(mod.det == 0)
		{
			std::cout << "Layer: " << mod.layer << ", ladder: " << mod.ladder << ", module: " << mod.module << std::endl;
		}
		if(mod.det == 1)
		{
			std::cout << "Disk: " << mod.disk << ", ring number: " << mod.ring << ", blade: " << mod.blade << ", panel: " << mod.panel << std::endl;
		}
		std::cout << "Global position of the hit  ("  <<
			std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.x()      << ", "  <<
			std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.y()      << ", " <<
			std::setw(8) << std::fixed << std::setprecision(2) << globalPosition.z()      << ")." << std::endl;
		std::cout << "Local position of the hit   ("  <<
			std::setw(8) << std::fixed << std::setprecision(2) << localPosition.x()       << ",  " <<
			std::setw(8) << std::fixed << std::setprecision(2) << localPosition.y()       << ", " <<
			std::setw(8) << std::fixed << std::setprecision(2) << localPosition.z()       << ")." << std::endl;
		std::cout << "Local pos. error of the hit ("  <<
			std::setw(8) << std::fixed << std::setprecision(5) << localPositionError.xx() << ", "  <<
			std::setw(8) << std::fixed << std::setprecision(5) << localPositionError.xy() << ", " <<
			std::setw(8) << std::fixed << std::setprecision(5) << localPositionError.yy() << ")." << std::endl;
		if(closestCluster)
		{
			LocalPoint clustLocalCoordinates;
			std::tie(clustLocalCoordinates, std::ignore, std::ignore) = pixelClusterParameterEstimator_ -> getParameters(*closestCluster, *geomDetUnit);
			std::cout << "Local position of the closest cluster to predicted hit on the same module: " << std::endl;
			std::cout << "("  << clustLocalCoordinates.x() << ",  " << clustLocalCoordinates.y() << ",  " << clustLocalCoordinates.z() << ")." << std::endl;
			std::cout << "Distance from the predicted hit: " << clusterPointDistanceVector(trajMeasDetId, *closestCluster, localPosition) << std::endl;
			std::cout << "Abs distance: " << clusterPointDistance(trajMeasDetId, *closestCluster, localPosition) << std::endl;
		}
	}
	std::cout << " --- End track informations --- " << std::endl;
}

namespace NtuplizerHelpers
{
	std::map<uint32_t, int> getFedErrors(const edm::Event& iEvent, const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& rawDataErrorToken)
	{
		std::map<uint32_t, int> federrors;
		edm::Handle<edm::DetSetVector<SiPixelRawDataError>> siPixelRawDataErrorCollectionHandle;
		iEvent.getByToken(rawDataErrorToken,                siPixelRawDataErrorCollectionHandle);
		// Return empty map if no fed error entry is available
		if(!siPixelRawDataErrorCollectionHandle.isValid()) return federrors;
		// Loop on errors
		for(const auto& pixel_error_set: *siPixelRawDataErrorCollectionHandle)
		{
			for(const auto& pixel_error: pixel_error_set)
			{
				if(pixel_error_set.detId() != 0xffffffff)
				{
					DetId detId(pixel_error_set.detId());
					int type = pixel_error.getType();
					federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
				}
			}
		}
		return federrors;
	}
	bool detidIsOnPixel(const DetId& detid)
	{
		if (detid.det()!=DetId::Tracker) return false;
		if (detid.subdetId() == PixelSubdetector::PixelBarrel) return true;
		if (detid.subdetId() == PixelSubdetector::PixelEndcap) return true;
		return false;
	}
	bool areIdenticalModules(const ModuleData& lhs, const ModuleData& rhs)
	{
		if(lhs.det != rhs.det) return false;
		if(rhs.det == 0)
		{
			if( lhs.layer  != rhs.layer  ||
				lhs.ladder != rhs.ladder || 
				lhs.module != rhs.module) return false;
			return true;
		}
		if(rhs.det == 1)
		{
			if( lhs.disk  != rhs.disk  ||
				lhs.ring != rhs.ring || 
				lhs.blade != rhs.blade || 
				lhs.panel != rhs.panel) return false;	
			return true;
		}
		std::cout << "Error in " << __PRETTY_FUNCTION__ << ": unidentified module comparison." << std::endl;
		return false;
	}
	int trajectoryHasPixelHit(const edm::Ref<std::vector<Trajectory>>& trajectory)
	{
		// Looping on the full track to check if we have pixel hits 
		// and to count the number of strip hits 
		for(auto& measurement: trajectory -> measurements())
		{
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
	reco::VertexCollection::const_iterator findClosestVertexToTrack(const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const unsigned int& minTracks)
	{
		auto isVertexGood        = [&] (const auto& vertex) { return vertex.isValid() && minTracks <= vertex.tracksSize(); };
		auto trackVertexDistance = [&] (const reco::VertexCollection::const_iterator& vertexIt)
		{
			double trkVtxD0 = track -> dxy(vertexIt -> position()) * -1.0;
			double trkVtxDz = track -> dz (vertexIt -> position());
			return sqrt(trkVtxD0 * trkVtxD0 + trkVtxDz * trkVtxDz);
		};
		reco::VertexCollection::const_iterator closestVtx = 
			std::find_if(vertexCollectionHandle -> begin(), vertexCollectionHandle -> end(), [&] (const auto& vertex) { return isVertexGood(vertex); });
		if(closestVtx == vertexCollectionHandle -> end()) return closestVtx;
		double minDistance = trackVertexDistance(closestVtx);
		for(reco::VertexCollection::const_iterator it = closestVtx; it != vertexCollectionHandle -> end(); ++it)
		{
			// Filter out invalid vertices
			if(!isVertexGood(*it)) continue;
			// Comparing squareroots should be quick enough, if required, change this to a comparison of squares
			double trkVtxDist = trackVertexDistance(it);
			if(trkVtxDist < minDistance)
			{
				minDistance = std::move(trkVtxDist);
				closestVtx  = it;
			}
		}
		return closestVtx;
	}
	std::pair<float, float> getLocalXY(const TrajectoryMeasurement& measurement)
	{
		std::pair<float, float> returnValue;
		static TrajectoryStateCombiner trajStateComb;
		TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
		LocalPoint localPosition = trajStateOnSurface.localPosition();
		returnValue.first  = localPosition.x();
		returnValue.second = localPosition.y();
		return returnValue;
	}
	float trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs)
	{
		std::pair<float, float> lhsLocalXY = getLocalXY(lhs);
		std::pair<float, float> rhsLocalXY = getLocalXY(rhs);
		float dxHit = lhsLocalXY.first  - rhsLocalXY.first;
		float dyHit = lhsLocalXY.second - rhsLocalXY.second;
		float distanceSquared = dxHit * dxHit + dyHit * dyHit;
		return distanceSquared;
	}
	void trajMeasurementDistanceSquared(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distanceSquared, float& dxSquared, float& dySquared)
	{
		std::pair<float, float> lhsLocalXY = getLocalXY(lhs);
		std::pair<float, float> rhsLocalXY = getLocalXY(rhs);
		float dxHit = lhsLocalXY.first  - rhsLocalXY.first;
		float dyHit = lhsLocalXY.second - rhsLocalXY.second;
		dxSquared = dxHit * dxHit;
		dySquared = dyHit * dyHit;
		distanceSquared = dxSquared + dySquared;
	}
	void trajMeasurementDistance(const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs, float& distance, float& dx, float& dy)
	{
		trajMeasurementDistanceSquared(lhs, rhs, distance, dx, dy);
		distance = sqrt(distance);
		dx       = sqrt(dx);
		dy       = sqrt(dy);
		if((dx == NOVAL_F) || (dy == NOVAL_F)) distance = NOVAL_F;
	}
	void getClosestOtherTrackDistanceByLooping(const TrajectoryMeasurement& measurement, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, float& distance, float& dx, float& dy)
	{
		dx = NOVAL_F;
		dy = NOVAL_F;
		std::vector<TrajectoryMeasurement>::const_iterator closestMeasurementIt = trajTrackCollectionHandle -> begin() -> key -> measurements().begin();
		if(&*closestMeasurementIt == &measurement) ++closestMeasurementIt;
		double closestTrajMeasurementDistanceSquared = trajMeasurementDistanceSquared(measurement, *closestMeasurementIt);
		for(const auto& otherTrackKeypair: *trajTrackCollectionHandle)
		{
			const edm::Ref<std::vector<Trajectory>> otherTraj = otherTrackKeypair.key;
			for(auto otherTrajMeasurementIt = otherTraj -> measurements().begin(); otherTrajMeasurementIt != otherTraj -> measurements().end(); ++otherTrajMeasurementIt)
			{
				if(&*otherTrajMeasurementIt == &measurement) continue;
				float distanceSquared = trajMeasurementDistanceSquared(measurement, *otherTrajMeasurementIt);
				if(distanceSquared < closestTrajMeasurementDistanceSquared)
				{
					closestMeasurementIt = otherTrajMeasurementIt;
					closestTrajMeasurementDistanceSquared = distanceSquared;
				}
			}
		}
		trajMeasurementDistance(measurement, *closestMeasurementIt, distance, dx, dy);
	}
	// THIS DOES NOT WORK !!!
	// int getTrackParentVtxNumTracks(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const reco::TrackRef trackToFind)
	// {
	// 	const auto trackToFindAsPtr = trackToFind.get();
	// 	std::cout << "Track ptr: " << trackToFindAsPtr << std::endl;
	// 	for(const auto &currentVertex: *vertexCollectionHandle)
	// 	{
	// 		for(reco::Vertex::trackRef_iterator trackRefIt = currentVertex.tracks_begin(); trackRefIt != currentVertex.tracks_end(); ++trackRefIt)
	// 		{
	// 			const auto comparableVtxTrackPtr = trackRefIt -> get();
	// 			std::cout << "Vertex track ptr: " << comparableVtxTrackPtr << std::endl;
	// 			if(trackToFindAsPtr == comparableVtxTrackPtr)
	// 			{
	// 				return currentVertex.tracksSize();
	// 			}
	// 		}
	// 	}	
	// 	return NOVAL_I;
	// } 
} // NtuplizerHelpers


DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);
