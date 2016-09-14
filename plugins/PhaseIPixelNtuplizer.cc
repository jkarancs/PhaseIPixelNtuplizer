#include "PhaseIPixelNtuplizer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG


PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	// Options
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

	//////////////////////
	// Tool definitions //
	//////////////////////

	// Tracker for valid and missing hits
	edm::ESHandle<TrackerGeometry> tracker;
	iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
	if(!tracker.isValid())
	{
		handleDefaultError("Invalid tracker.", "tool_access", "Inaccessible or invalid tracker.");
	}

	// Tracker topology for module informations
	edm::ESHandle<TrackerTopology> trackerTopologyHandle;
	iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
	const TrackerTopology* const trackerTopology = trackerTopologyHandle.product();

	////////////////
	// Event tree //
	////////////////

	// Set data holder object
	// PhaseIDataTrees::setEventTreeDataFields(eventTree, eventField);
	// eventField.fill         = static_cast<int>(0); // FIXME
	// eventField.run          = static_cast<int>(iEvent.id().run());
	// eventField.ls           = static_cast<int>(iEvent.luminosityBlock());
	// eventField.orb          = static_cast<int>(iEvent.orbitNumber());
	// eventField.bx           = static_cast<int>(iEvent.bunchCrossing());
	// eventField.evt          = static_cast<int>(iEvent.id().event());
	// get_nvtx_and_vtx_data(iEvent);

	// Fill the tree
	// eventTree -> Fill();

	// Get vertices by token
	edm::Handle<reco::VertexCollection> vertexCollectionHandle;
	iEvent.getByToken(primaryVerticesToken, vertexCollectionHandle);

	// FED errors
	std::map<uint32_t, int> fedErrors = FedErrorFetcher::getFedErrors(iEvent, rawDataErrorToken);

	getClusters(iEvent, trackerTopology, fedErrors);
	getTrajMeasurements(iEvent, vertexCollectionHandle, tracker, trackerTopology, fedErrors);
	// Added for safety
	clearAllContainers();
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}
void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {}

/////////////////////////////
// Event tree calculations //
/////////////////////////////

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

////////////////
// Clust tree //
////////////////

void PhaseIPixelNtuplizer::getClusters(const edm::Event& iEvent, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	// Set data holder objects
	PhaseIDataTrees::setClusterTreeDataFields(clusterTree, eventField, clusterField);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
	iEvent.getByToken(clustersToken, clusterCollectionHandle);
	// Trying to access the clusters
	if(clusterCollectionHandle.isValid())
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
			if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
			{
				continue;
			}
			typedef edmNew::DetSet<SiPixelCluster>::const_iterator clustSetIt_t;
			for(clustSetIt_t currentClusterIt = currentClusterSet.begin(); currentClusterIt != currentClusterSet.end(); ++currentClusterIt)
			{
				const auto& currentCluster = *currentClusterIt;
				// Serial num of cluster in the given module
				clusterField.i = currentClusterIt - currentClusterSet.begin();
				// Set if there is a valid hits
				// clusterField.edge;
				// clusterField.badpix;
				// clusterField.tworoc;
				// Module information
				clusterField.mod    = ModuleDataProducer::getPhaseOneOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
				clusterField.mod_on = ModuleDataProducer::convertPhaseOneOfflineOnline(clusterField.mod);
				// Position and size
				clusterField.x     = currentCluster.x();
				clusterField.y     = currentCluster.y();
				clusterField.sizeX = currentCluster.sizeX();
				clusterField.sizeY = currentCluster.sizeY();
				clusterField.size  = currentCluster.size();
				// Charge
				clusterField.charge = currentCluster.charge();
				// Misc.
				for(int i = 0; i < clusterField.size && i < 1000; ++i)
				{
					const auto& currentPixels = currentCluster.pixels();
					clusterField.adc[i]    = currentCluster.pixelADC()[i] / 1000.0;
					clusterField.pix[i][0] = currentPixels[i].x;
					clusterField.pix[i][1] = currentPixels[i].y;
				}
				completeClusterCollection.push_back(clusterField);
				// The number of saved clusters can be downscaled to save space
				if(clusterCounter++ % clusterSaveDownlscaling != 0)
				{
					continue;
				}
				clusterTree -> Fill();
			}
		}
	}
	else
	{
		handleDefaultError("data_access", "data_access", "Failed to fetch the clusters.");
	}
}

void PhaseIPixelNtuplizer::getTrajMeasurements(const edm::Event& iEvent, const edm::Handle<reco::VertexCollection>& vertexCollectionHandle, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	PhaseIDataTrees::setTrajTreeDataFields(trajTree, eventField, trajField);
	// Fetching the tracks by token
	edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
	iEvent.getByToken(trajTrackCollectionToken, trajTrackCollectionHandle);
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
			// Fetch the hit
			const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit -> hit());
			// Check hit qualty
			if(!pixhit) continue;
			uint32_t subDetId = (detId.subdetId());
			// Looking for pixel hits
			if(TrajAnalyzer::subdetidIsOnPixel(subDetId)) continue;
			// Looking for valid and missing hits
			trajField.validhit = hit -> getType() == TrackingRecHit::valid;
			trajField.missing  = hit -> getType() == TrackingRecHit::missing;
			// Save module data
			trajField.mod    = ModuleDataProducer::getPhaseOneOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
			trajField.mod_on = ModuleDataProducer::convertPhaseOneOfflineOnline(trajField.mod);
			// Position measurements
			static TrajectoryStateCombiner trajStateComb; // operator () should be const, so this is fine as a static variable
			TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
			GlobalPoint globalPosition     = trajStateOnSurface.globalPosition();
			LocalPoint  localPosition      = trajStateOnSurface.localPosition();
			LocalError  localPositionError = trajStateOnSurface.localError().positionError();
			trajField.glx    = globalPosition.x();
			trajField.gly    = globalPosition.y();
			trajField.glz    = globalPosition.z();
			trajField.lx     = localPosition.x();
			trajField.ly     = localPosition.y();
			trajField.lz     = localPosition.z();
			trajField.lx_err = localPositionError.xx();
			trajField.ly_err = localPositionError.yy();
			// trajField.lz_err = localPositionError.zz();
			// trajField.onedge = std::abs(trajField.lx) < 0.55 && std::abs(trajField.ly) < 3.0;
			// Track local angles
			LocalTrajectoryParameters trajectoryParameters = trajStateOnSurface.localParameters();
			auto trajectoryMomentum = trajectoryParameters.momentum();
			LocalVector localTrackDirection = trajectoryMomentum / trajectoryMomentum.mag();
			trajField.alpha = atan2(localTrackDirection.z(), localTrackDirection.x());
			trajField.beta  = atan2(localTrackDirection.z(), localTrackDirection.y());
			// Get closest other traj measurement
			double closestTrajMeasurementDistanceSquared = TrajAnalyzer::trajMeasurementDistanceSquared(measurement, *(trajTrackCollectionHandle -> begin() -> key -> measurements().begin()));
			for(const auto& otherTrackKeypair: *trajTrackCollectionHandle)
			{
				const edm::Ref<std::vector<Trajectory>> otherTraj = otherTrackKeypair.key;
				for(const TrajectoryMeasurement& otherTrajMeasurement: otherTraj -> measurements())
				{
					float distanceSquared = TrajAnalyzer::trajMeasurementDistanceSquared(measurement, otherTrajMeasurement);
					if(distanceSquared < closestTrajMeasurementDistanceSquared)
					{
						closestTrajMeasurementDistanceSquared = distanceSquared;
					}
				}
			}
			double closestTrajMeasurementDistance = sqrt(closestTrajMeasurementDistanceSquared);
			// trajField.clust_near = (trajField.d_cl[0] != NOVAL_F && trajField.d_cl[0] < 0.05);
			trajField.hit_near = (closestTrajMeasurementDistance < 0.5);
			// Add hit efficiency cuts to the saved fields
			getHitEfficiencyCuts();
			// Filling the tree
			trajTree -> Fill();
		}
	}
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
	completeClusterCollection.clear();
	completeTrackCollection.clear();
	completeTrajMeasCollection.clear();
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

