#include "PhaseIPixelNtuplizer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG


PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	LogDebug("step") << "PhaseIPixelNtuplizer() constructor called." << std::endl;

	/////////////
	// Options //
	/////////////

	cluster_save_downlscaling = 1;

	//////////////////////////////////////
	// Product consumption declarations //
	//////////////////////////////////////

	// FIXME: add reco for PhaseI vertices
	primary_vertices_token      = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
	clusters_token              = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
	// clusters_token              = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClustersPreSplitting"));
	// traj_track_collection_token = consumes<TrajTrackAssociationCollection>(edm::InputTag("trajectoryInput"));
	traj_track_collection_token = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
	raw_data_error_token        = consumes<edm::DetSetVector<SiPixelRawDataError> >(edm::InputTag("siPixelDigis"));
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer()
{
	LogDebug("step") << "~PhaseIPixelNtuplizer()" << std::endl;
}

void PhaseIPixelNtuplizer::beginJob()
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginJob()..." << std::endl;
	this -> iConfig = iConfig;

	if(iConfig.exists("fileName"))
	{
		ntuple_output_filename = iConfig.getParameter<std::string>("filename");
	}

	////////////////////////
	// Create output file //
	////////////////////////

	ntuple_output_file = new TFile(ntuple_output_filename.c_str(), "RECREATE");
	
	if(!(ntuple_output_file -> IsOpen()))
	{
		handle_default_error("file_operations", "file_operations", { "Failed to open output file: ", ntuple_output_filename });
	}

	LogDebug("file_operations") << "Output file: \"" << ntuple_output_filename << "\" created." << std::endl;

	////////////////////////
	// Branch definitions //
	////////////////////////

	// Event tree branches
	// event_tree = new TTree("eventTree", "The event");
	// PhaseIDataTrees::define_event_tree_branches(event_tree, event_field);
	// Cluster tree branches
	cluster_tree = new TTree("clustTree", "Pixel clusters");
	PhaseIDataTrees::define_cluster_tree_branches(cluster_tree, event_field, cluster_field);
	// Traj tree branches
	traj_tree = new TTree("trajTree", "Trajectory measurements in the Pixel");
	PhaseIDataTrees::define_traj_tree_branches(traj_tree, event_field, traj_field);
}

void PhaseIPixelNtuplizer::endJob()
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endJob()..." << std::endl;
	LogDebug("file_operations") << "Writing plots to file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Write();
	LogDebug("file_operations") << "Closing file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << ntuple_output_filename << "\"." << std::endl;
	// if(event_tree)   delete event_tree;
	// if(cluster_tree) delete cluster_tree;
	// if(traj_tree)    delete traj_tree;
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
		handle_default_error("Invalid tracker.", "tool_access", "Inaccessible or invalid tracker.");
	}

	// Tracker topology for module informations
	edm::ESHandle<TrackerTopology> tracker_topology_handle;
	iSetup.get<TrackerTopologyRcd>().get(tracker_topology_handle);
	const TrackerTopology* const tracker_topology = tracker_topology_handle.product();

	////////////////
	// Event tree //
	////////////////

	// Set data holder object
	// PhaseIDataTrees::set_event_tree_data_fields(event_tree, event_field);
	// event_field.fill         = static_cast<int>(0); // FIXME
	// event_field.run          = static_cast<int>(iEvent.id().run());
	// event_field.ls           = static_cast<int>(iEvent.luminosityBlock());
	// event_field.orb          = static_cast<int>(iEvent.orbitNumber());
	// event_field.bx           = static_cast<int>(iEvent.bunchCrossing());
	// event_field.evt          = static_cast<int>(iEvent.id().event());
	// get_nvtx_and_vtx_data(iEvent);

	// Fill the tree
	// event_tree -> Fill();

	// FED errors
	std::map<uint32_t, int> federrors = get_FED_errors(iEvent);

	//////////////////
	// Cluster tree //
	//////////////////

	get_clusters(iEvent, tracker_topology, federrors);

	///////////////
	// Traj tree //
	///////////////

	get_traj_measurements(iEvent, tracker, tracker_topology, federrors);

	/////////////////////////
	// For testing purpose //
	/////////////////////////

	clear_all_containers();

	// produce_fake_events(1);
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginRun()..." << std::endl;
}

void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endRun()..." << std::endl;
}

void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginLuminosityBlock()..." << std::endl;
}

void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endLuminosityBlock()..." << std::endl;
}

/////////////////////////////
// Event tree calculations //
/////////////////////////////

void PhaseIPixelNtuplizer::get_nvtx_and_vtx_data(const edm::Event& iEvent)
{
	// edm::Handle<reco::VertexCollection> vertices;
	// iEvent.getByToken(primary_vertices_token, vertices);
	// if(!vertices.isValid() || vertices -> size() == 0) return;

	// // // Get vertices by token
	// // edm::Handle<reco::VertexCollection> vertexCollectionHandle;
	// // iEvent.getByToken(primary_vertices_token, vertexCollectionHandle);
	// // Loop on vertices
	// event_field.nvtx = 0;
	// const reco::VertexCollection& vertices = *vertexCollectionHandle.product();
	// reco::VertexCollection::const_iterator primary_vtx = vertices.end(); // Should be invalid
	// for(const auto& current_vertex: vertices)
	// {
	// 	// Invalid vertex
	// 	if(!current_vertex.isValid()) continue;
	// 	// Setting up some aliases
	// 	const int it_ntrk       = static_cast<int>(current_vertex.tracksSize());
	// 	const int& evt_ntrk     = event_field.vtxntrk;
	// 	// Vertex checks
	// 	bool ntrk_is_invalid       = evt_ntrk == NOVAL_I;
	// 	bool has_more_trks         = evt_ntrk < it_ntrk;
	// 	bool has_just_as_much_trks = evt_ntrk == it_ntrk;
	// 	bool nvtx_z_is_smaller     = std::abs(event_field.vtxZ) > std::abs(current_vertex.z());
	// 	// Check if it is the best vertex
	// 	if(ntrk_is_invalid || has_more_trks || (has_just_as_much_trks && nvtx_z_is_smaller))
	// 	{
	// 		event_field.vtxntrk = it_ntrk;
	// 		event_field.vtxD0   = current_vertex.position().rho();
	// 		event_field.vtxX    = current_vertex.x();
	// 		event_field.vtxY    = current_vertex.y();
	// 		event_field.vtxZ    = current_vertex.z();
	// 		event_field.vtxndof = current_vertex.ndof();
	// 		event_field.vtxchi2 = current_vertex.chi2();
	// 		primary_vtx         = static_cast<reco::VertexCollection::const_iterator>(&current_vertex);
	// 	}
	// 	// Counting the good vertices
	// 	if(std::abs(current_vertex.z()) <= 25.0 && std::abs(current_vertex.position().rho()) <= 2.0 && current_vertex.ndof() > 4)
	// 	{
	// 		event_field.nvtx++;
	// 	}
	// }
}

////////////////////
// FED error info //
////////////////////

std::map<uint32_t, int> PhaseIPixelNtuplizer::get_FED_errors(const edm::Event& iEvent)
{
	std::map<uint32_t, int> federrors;
	edm::Handle<edm::DetSetVector<SiPixelRawDataError>> siPixelRawDataErrorCollectionHandle;
	iEvent.getByToken(raw_data_error_token, siPixelRawDataErrorCollectionHandle);

	if(siPixelRawDataErrorCollectionHandle.isValid())
	{
		for(const auto& pixel_error_set: *siPixelRawDataErrorCollectionHandle)
		{
			for(const auto& pixel_error: pixel_error_set)
			{
				if(pixel_error_set.detId()!=0xffffffff)
				{
					DetId detId(pixel_error_set.detId());
					int type = pixel_error.getType();
					federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
					// if(type>24&&type<=40) federr[type-25]++;
					// else edm::LogError("data_access") << "New FED error with not recognised error-type: " << type << std::endl;
				}
			}
		}
		// for(int i=0; i<16; i++)
		// {
		// 	if(federr[i]!=0)
		// 	{
		// 		event_field.federrs[event_field.federrs_size][0] = federr[i];
		// 		event_field.federrs[event_field.federrs_size][1] = i + 25;
		// 		event_field.federrs_size++;
		// 	}
		// }
	}
	return federrors;
}

/////////////////////////
// Module informations //
/////////////////////////

ModuleData PhaseIPixelNtuplizer::get_offline_module_data(const uint32_t& rawId, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors)
{
	ModuleData mod;
	mod.rawid = rawId;
	int subdetId = DetId(mod.rawid).subdetId();
	if(subdetId == PixelSubdetector::PixelBarrel)
	{
		mod.det    = 0;
		mod.layer  = tracker_topology -> pxbLayer(rawId);
		mod.ladder = tracker_topology -> pxbLadder(rawId);
		mod.module = tracker_topology -> pxbModule(rawId);
	}
	if(subdetId == PixelSubdetector::PixelEndcap)
	{
		mod.det    = 1;
		mod.side   = tracker_topology -> pxfSide(rawId);
		mod.disk   = tracker_topology -> pxfDisk(rawId);   // Increasing abs(z): 1-3
		mod.blade  = tracker_topology -> pxfBlade(rawId);  // Inner blades: 1-22, outer blades: 23-56
		mod.panel  = tracker_topology -> pxfPanel(rawId);  // Forward: 1; Backward 2
		mod.module = tracker_topology -> pxfModule(rawId); // Always 1
		mod.ring   = 1 + (22 < mod.blade);                 // Inner: 1, Outer: 2
	}
	// FED error
	mod.federr = 0;
	auto found_error = federrors.find(mod.rawid);
	if(found_error != federrors.end())
	{
		mod.federr = found_error -> second;
	}
	return mod;
}

ModuleData PhaseIPixelNtuplizer::convert_offline_to_online_module_data(const ModuleData mod_off)
{
	ModuleData mod_on;
	mod_on.rawid  = mod_off.rawid;
	mod_on.det    = mod_off.det;
	if(mod_off.det == 0)
	{
		mod_on.layer  = mod_off.layer;
		mod_on.module = convert_offline_module_to_online_module_coordinate(mod_off.module);
		mod_on.ladder = convert_offline_ladder_to_online_ladder_coordinate(mod_off.layer, mod_off.ladder);
	}
	if(mod_off.det == 1)
	{
		mod_on.side   = mod_off.side;
		mod_on.disk   = convert_offline_disk_to_online_disk_coordinate(mod_off.side, mod_off.disk);    // Increasing abs(z): 1-3
		mod_on.blade  = convert_offline_blade_to_online_blade_coordinate(mod_off.blade); // Inner blades: (-11)-(-1), 1-11, outer blades: (-17)-(1), 1-17
		mod_on.panel  = mod_off.panel;  // Forward: 1; Backward 2
		mod_on.module = mod_off.module; // Always 1
		mod_on.ring   = mod_off.ring;   // Inner: 1, Outer: 2
	}
	mod_on.federr = mod_off.federr;
	return mod_on;
}

int PhaseIPixelNtuplizer::convert_offline_module_to_online_module_coordinate(const int& module)
{
	int converted = NOVAL_I;
	if(0 < module && module < 5) converted = module - 5;
	if(4 < module && module < 9) converted = module - 4;
	return converted;
}

int PhaseIPixelNtuplizer::convert_offline_ladder_to_online_ladder_coordinate(const int& layer, const int& ladder)
{
	static const std::vector<int> layer_quarter_sizes = {NOVAL_I, 3, 7, 11, 16};
	const int& quarter_size = layer_quarter_sizes[layer];
	int converted = NOVAL_I;
	if(0                < ladder && ladder <= quarter_size)     converted = quarter_size - ladder + 1;
	if(quarter_size     < ladder && ladder <= quarter_size * 3) converted = quarter_size - ladder;
	if(quarter_size * 3 < ladder && ladder <= quarter_size * 4) converted = quarter_size * 5 - ladder;
	return converted;
}

int PhaseIPixelNtuplizer::convert_offline_disk_to_online_disk_coordinate(const int& side, const int& disk)
{
	int converted = NOVAL_I;
	if(side == 2) converted =  disk;
	if(side == 1) converted = -disk;
	return converted;
}

int PhaseIPixelNtuplizer::convert_offline_blade_to_online_blade_coordinate(const int& blade)
{
	int converted = NOVAL_I;
	if(1  <= blade && blade < 7)  converted = 7  - blade; // 6 on 1st quarter
	if(7  <= blade && blade < 18) converted = 6  - blade; // 11 on 2nd half
	if(18 <= blade && blade < 23) converted = 29 - blade; // 5 on 4th quarter
	if(23 <= blade && blade < 32) converted = 32 - blade; // 9 on 1st quarter
	if(32 <= blade && blade < 49) converted = 31 - blade; // 17 on 2nd half
	if(49 <= blade && blade < 57) converted = 66 - blade; // 8 on 4th quarter
	return converted;
}

////////////////
// Clust tree //
////////////////

void PhaseIPixelNtuplizer::get_clusters(const edm::Event& iEvent, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors)
{
	// Set data holder objects
	PhaseIDataTrees::set_cluster_tree_data_fields(cluster_tree, event_field, cluster_field);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster> > cluster_collection_handle;
	iEvent.getByToken(clusters_token, cluster_collection_handle);
	// Trying to access the clusters
	if(cluster_collection_handle.isValid())
	{
		int cluster_counter = 0;
		const edmNew::DetSetVector<SiPixelCluster>& current_cluster_collection = *cluster_collection_handle;
		// Looping on clusters with the same location
		typedef edmNew::DetSetVector<SiPixelCluster>::const_iterator clust_coll_it_t;
		for(clust_coll_it_t current_cluster_set_it = current_cluster_collection.begin(); current_cluster_set_it != current_cluster_collection.end(); ++current_cluster_set_it)
		{
			const auto& current_cluster_set = *current_cluster_set_it;
			DetId det_id(current_cluster_set.id());
			unsigned int subdetId = det_id.subdetId();
			// Take only pixel clusters
			if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
			{
				continue;
			}
			typedef edmNew::DetSet<SiPixelCluster>::const_iterator clust_set_it_t;
			for(clust_set_it_t current_cluster_it = current_cluster_set.begin(); current_cluster_it != current_cluster_set.end(); ++current_cluster_it)
			{
				const auto& current_cluster = *current_cluster_it;
				// Serial num of cluster in the given module
				cluster_field.i = current_cluster_it - current_cluster_set.begin();
				// Set if there is a valid hits
				// cluster_field.edge;
				// cluster_field.badpix;
				// cluster_field.tworoc;
				// Module information
				cluster_field.mod    = get_offline_module_data(det_id.rawId(), tracker_topology, federrors);
				cluster_field.mod_on = convert_offline_to_online_module_data(cluster_field.mod);
				// Position and size
				cluster_field.x     = current_cluster.x();
				cluster_field.y     = current_cluster.y();
				cluster_field.sizeX = current_cluster.sizeX();
				cluster_field.sizeY = current_cluster.sizeY();
				cluster_field.size  = current_cluster.size();
				// Charge
				cluster_field.charge = current_cluster.charge();
				// Misc.
				for(int i = 0; i < cluster_field.size && i < 1000; ++i)
				{
					const auto& current_pixels = current_cluster.pixels();
					cluster_field.adc[i]    = current_cluster.pixelADC()[i] / 1000.0;
					cluster_field.pix[i][0] = current_pixels[i].x;
					cluster_field.pix[i][1] = current_pixels[i].y;
				}
				complete_cluster_collection.push_back(cluster_field);
				// The number of saved clusters can be downscaled to save space
				if(cluster_counter++ % cluster_save_downlscaling != 0)
				{
					continue;
				}
				cluster_tree -> Fill();
			}
		}
	}
	else
	{
		handle_default_error("data_access", "data_access", "Failed to fetch the clusters.");
	}
}


///////////////
// Traj tree //
///////////////

int PhaseIPixelNtuplizer::trajectory_has_pixel_hit(const edm::Ref<std::vector<Trajectory>>& trajectory)
{
	bool is_barrel_pixel_track = false;
	bool is_endcap_pixel_track = false;
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
		if(subdetid == PixelSubdetector::PixelBarrel) is_barrel_pixel_track = true;
		if(subdetid == PixelSubdetector::PixelEndcap) is_endcap_pixel_track = true;
	}
	if(!is_barrel_pixel_track && !is_endcap_pixel_track)
	{
		return 0;
	}
	return 1;
}

void PhaseIPixelNtuplizer::get_traj_measurements(const edm::Event& iEvent, const edm::ESHandle<TrackerGeometry>& tracker, const TrackerTopology* const tracker_topology, const std::map<uint32_t, int>& federrors)
{
	PhaseIDataTrees::set_traj_tree_data_fields(traj_tree, event_field, traj_field);
	// Fetching the tracks by token
	edm::Handle<TrajTrackAssociationCollection> traj_track_collection_handle;
	iEvent.getByToken(traj_track_collection_token, traj_track_collection_handle);
	// Looping on the whole track collection
	for(const auto& current_track_keypair: *traj_track_collection_handle)
	{
		const edm::Ref<std::vector<Trajectory>> traj = current_track_keypair.key;
		const reco::TrackRef track                   = current_track_keypair.val; // TrackRef is actually a pointer type
		// Discarding tracks without pixel measurements
		if(!trajectory_has_pixel_hit(traj)) continue;
		// Looping again to check hit efficiency of pixel hits
		for(auto& measurement: traj -> measurements())
		{
			// Check measurement validity
			if(!measurement.updatedState().isValid()) continue;
			auto hit = measurement.recHit();
			// Det id
			DetId det_id = hit -> geographicalId();
			uint32_t subdetid = (det_id.subdetId());
			// Looking for pixel hits
			bool is_pixel_hit = false;
			is_pixel_hit |= subdetid == PixelSubdetector::PixelBarrel;
			is_pixel_hit |= subdetid == PixelSubdetector::PixelEndcap;
			if(!is_pixel_hit) continue;
			// Looking for valid and missing hits
			traj_field.validhit = hit -> getType() == TrackingRecHit::valid;
			traj_field.missing  = hit -> getType() == TrackingRecHit::missing;
			// Fetch the hit
			const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit -> hit());
			// Check hit qualty
			if(pixhit)
			{
				// Save module data
				traj_field.mod    = get_offline_module_data(det_id.rawId(), tracker_topology, federrors);
				traj_field.mod_on = convert_offline_to_online_module_data(traj_field.mod);
				// Position measurements
				TrajectoryStateCombiner  trajStateComb;
				TrajectoryStateOnSurface traj_state_on_surface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
				auto global_position      = traj_state_on_surface.globalPosition();
				auto local_position       = traj_state_on_surface.localPosition();
				auto local_position_error = traj_state_on_surface.localError().positionError();
				traj_field.glx    = global_position.x();
				traj_field.gly    = global_position.y();
				traj_field.glz    = global_position.z();
				traj_field.lx     = local_position.x();
				traj_field.ly     = local_position.y();
				traj_field.lz     = local_position.z();
				traj_field.lx_err = local_position_error.xx();
				traj_field.ly_err = local_position_error.yy();
				traj_field.onedge = std::abs(traj_field.lx) < 0.55 && std::abs(traj_field.ly) < 3.0;
				// // Top-of-detector tracks
				// if(0 <= traj_field.gly && traj_field.gly != NOVAL_F)
				// {
				// 	traj_field.trk.pixhit[0] += 1;
				// 	if(traj_field.validhit) traj_field.trk.validpixhit[0] += 1;
				// }
				// // Bottom-of-detector tracks
				// if(traj_field.gly < 0 && traj_field.gly != NOVAL_F)
				// {
				// 	traj_field.trk.pixhit[1] += 1;
				// 	if(traj_field.validhit) traj_field.trk.validpixhit[1] += 1;
				// }
				// Track local angles
				LocalTrajectoryParameters trajectory_parameters = traj_state_on_surface.localParameters();
				auto trajectory_momentum = trajectory_parameters.momentum();
				LocalVector local_track_direction = trajectory_momentum / trajectory_momentum.mag();
				traj_field.alpha = atan2(local_track_direction.z(), local_track_direction.x());
				traj_field.beta  = atan2(local_track_direction.z(), local_track_direction.y());
				// Filling the tree
				traj_tree -> Fill();
			}
		}
	}
}

// void PhaseIPixelNtuplizer::get_track_data()
// {
// 	static int track_index = 0;
	
// 	TrackData& track_field = traj_field.trk;
// 	// FIXME: Add global track counting
// 	track_field.i   = track_index++;
// 	// Top and bottom hits
// 	track_field.pixhit[0] = 0;
// 	track_field.pixhit[1] = 0;
// }

void PhaseIPixelNtuplizer::get_hit_efficiency_cuts()
{
	// Moved to src/PhaseI_tracking_efficiency_filters.cc
	PhaseI_tracking_efficiency_filters filter_object(event_field, traj_field);
	int cut_results = filter_object.perform_cuts(
		PhaseI_tracking_efficiency_filters::Cuts::nvtx   |
		PhaseI_tracking_efficiency_filters::Cuts::federr |
		PhaseI_tracking_efficiency_filters::Cuts::nstrip
	);
	traj_field.pass_effcuts = cut_results;
}

/////////////
// Utility //
/////////////

void PhaseIPixelNtuplizer::clear_all_containers()
{
	complete_cluster_collection.clear();
	complete_track_collection.clear();
	complete_traj_meas_collection.clear();
}

////////////////////
// Error handling //
////////////////////

void PhaseIPixelNtuplizer::handle_default_error(const std::string& exception_type, const std::string& stream_type, std::string msg)
{
	edm::LogError(stream_type.c_str()) << c_red << msg << c_def << std::endl;
	print_evt_info(stream_type);
	throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::handle_default_error(const std::string& exception_type, const std::string& stream_type, std::vector<std::string> msg)
{
	edm::LogError(stream_type.c_str()) << c_red;
	for(const auto& msg_part: msg)
	{
		edm::LogError(stream_type.c_str()) << msg_part;
	}
	edm::LogError(stream_type.c_str()) << c_def << std::endl;
	print_evt_info(stream_type);
	throw cms::Exception(exception_type.c_str());
}

void PhaseIPixelNtuplizer::print_evt_info(const std::string& stream_type)
{
	edm::LogError(stream_type.c_str()) << c_blue <<
		"Run: " << event_field.run << 
		" Ls: " << event_field.ls  << 
		" Evt:" << event_field.evt << c_def << std::endl;
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);

// edm::LogError("category")    << "error    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogProblem("category")  << "problem  PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogWarning ("category") << "warning  PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogPrint("category")    << "print    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogInfo("category")     << "infor    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogVerbatim("category") << "verbatim PhaseIPixelNtuplizer() constructor called." << std::endl;

// // TODO: define fields
// // event_field.trig         = get_trig();
// // event_field.l1_rate      = get_l1_rate();
// // event_field.intlumi      = static_cast<float>(lumi -> intgRecLumi());
// // event_field.instlumi     = static_cast<float>(lumi -> avgInsDelLumi());
// // event_field.instlumi_ext = get_instlumi_ext();
// // event_field.pileup       = get_pileup();
// // event_field.weight       = get_weight();
// // event_field.good         = get_good();
// // event_field.tmuon        = get_tmuon();
// // event_field.tmuon_err    = get_tmuon_err();
// // event_field.tecal        = get_tecal();
// // event_field.tecal_raw    = get_tecal_raw();
// // event_field.tecal_err    = get_tecal_err();
// // event_field.field        = get_field();
// // event_field.wbc          = get_wbc();
// // event_field.delay        = get_delay();
// // event_field.ntracks      = ...;
// for(int i = 0; i < 2; ++i)
// {
// 	// event_field.beamint      = [2];
// }
// for(int i = 0; i < 4; ++i)
// {
// 	// event_field.nclu[i] = get_nclu(); // [0: fpix, i: layer i]
// 	// event_field.npix[i] = get_npix(); // [0: fpix, i: layer i]
// }
// 

// const PixelGeomDetUnit* geomdetunit      = dynamic_cast<const PixelGeomDetUnit*>(tracker -> idToDet(det_id));
// const PixelTopology&    topology         = geomdetunit -> specificTopology();
// LocalPoint const&       localpoint       = pixhit -> localPositionFast();
// MeasurementPoint        measurementpoint = topology.measurementPosition(localpoint);
// row = static_cast<int>(mp.x());
// col = static_cast<int>(mp.y());
// Temporarily saving row and col
// traj_field.row = row;
// traj_field.col = col;

// Pixel and strip hits
// traj_field.trk.pix   = n_pix_hits;
// traj_field.trk.strip = n_strip_hits;

// if(hit -> isValid())
// {
// 	// Counting the non-pixel hits
// 	if(subdetid == StripSubdetector::TIB) ++n_strip_hits;
// 	if(subdetid == StripSubdetector::TOB) ++n_strip_hits;
// 	if(subdetid == StripSubdetector::TID) ++n_strip_hits;
// 	if(subdetid == StripSubdetector::TEC) ++n_strip_hits;
// }

