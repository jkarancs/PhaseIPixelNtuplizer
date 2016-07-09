#include "PhaseIPixelNtuplizer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG


PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	LogDebug("step") << "PhaseIPixelNtuplizer() constructor called." << std::endl;

	/////////////
	// Options //
	/////////////

	cluster_save_downlscaling = 100;

	//////////////////////////////////////
	// Product consumption declarations //
	//////////////////////////////////////

	// FIXME: add reco for PhaseI vertices
	primary_vertices_token = consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
	clusters_token = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
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
		handle_default_error("file_operations", { "Failed to open output file: ", ntuple_output_filename });
	}

	LogDebug("file_operations") << "Output file: \"" << ntuple_output_filename << "\" created." << std::endl;

	////////////////////////
	// Branch definitions //
	////////////////////////

	// Event tree branches
	event_tree = new TTree("eventTree", "The event");
	PhaseIDataTrees::define_event_tree_branches(event_tree, event_field);
	// Cluster tree branches
	cluster_tree = new TTree("clustTree", "Pixel clusters");
	PhaseIDataTrees::define_cluster_tree_branches(cluster_tree, event_field, cluster_field);
	// Traj tree branches
	traj_tree = new TTree("trajTree", "Trajectory measurements in the Pixel");
	// PhaseIDataTrees::define_traj_tree_branches(traj_tree, event_field, traj_field);
}

void PhaseIPixelNtuplizer::endJob()
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endJob()..." << std::endl;

	LogDebug("file_operations") << "Writing plots to file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Write();
	LogDebug("file_operations") << "Closing file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << ntuple_output_filename << "\"." << std::endl;
}

void PhaseIPixelNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;

	////////////////
	// Event tree //
	////////////////

	// Set data holder object
	PhaseIDataTrees::set_event_tree_data_fields(event_tree, event_field);
	event_field.fill         = static_cast<int>(0); // FIXME
	event_field.run          = static_cast<int>(iEvent.id().run());
	event_field.ls           = static_cast<int>(iEvent.luminosityBlock());
	event_field.orb          = static_cast<int>(iEvent.orbitNumber());
	event_field.bx           = static_cast<int>(iEvent.bunchCrossing());
	event_field.evt          = static_cast<int>(iEvent.id().event());
	get_nvtx_and_vtx_data(iEvent);
	// TODO: define fields
	// event_field.trig         = get_trig();
	// event_field.l1_rate      = get_l1_rate();
	// event_field.intlumi      = static_cast<float>(lumi -> intgRecLumi());
	// event_field.instlumi     = static_cast<float>(lumi -> avgInsDelLumi());
	// event_field.instlumi_ext = get_instlumi_ext();
	// event_field.pileup       = get_pileup();
	// event_field.weight       = get_weight();
	// event_field.good         = get_good();
	// event_field.tmuon        = get_tmuon();
	// event_field.tmuon_err    = get_tmuon_err();
	// event_field.tecal        = get_tecal();
	// event_field.tecal_raw    = get_tecal_raw();
	// event_field.tecal_err    = get_tecal_err();
	// event_field.field        = get_field();
	// event_field.wbc          = get_wbc();
	// event_field.delay        = get_delay();
	// event_field.ntracks      = ...;
	for(int i = 0; i < 2; ++i)
	{
		// event_field.beamint      = [2];
	}
	for(int i = 0; i < 4; ++i)
	{
		// event_field.nclu[i] = get_nclu(); // [0: fpix, i: layer i]
		// event_field.npix[i] = get_npix(); // [0: fpix, i: layer i]
	}
	// Fill the tree
	event_tree -> Fill();

	//////////////////
	// Cluster tree //
	//////////////////

	// // Set data holder objects
	PhaseIDataTrees::set_cluster_tree_data_fields(cluster_tree, event_field, cluster_field);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster> > cluster_collection_handle;
	iEvent.getByToken(clusters_token, cluster_collection_handle);
	// Trying to access the clusters
	if(cluster_collection_handle.isValid())
	{
		// TODO: set module field
		int cluster_counter = 0;
		const edmNew::DetSetVector<SiPixelCluster>& current_cluster_collection = *cluster_collection_handle;
		// Parsing all nth cluster in the event
		typedef edmNew::DetSetVector<SiPixelCluster>::const_iterator clust_coll_it_t;
		for(clust_coll_it_t current_cluster_set_it = current_cluster_collection.begin(); current_cluster_set_it != current_cluster_collection.end(); ++current_cluster_set_it)
		{
			const auto& current_cluster_set = *current_cluster_set_it;
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
		handle_default_error("data_access", "Failed to fetch clusters.");
	}

	///////////////
	// Traj tree //
	///////////////



	/////////////////////////
	// For testing purpose //
	/////////////////////////

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
	// Get vertices by token
	edm::Handle<reco::VertexCollection> vertexCollectionHandle;
	iEvent.getByToken(primary_vertices_token, vertexCollectionHandle);
	// Loop on vertices
	event_field.nvtx = 0;
	const reco::VertexCollection& vertices = *vertexCollectionHandle.product();
	reco::VertexCollection::const_iterator primary_vtx = vertices.end(); // Should be invalid
	for(const auto& current_vertex: vertices)
	{
		// Invalid vertex
		if(!current_vertex.isValid()) continue;
		// Setting up some aliases
		const int it_ntrk       = static_cast<int>(current_vertex.tracksSize());
		const int& evt_ntrk     = event_field.vtxntrk;
		// Vertex checks
		bool ntrk_is_invalid       = evt_ntrk == NOVAL_I;
		bool has_more_trks         = evt_ntrk < it_ntrk;
		bool has_just_as_much_trks = evt_ntrk == it_ntrk;
		bool nvtx_z_is_smaller     = std::abs(event_field.vtxZ) > std::abs(current_vertex.z());
		// Check if it is the best vertex
		if(ntrk_is_invalid || has_more_trks || (has_just_as_much_trks && nvtx_z_is_smaller))
		{
			event_field.vtxntrk = it_ntrk;
			event_field.vtxD0   = current_vertex.position().rho();
			event_field.vtxX    = current_vertex.x();
			event_field.vtxY    = current_vertex.y();
			event_field.vtxZ    = current_vertex.z();
			event_field.vtxndof = current_vertex.ndof();
			event_field.vtxchi2 = current_vertex.chi2();
			primary_vtx         = static_cast<reco::VertexCollection::const_iterator>(&current_vertex);
		}
		// Counting the good vertices
		if(std::abs(current_vertex.z()) <= 25.0 && std::abs(current_vertex.position().rho()) <= 2.0 && current_vertex.ndof() > 4)
		{
			event_field.nvtx++;
		}
	}
}

//////////
// Test //
//////////

void PhaseIPixelNtuplizer::produce_fake_events(const int& num_events)
{
	for(int i = 0; i < num_events; ++i)
	{
		// Set data holder object
		event_tree -> SetBranchAddress("event", &event_field);
		// Assign random values to some of the fields
		event_field.fill = static_cast<int>(random.Rndm() * 3);
		event_field.fill = static_cast<int>(random.Rndm() * 3000 + 272232);
		event_field.instlumi = random.Rndm() * 100;
		// Fill the tree
		event_tree -> Fill();
	}
}

////////////////////
// Error handling //
////////////////////

void PhaseIPixelNtuplizer::handle_default_error(const std::string& stream_type, std::string msg)
{
	edm::LogError(stream_type.c_str()) << msg << std::endl;
	print_evt_info(stream_type);
	exit(-1); // TODO: replace with exception
}

void PhaseIPixelNtuplizer::handle_default_error(const std::string& stream_type, std::vector<std::string> msg)
{
	for(const auto& msg_part: msg)
	{
		edm::LogError(stream_type.c_str()) << msg_part;
	}
	edm::LogError(stream_type.c_str()) << std::endl;
	print_evt_info(stream_type);
	exit(-1); // TODO: replace with exception
}

void PhaseIPixelNtuplizer::print_evt_info(const std::string& stream_type)
{
	edm::LogError(stream_type.c_str()) <<
		"Run: " << event_field.run << 
		" Ls: " << event_field.ls  << 
		" Evt:" << event_field.evt << std::endl;
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);

// edm::LogError("category")    << "error    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogProblem("category")  << "problem  PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogWarning ("category") << "warning  PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogPrint("category")    << "print    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogInfo("category")     << "infor    PhaseIPixelNtuplizer() constructor called." << std::endl;
// edm::LogVerbatim("category") << "verbatim PhaseIPixelNtuplizer() constructor called." << std::endl;
