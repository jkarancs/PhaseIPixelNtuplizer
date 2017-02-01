#include "PhaseIPixelNtuplizer.h"

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	iConfig_                = iConfig;
	clusterSaveDownscaling_ = 1;
	// Tokens
	rawDataErrorToken_        = consumes<edm::DetSetVector<SiPixelRawDataError> >(edm::InputTag("siPixelDigis"));
	primaryVerticesToken_     = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
	clustersToken_            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
	trajTrackCollectionToken_ = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
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
	eventTree_ -> Branch("event",  &evt_,         evt_        .list.c_str());
	// Cluster tree
	clustTree_ -> Branch("event",  &evt_,         evt_        .list.c_str());
	clustTree_ -> Branch("mod_on", &clu_.mod_on,  clu_.mod_on .list.c_str());
	clustTree_ -> Branch("mod",    &clu_.mod,     clu_.mod    .list.c_str());
	clustTree_ -> Branch("clust",  &clu_,         clu_        .list.c_str());
	// Track tree
	trackTree_ -> Branch("event",  &evt_,         evt_        .list.c_str());
	trackTree_ -> Branch("track",  &track_,       track_      .list.c_str());
	// Trajectory tree
	trajTree_  -> Branch("event",  &evt_,         evt_        .list.c_str());
	trajTree_  -> Branch("mod_on", &traj_.mod_on, traj_.mod_on.list.c_str());
	trajTree_  -> Branch("mod",    &traj_.mod,    traj_.mod   .list.c_str());
	trajTree_  -> Branch("clust",  &traj_.clu,    traj_.clu   .list.c_str());
	trajTree_  -> Branch("track",  &track_,       track_      .list.c_str());
	trajTree_  -> Branch("traj",   &traj_,        traj_       .list.c_str());
}

void PhaseIPixelNtuplizer::endJob() 
{
	LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename_ << "\"." << std::endl;
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
	coord_.init(iSetup);
	getEvtInfo(iEvent, vertexCollectionHandle, clusterCollectionHandle, trajTrackCollectionHandle);
	getClustInfo(clusterCollectionHandle, federrors);
	getTrajTrackInfo(vertexCollectionHandle, trajTrackCollectionHandle, federrors);
}

void PhaseIPixelNtuplizer::getEvtInfo(const edm::Event& iEvent, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle)
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
	// Loop on vertices
	evt_.nvtx               = 0;
	evt_.vtxntrk = 0;
	for(const auto& current_vertex: *vertexCollectionHandle)
	{
		// Invalid vertex
		if(!current_vertex.isValid()) continue;
		// Check if it is the best vertex (largest trk number, preferably in the middle
		if(
			(current_vertex.tracksSize()  > static_cast<size_t>(evt_.vtxntrk)) ||
			(current_vertex.tracksSize() == static_cast<size_t>(evt_.vtxntrk)  && 
			std::abs(current_vertex.z()) < std::abs(evt_.vtxZ)))
		{
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
		if(
			std::abs(current_vertex.z())              <  24.0 && 
			std::abs(current_vertex.position().rho()) <  2.0  &&
			current_vertex.ndof()                     >= 4)
		{
			evt_.nvtx++;
		}
	}
	// Quick loop to get number of clusters on layers/disk
	std::fill(evt_.nclu, evt_.nclu + 6, 0);
	std::fill(evt_.npix, evt_.npix + 6, 0);
	if(clusterCollectionHandle.isValid())
	{
		for(const auto& clu_set: *clusterCollectionHandle)
		{
			int layDiskIndex = -1;
			DetId detId(clu_set.id());
			if(detId.subdetId() == PixelSubdetector::PixelBarrel) layDiskIndex = trackerTopology_ -> pxbLayer(detId.rawId()) - 1;
			else
			{
				if(detId.subdetId() == PixelSubdetector::PixelEndcap) layDiskIndex = trackerTopology_ -> pxfDisk(detId.rawId()) + 3;
				else
				{
					continue;
				}
			}
			evt_.nclu[layDiskIndex] += clu_set.size();
			for(const auto& clu: clu_set)
			{
				evt_.npix[layDiskIndex] += clu.size();
			}
		}
	}
	// Quick loop to get track numbers
	evt_.ntracks = 0;
	std::fill(evt_.ntrackBPix, evt_.ntrackBPix + 3, 0);
	std::fill(evt_.ntrackFPix, evt_.ntrackFPix + 2, 0);
	for(const auto& pair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj  = pair.key;
		const reco::TrackRef                    track = pair.val;
		// Discarding tracks without pixel measurements
		if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;
		++evt_.ntracks;
		for(const auto& meas: traj -> measurements())
		{
			if(!meas.updatedState().isValid()) continue;
			DetId detId(meas.recHit() -> geographicalId());
			if(detId.subdetId() == PixelSubdetector::PixelBarrel)
			{
				evt_.ntrackBPix[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
				if(meas.recHit() -> getType() == TrackingRecHit::valid)
				{
					evt_.ntrackBPixvalid[trackerTopology_ -> pxbLayer(detId.rawId()) - 1]++;
				}
			}
			else
			{
				if(detId.subdetId() == PixelSubdetector::PixelEndcap)
				{
					evt_.ntrackFPix[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
					if(meas.recHit() -> getType() == TrackingRecHit::valid)
					{
						evt_.ntrackFPixvalid[trackerTopology_ -> pxfDisk(detId.rawId()) - 1]++;
					}
				}
			}
		}
	}
	eventTree_ -> Fill();
}


void PhaseIPixelNtuplizer::getClustInfo(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const std::map<uint32_t, int>& federrors)
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
			// Position and size
			clu_.x     = currentCluster.x();
			clu_.y     = currentCluster.y();
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

void PhaseIPixelNtuplizer::getTrackInfo(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Ref<std::vector<Trajectory>>& traj, const reco::TrackRef& track, const std::map<uint32_t, int>& federrors, const int& trackIndex)
{
	reco::VertexCollection::const_iterator closestVtx = NtuplizerHelpers::findClosestVertexToTrack(track, vertexCollectionHandle);
	// Initialize track data
	// FIXME: Move conter zeroing to the DataStructures
	track_.init();
	// Zeroing counters
	std::fill(track_.validfpix,   track_.validfpix   + 2, 0);
	std::fill(track_.validbpix,   track_.validbpix   + 3, 0);
	std::fill(track_.pixhit,      track_.pixhit      + 1, 0);
	std::fill(track_.validpixhit, track_.validpixhit + 1, 0);
	std::fill(track_.bpix,        track_.bpix        + 3, 0);
	std::fill(track_.fpix,        track_.fpix        + 2, 0);
	track_.strip          = 0;
	// Basic track quantities
	track_.i       = trackIndex;
	track_.quality = track -> qualityMask();
	track_.pt      = track -> pt();
	track_.p       = track -> p();
	track_.eta     = track -> eta();
	track_.theta   = track -> theta();
	track_.phi     = track -> phi();
	// FIXME: use best vertex selection instead of closest vertex selection
	track_.d0      = track -> dxy(closestVtx -> position()) * -1.0;
	track_.dz      = track -> dz(closestVtx -> position());
	// Counted in this loop:
	// barrel hits, valid barrel hits, forward hits, valid forward hits,
	// top of detector hits, top of detector hits, strip hits
	for(const TrajectoryMeasurement& measurement: traj -> measurements())
	{
		// Check measurement validity
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
					++track_.strip;
				}
		}
		// Looking for pixel hits
		if(NtuplizerHelpers::detidIsOnPixel(detId)) continue;
		// Looking for valid and missing hits
		const int& validhit = hit -> getType() == TrackingRecHit::valid;
		// const int& missing  = hit -> getType() == TrackingRecHit::missing;
		// Module info
		ModuleData mod;
		getModuleData(mod,    0, detId, federrors);
		// Forward and barrel hits
		if(subDetId == PixelSubdetector::PixelBarrel)
		{
			track_.bpix[mod.layer - 1]++;
			if(validhit) track_.validbpix[mod.layer-1]++;
		}
		if(subDetId == PixelSubdetector::PixelEndcap)
		{
			track_.fpix[mod.disk - 1]++;
			if(validhit) track_.validfpix[mod.disk - 1]++;
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
			// Count op-of-detector tracks
			if(0 <= gly && gly != NOVAL_F)
			{
				track_.pixhit[0] += 1;
				if(validhit) track_.validpixhit[0] += 1;
			}
			// Count Bottom-of-detector tracks
			if(gly < 0 && gly != NOVAL_F)
			{
				track_.pixhit[1] += 1;
				if(validhit) track_.validpixhit[1] += 1;
			}
		}
	}
	trackTree_ -> Fill();	
}

void PhaseIPixelNtuplizer::getTrajTrackInfo(const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const std::map<uint32_t, int>& federrors)
{
	// Track and trajectory measurement loop
	// Looping on the whole track collection
	int trackIndex = 0;
	for(const auto& currentTrackKeypair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj  = currentTrackKeypair.key;
		const reco::TrackRef                    track = currentTrackKeypair.val;
		// Discarding tracks without pixel measurements
		if(!NtuplizerHelpers::trajectoryHasPixelHit(traj)) continue;
		getTrackInfo(vertexCollectionHandle, traj, track, federrors, trackIndex);
		trackIndex++;
		// Trajectory info
		traj_.init();
		for(const TrajectoryMeasurement& meas: traj -> measurements())
		{
			// Check measurement validity
			if(!meas.updatedState().isValid()) continue;
			TransientTrackingRecHit::ConstRecHitPointer recHit = meas.recHit();
			// Only looking for pixel hits
			DetId detId = recHit -> geographicalId();
			if(!NtuplizerHelpers::detidIsOnPixel(detId)) continue;

			// We could match recHits to simHits, here, using recipe:
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
			if(recHit -> isValid() && recHit -> hit() != 0)
			{
				const SiPixelRecHit *hit = (const SiPixelRecHit*)recHit -> hit();
				SiPixelRecHit::ClusterRef const& clust = hit -> cluster();
				if(clust.isNonnull())
				{
					traj_.clu.charge = clust -> charge() / 1000.0f;
					traj_.clu.size   = clust -> size();
					traj_.clu.edge   = hit -> isOnEdge() ? 1 : 0;
					traj_.clu.badpix = hit -> hasBadPixels() ? 1 : 0;
					traj_.clu.tworoc = hit -> spansTwoROCs() ? 1 : 0;
					traj_.clu.sizeX  = clust -> sizeX();
					traj_.clu.sizeY  = clust -> sizeY();
					traj_.clu.x      = clust -> x();
					traj_.clu.y      = clust -> y();
					for(int i = 0; i < clust -> size() && i < 1000; i++)
					{
						traj_.clu.adc[i]    = static_cast<float>(clust -> pixelADC()[i]) / 1000.0f;
						traj_.clu.pix[i][0] = ((clust -> pixels())[i]).x;
						traj_.clu.pix[i][1] = ((clust -> pixels())[i]).y;
					}
					traj_.norm_charge = traj_.clu.charge * sqrt(1.0f / (1.0f / pow(tan(traj_.alpha), 2) + 1.0f / pow(tan(traj_.beta), 2) + 1.0f));
					traj_.dx_cl       = std::abs(clust -> x() - traj_.lx);
					traj_.dy_cl       = std::abs(clust -> y() - traj_.ly);
					traj_.d_cl        = sqrt(traj_.dx_cl * traj_.dx_cl + traj_.dy_cl * traj_.dy_cl);
				}
			}

			// Get closest other traj measurement
			NtuplizerHelpers::getClosestOtherTrackDistanceByLooping(meas, trajTrackCollectionHandle, traj_.d_tr, traj_.dx_tr, traj_.dy_tr);
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
			trajTree_ -> Fill();
		}
	}
}

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
	reco::VertexCollection::const_iterator findClosestVertexToTrack(const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle)
	{
		reco::VertexCollection::const_iterator closestVtx = vertexCollectionHandle -> end();
		// FIXME: This is awkward, change the minDistance to the distance of the first valid vertex
		double minDistance = 9999;
		for(reco::VertexCollection::const_iterator it = vertexCollectionHandle -> begin() ; it != vertexCollectionHandle -> end() ; ++it)
		{
			// Filter out invalid vertices
			if(!it -> isValid()) continue;
			double trkVtxD0 = track -> dxy(it -> position()) * -1.0;
			double trkVtxDz = track -> dz (it -> position());
			// Comparing squareroots should be quick enough, if required, change this to a comparison of squares
			double trkVtxDB = sqrt(trkVtxD0 * trkVtxD0 + trkVtxDz * trkVtxDz);
			if(trkVtxDB < minDistance)
			{
				minDistance = trkVtxDB;
				closestVtx=it;
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
		distanceSquared = dxSquared * dySquared;
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
} // NtuplizerHelpers

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);
