#include "PhaseIPixelNtuplizer.h"

// #define EDM_ML_LOGDEBUG
// #define ML_DEBUG

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	// Options
	// Save only every nth cluster
	clusterSaveDownlscaling = 1;
	// Product consumption declarations
	primaryVerticesToken     = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
	clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
	// clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClustersPreSplitting"));
	// trajTrackCollectionToken = consumes<TrajTrackAssociationCollection>(edm::InputTag("trajectoryInput"));
	trajTrackCollectionToken = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
	rawDataErrorToken        = consumes<edm::DetSetVector<SiPixelRawDataError> >(edm::InputTag("siPixelDigis"));
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer() {}

void PhaseIPixelNtuplizer::beginJob()
{
	this -> iConfig = iConfig;
	if(iConfig.exists("fileName"))
	{
		ntupleOutputFilename = iConfig.getParameter<std::string>("filename");
	}
	// Create output file
	ntupleOutputFile = new TFile(ntupleOutputFilename.c_str(), "RECREATE");
	if(!(ntupleOutputFile -> IsOpen()))
	{
		handleDefaultError("file_operations", "file_operations", { "Failed to open output file: ", ntupleOutputFilename });
	}
	LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename << "\" created." << std::endl;
	// Branch definitions
	// Event tree branches
	// eventTree = new TTree("eventTree", "The event");
	// PhaseIDataTrees::defineEventTreeBranches(eventTree, eventField);
	// Cluster tree branches
	clusterTree = new TTree("clustTree", "Pixel clusters");
	PhaseIDataTrees::defineClusterTreeBranches(clusterTree, eventField, clusterField);
	// Traj tree branches
	trajTree = new TTree("trajTree", "Trajectory measurements in the Pixel");
	PhaseIDataTrees::defineTrajTreeBranches(trajTree, eventField, trajField);
}

void PhaseIPixelNtuplizer::endJob()
{
	LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename << "\"." << std::endl;
	ntupleOutputFile -> Write();
	LogDebug("file_operations") << "Closing file: \"" << ntupleOutputFilename << "\"." << std::endl;
	ntupleOutputFile -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << ntupleOutputFilename << "\"." << std::endl;
}

void PhaseIPixelNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;
	// Tracker for valid and missing hits
	edm::ESHandle<TrackerGeometry> tracker;
	iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
	if(!tracker.isValid()) handleDefaultError("Invalid tracker.", "tool_access", "Inaccessible or invalid tracker.");
	// Tracker topology for module informations
	edm::ESHandle<TrackerTopology> trackerTopologyHandle;
	iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
	const TrackerTopology* const trackerTopology = trackerTopologyHandle.product();

	///////////////////
	// Data fetching //
	///////////////////

	// FED errors
	std::map<uint32_t, int> fedErrors = FedErrorFetcher::getFedErrors(iEvent, rawDataErrorToken);
	// Fetching vertices by token
	edm::Handle<reco::VertexCollection>     vertexCollectionHandle;
	iEvent.getByToken(primaryVerticesToken, vertexCollectionHandle);
	// Fetching pixel digis by token
	// edm::Handle<edm::DetSetVector<PixelDigi>> digiCollectionHandle;
	// iEvent.getByToken(pixelDigisToken,        digiCollectionHandle);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollectionHandle;
	iEvent.getByToken(clustersToken,                  clusterCollectionHandle);
	// Fetching the tracks by token
	edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
	iEvent.getByToken(trajTrackCollectionToken, trajTrackCollectionHandle);
	// Check if the data is accessible
	if(!vertexCollectionHandle    .isValid()) handleDefaultError("data access", "data_access", "Failed to fetch primary vertex collection.");
	// if(!digiCollectionHandle      .isValid()) handleDefaultError("data access", "data_access", "Failed to fetch digi collection.");
	if(!clusterCollectionHandle   .isValid()) handleDefaultError("data access", "data_access", "Failed to fetch clusters.");
	if(!trajTrackCollectionHandle .isValid()) handleDefaultError("data access", "data_access", "Failed to fetch trajectory measurements.");

	///////////////////
	// Data handling //
	///////////////////

	// // Event tree
	// // Set data holder object
	// PhaseIDataTrees::setEventTreeDataFields(eventTree, eventField);
	// eventField.fill         = static_cast<int>(0); // FIXME
	// eventField.run          = static_cast<int>(iEvent.id().run());
	// eventField.ls           = static_cast<int>(iEvent.luminosityBlock());
	// eventField.orb          = static_cast<int>(iEvent.orbitNumber());
	// eventField.bx           = static_cast<int>(iEvent.bunchCrossing());
	// eventField.evt          = static_cast<int>(iEvent.id().event());
	// get_nvtx_and_vtx_data(iEvent);
	// // Fill the tree
	// eventTree -> Fill();
	std::vector<TrajClusterAssociationData> onTrackClusters;
	handleTrajMeasurements(trajTrackCollectionHandle, vertexCollectionHandle, tracker, trackerTopology, fedErrors, onTrackClusters);
	handleClusters(clusterCollectionHandle, trackerTopology, fedErrors, onTrackClusters);
	// Added for safety
	clearAllContainers();
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}

void PhaseIPixelNtuplizer::getNvtxAndVtxData(const edm::Event& iEvent)
{
	// edm::Handle<reco::VertexCollection> vertices;
	// iEvent.getByToken(primaryVerticesToken, vertices);
	// if(!vertices.isValid() || vertices -> size() == 0) return;

	// // // Get vertices by token
	// // edm::Handle<reco::VertexCollection> vertexCollectionHandle;
	// // iEvent.getByToken(primaryVerticesToken, vertexCollectionHandle);
	// // Loop on vertices
	// eventField.nvtx = 0;
	// const reco::VertexCollection& vertices = *vertexCollectionHandle.product();
	// reco::VertexCollection::const_iterator primary_vtx = vertices.end(); // Should be invalid
	// for(const auto& current_vertex: vertices)
	// {
	// 	// Invalid vertex
	// 	if(!current_vertex.isValid()) continue;
	// 	// Setting up some aliases
	// 	const int it_ntrk       = static_cast<int>(current_vertex.tracksSize());
	// 	const int& evt_ntrk     = eventField.vtxntrk;
	// 	// Vertex checks
	// 	bool ntrk_is_invalid       = evt_ntrk == NOVAL_I;
	// 	bool has_more_trks         = evt_ntrk < it_ntrk;
	// 	bool has_just_as_much_trks = evt_ntrk == it_ntrk;
	// 	bool nvtx_z_is_smaller     = std::abs(eventField.vtxZ) > std::abs(current_vertex.z());
	// 	// Check if it is the best vertex
	// 	if(ntrk_is_invalid || has_more_trks || (has_just_as_much_trks && nvtx_z_is_smaller))
	// 	{
	// 		eventField.vtxntrk = it_ntrk;
	// 		eventField.vtxD0   = current_vertex.position().rho();
	// 		eventField.vtxX    = current_vertex.x();
	// 		eventField.vtxY    = current_vertex.y();
	// 		eventField.vtxZ    = current_vertex.z();
	// 		eventField.vtxndof = current_vertex.ndof();
	// 		eventField.vtxchi2 = current_vertex.chi2();
	// 		primary_vtx         = static_cast<reco::VertexCollection::const_iterator>(&current_vertex);
	// 	}
	// 	// Counting the good vertices
	// 	if(std::abs(current_vertex.z()) <= 25.0 && std::abs(current_vertex.position().rho()) <= 2.0 && current_vertex.ndof() > 4)
	// 	{
	// 		eventField.nvtx++;
	// 	}
	// }
}

void PhaseIPixelNtuplizer::handleTrajMeasurements(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollectionHandle, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors, std::vector<TrajClusterAssociationData>& onTrackClusters) try
{
	if(!onTrackClusters.empty()) throw new std::runtime_error("onTrackClusters should be an empty container before filling in handleTrajectories().");
	// Looping on the whole track collection
	for(const auto& currentTrackKeypair: *trajTrackCollectionHandle)
	{
		const edm::Ref<std::vector<Trajectory>> traj = currentTrackKeypair.key;
		const reco::TrackRef track                   = currentTrackKeypair.val; // TrackRef is actually a pointer type
		// Discarding tracks without pixel measurements
		if(!TrajAnalyzer::trajectoryHasPixelHit(traj)) continue;
		// Track data
		getTrackData(traj, track, vertexCollectionHandle, tracker, trackerTopology, fedErrors);
		// Looping again to check hit efficiency of pixel hits
		for(const TrajectoryMeasurement& measurement: traj -> measurements())
		{
			// Check measurement validity
			if(!measurement.updatedState().isValid()) continue;
			auto hit = measurement.recHit();
			// Det id
			DetId detId = hit -> geographicalId();
			uint32_t subDetId = (detId.subdetId());
			// Looking for pixel hits
			if(TrajAnalyzer::subdetidIsOnPixel(subDetId)) continue;
			// Fetch the hit
			auto recHit = measurement.recHit();
			if(recHit -> hit() == nullptr) continue;
			const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(recHit -> hit());
			// Check hit qualty
			if(!recHit -> isValid()) continue;
			if(!pixhit)              continue;
			// Position measurements
			static TrajectoryStateCombiner trajStateComb; // operator () should be const, so this is fine as a static variable
			TrajectoryStateOnSurface trajStateOnSurface    = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
			GlobalPoint globalPosition                     = trajStateOnSurface.globalPosition();
			LocalPoint  localPosition                      = trajStateOnSurface.localPosition();
			LocalError  localPositionError                 = trajStateOnSurface.localError().positionError();
			LocalTrajectoryParameters trajectoryParameters = trajStateOnSurface.localParameters();
			auto trajectoryMomentum                        = trajectoryParameters.momentum();
			LocalVector localTrackDirection                = trajectoryMomentum / trajectoryMomentum.mag();
			SiPixelRecHit::ClusterRef const& clusterRef    = pixhit -> cluster();
			// Valid and missing hits
			trajField.validhit = recHit -> getType() == TrackingRecHit::valid;
			trajField.missing  = recHit -> getType() == TrackingRecHit::missing;
			// Save module data
			trajField.mod    = ModuleDataProducer::getPhaseOneOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
			trajField.mod_on = ModuleDataProducer::convertPhaseOneOfflineOnline(trajField.mod);
			// Other data
			trajField.glx    = globalPosition.x();
			trajField.gly    = globalPosition.y();
			trajField.glz    = globalPosition.z();
			trajField.lx     = localPosition.x();
			trajField.ly     = localPosition.y();
			trajField.lz     = localPosition.z();
			trajField.lx_err = localPositionError.xx();
			trajField.ly_err = localPositionError.yy();
			// Track local angles
			trajField.alpha = atan2(localTrackDirection.z(), localTrackDirection.x());
			trajField.beta  = atan2(localTrackDirection.z(), localTrackDirection.y());
			if(!clusterRef.isNonnull())
			{
				trajField.dx_cl = std::abs(clusterRef -> x() - trajField.lx);
				trajField.dy_cl = std::abs(clusterRef -> y() - trajField.ly);
			}
			if(trajField.dx_cl == NOVAL_F || trajField.dy_cl == NOVAL_F) trajField.d_cl = NOVAL_F;
			else 
			{
				trajField.d_cl = sqrt(trajField.dx_cl * trajField.dx_cl + trajField.dy_cl + trajField.dy_cl);
			}
			// Get closest other traj measurement
			TrajAnalyzer::getClosestOtherTrackDistanceByLooping(
				measurement, trajTrackCollectionHandle, 
				trajField.d_tr, trajField.dx_tr, trajField.dy_tr);
			// trajField.lz_err = localPositionError.zz();
			// trajField.onedge = std::abs(trajField.lx) < 0.55 && std::abs(trajField.ly) < 3.0;
			// Get closest cluster
			trajField.hit_near   = (trajField.d_tr < 0.5);
			trajField.clust_near = (trajField.d_cl != NOVAL_F && trajField.d_cl < 0.05);
			// Add hit efficiency cuts to the saved fields
			getHitEfficiencyCuts();
			// Filling the tree
			trajTree -> Fill();
			// Save data
			if(!clusterRef.isNonnull()) continue;
			onTrackClusters.emplace_back(clusterRef, trajField.alpha, trajField.beta);
		}
	}
}
catch(const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }

void PhaseIPixelNtuplizer::handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors, const std::vector<TrajClusterAssociationData>& onTrackClusters)
{
	int clusterCounter = 0;
	const edmNew::DetSetVector<SiPixelCluster>& currentClusterCollection = *clusterCollectionHandle;
	// Looping on clusters with the same location
	typedef edmNew::DetSetVector<SiPixelCluster>::const_iterator clustCollIt_t;
	for(clustCollIt_t currentClusterSetIt = currentClusterCollection.begin(); currentClusterSetIt != currentClusterCollection.end(); ++currentClusterSetIt)
	{
		const auto& currentClusterSet = *currentClusterSetIt;
		DetId detId(currentClusterSet.id());
		unsigned int subdetId = detId.subdetId();
		// Take only pixel clusters
		if(!TrajAnalyzer::subdetidIsOnPixel(subdetId)) continue;
		typedef edmNew::DetSet<SiPixelCluster>::const_iterator clustSetIt_t;
		for(clustSetIt_t currentClusterIt = currentClusterSet.begin(); currentClusterIt != currentClusterSet.end(); ++currentClusterIt)
		{
			// Only save every nth cluster
			// The number of saved clusters can be downscaled to save space
			if(clusterCounter++ % clusterSaveDownlscaling != 0) continue;
			const auto& currentCluster = *currentClusterIt;
			ModuleData mod    = ModuleDataProducer::getPhaseZeroOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
			ModuleData mod_on = ModuleDataProducer::convertPhaseZeroOfflineOnline(mod);
			saveClusterData(currentCluster, mod, mod_on, onTrackClusters);
		}
	}
}

void PhaseIPixelNtuplizer::saveClusterData(const SiPixelCluster& cluster, const ModuleData& mod, const ModuleData& mod_on, const std::vector<TrajClusterAssociationData>& onTrackClusters)
{
	// FIXME: change this to a global cluster counting
	static int clusterIndex = 0;
	clusterField.init();
	clusterField.i = ++clusterIndex;
	// Set if there is a valid hits
	// clusterField.edge;
	// clusterField.badpix;
	// clusterField.tworoc;
	// Module information
	clusterField.mod          = mod;
	clusterField.mod_on       = mod_on;
	// Position and size
	clusterField.x     = cluster.x();
	clusterField.y     = cluster.y();
	clusterField.sizeX = cluster.sizeX();
	clusterField.sizeY = cluster.sizeY();
	clusterField.size  = cluster.size();
	// Charge
	clusterField.charge = cluster.charge();
	// Misc.
	for(int i = 0; i < clusterField.size && i < 1000; ++i)
	{
		const auto& pixels = cluster.pixels();
		clusterField.adc[i]    = cluster.pixelADC()[i] / 1000.0;
		clusterField.pix[i][0] = pixels[i].x;
		clusterField.pix[i][1] = pixels[i].y;
	}
	// Loop on trajectories to see if this is an on-hit cluster
	auto searchResult = std::find_if(onTrackClusters.begin(), onTrackClusters.end(), [&cluster] (const TrajClusterAssociationData& toCheck) {return toCheck.clusterRef.get() == &cluster;});
	if(searchResult != onTrackClusters.end())
	{
		// clusterField.isOnHit = 1;
		// clusterField.alpha   = searchResult -> alpha;
		// clusterField.beta    = searchResult -> beta;
	}
	clusterTree -> Fill();
}

void PhaseIPixelNtuplizer::getTrackData(const edm::Ref<std::vector<Trajectory>>& traj, const reco::TrackRef& track, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	static int trackIndex = 0;
	TrackData& trackField = trajField.trk;
	trackField.init();
	// Finding the closest vertex to the track
	reco::VertexCollection::const_iterator closestVtx = TrajAnalyzer::findClosestVertexToTrack(track, vertexCollectionHandle);
	// Basic track quantities
	// FIXME: Add global track counting
	trackField.i       = trackIndex++;
	trackField.quality = track -> qualityMask();
	trackField.pt      = track -> pt();
	trackField.p       = track -> p();
	trackField.eta     = track -> eta();
	trackField.theta   = track -> theta();
	trackField.phi     = track -> phi();
	// FIXME: use best vertex selection instead of closest vertex selection
	trackField.d0      = track -> dxy(closestVtx -> position()) * -1.0;
	trackField.dz      = track -> dz (closestVtx -> position());
	// Zeroing counters
	trackField.validfpix[0]   = 0;
	trackField.validfpix[1]   = 0;
	trackField.validbpix[0]   = 0;
	trackField.validbpix[1]   = 0;
	trackField.validbpix[2]   = 0;
	trackField.validbpix[3]   = 0;
	trackField.pixhit[0]      = 0;
	trackField.pixhit[1]      = 0;
	trackField.validpixhit[0] = 0;
	trackField.validpixhit[1] = 0;
	trackField.bpix[0]        = 0;
	trackField.bpix[1]        = 0;
	trackField.bpix[2]        = 0;
	trackField.bpix[3]        = 0;
	trackField.fpix[0]        = 0;
	trackField.fpix[1]        = 0;
	trackField.strip          = 0;
	// Counted in this loop: 
	// barrel hits, valid barrel hits, forward hits, valid forward hits,
	// top of detector hits, top of detector hits, strip hits
	for(const TrajectoryMeasurement& measurement: traj -> measurements())
	{
		// Check measurement validity
		if(!measurement.updatedState().isValid()) continue;
		auto hit = measurement.recHit();
		// Det id
		DetId detId = hit -> geographicalId();
		uint32_t subDetId = (detId.subdetId());
		// Counting the strip hits
		if(hit -> isValid())
		{
			if(subDetId == StripSubdetector::TIB) ++trackField.strip;
			if(subDetId == StripSubdetector::TOB) ++trackField.strip;
			if(subDetId == StripSubdetector::TID) ++trackField.strip;
			if(subDetId == StripSubdetector::TEC) ++trackField.strip;
		}
		// Looking for pixel hits
		if(TrajAnalyzer::subdetidIsOnPixel(subDetId)) continue;
		// Looking for valid and missing hits
		const int& validhit = hit -> getType() == TrackingRecHit::valid;
		// const int& missing  = hit -> getType() == TrackingRecHit::missing;
		// Module info
		ModuleData mod = ModuleDataProducer::getPhaseOneOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
		// Forward and barrel hits
		if(subDetId == PixelSubdetector::PixelBarrel)
		{
			trackField.bpix[mod.layer - 1]++;
			if(validhit) trackField.validbpix[mod.layer-1]++;
		}
		if(subDetId == PixelSubdetector::PixelEndcap)
		{
			trackField.fpix[mod.disk - 1]++;
			if(validhit) trackField.validfpix[mod.disk - 1]++;
		}
		// Fetch the hit
		const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit -> hit());
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
				trackField.pixhit[0] += 1;
				if(validhit) trackField.validpixhit[0] += 1;
			}
			// Count Bottom-of-detector tracks
			if(gly < 0 && gly != NOVAL_F)
			{
				trackField.pixhit[1] += 1;
				if(validhit) trackField.validpixhit[1] += 1;
			}
		}
	}
}

void PhaseIPixelNtuplizer::getHitEfficiencyCuts()
{
	// Moved to src/PhaseI_tracking_efficiency_filters.cc
	PhaseITrackingEfficiencyFilters filterObject(eventField, trajField);
	int cutResults = filterObject.performCuts(
		PhaseITrackingEfficiencyFilters::Cuts::nvtx     |
		// PhaseITrackingEfficiencyFilters::Cuts::zerobias |
		PhaseITrackingEfficiencyFilters::Cuts::federr   |
		PhaseITrackingEfficiencyFilters::Cuts::hp       |
		PhaseITrackingEfficiencyFilters::Cuts::pt       |
		PhaseITrackingEfficiencyFilters::Cuts::nstrip   |
		PhaseITrackingEfficiencyFilters::Cuts::d0       |
		PhaseITrackingEfficiencyFilters::Cuts::dz       |
		// PhaseITrackingEfficiencyFilters::Cuts::pixhit   |
		// PhaseITrackingEfficiencyFilters::Cuts::lx_fid   |
		// PhaseITrackingEfficiencyFilters::Cuts::ly_fid   |
		PhaseITrackingEfficiencyFilters::Cuts::valmis   |
		PhaseITrackingEfficiencyFilters::Cuts::hitsep
	);
	trajField.pass_effcuts = cutResults;
}

/////////////
// Utility //
/////////////

void PhaseIPixelNtuplizer::clearAllContainers()
{
}

////////////////////
// Error handling //
////////////////////

void PhaseIPixelNtuplizer::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::string msg)
{
	edm::LogError(streamType.c_str()) << c_red << msg << c_def << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::handleDefaultError(const std::string& exception_type, const std::string& streamType, std::vector<std::string> msg)
{
	edm::LogError(streamType.c_str()) << c_red;
	for(const auto& msg_part: msg)
	{
		edm::LogError(streamType.c_str()) << msg_part;
	}
	edm::LogError(streamType.c_str()) << c_def << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::printEvtInfo(const std::string& streamType)
{
	edm::LogError(streamType.c_str()) << c_blue <<
		"Run: " << eventField.run << 
		" Ls: " << eventField.ls  << 
		" Evt:" << eventField.evt << c_def << std::endl;
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);

