#include "../interface/PhaseIDataTrees.h"

////////////////////
// Tree branching //
////////////////////

void PhaseIDataTrees::define_event_tree_branches(TTree*& event_tree, EventData& event_field)
{
	LogDebug("tree_branching") << "Defining event tree branches..." << std::endl;
	event_tree -> Branch("event", &event_field, event_field.list.c_str());
	LogDebug("tree_branching") << "Done defining event tree branches..." << std::endl;
}

void PhaseIDataTrees::define_cluster_tree_branches(TTree*& cluster_tree, EventData& event_field, Cluster& cluster_field)
{
	LogDebug("tree_branching") << "Defining cluster tree branches..." << std::endl;
	// cluster_tree -> Branch("event",        &event_field,          event_field.list.c_str());
	cluster_tree -> Branch("module",       &cluster_field.mod, cluster_field.mod.list.c_str());
	// cluster_tree -> Branch("module_on",    &cluster_field.mod_on, cluster_field.mod_on.list.c_str());
	// Serial number of cluster in the given module
	cluster_tree -> Branch("clust_i",      &cluster_field.i,      "i/I");
	// // Set if there is a valid hits
	// cluster_tree -> Branch("clust_edge",   &cluster_field.edge,   "edge/I");
	// cluster_tree -> Branch("clust_badpix", &cluster_field.badpix, "badpix/I");
	// cluster_tree -> Branch("clust_tworoc", &cluster_field.tworoc, "tworoc/I");
	// Position and size
	cluster_tree -> Branch("clust_xy",     &cluster_field.x,      "x/F:y");
	cluster_tree -> Branch("clust_size",   &cluster_field.size,   "size/I");
	cluster_tree -> Branch("clust_sizeXY", &cluster_field.sizeX,  "sizeX/I:sizeY");
	// Charge
	cluster_tree -> Branch("clust_charge", &cluster_field.charge, "charge/F");
	// Misc.
	cluster_tree -> Branch("clust_adc",    &cluster_field.adc,    "adc[size]/F");
	cluster_tree -> Branch("clust_pix",    &cluster_field.pix,    "pix[size][2]/F");
	LogDebug("tree_branching") << "Done defining cluster tree branches..." << std::endl;
}

void PhaseIDataTrees::define_traj_tree_branches(TTree*& traj_tree, EventData& event_field, TrajMeasurement& traj_field)
{
	LogDebug("tree_branching") << "Defining traj tree branches..." << std::endl;
	// traj_tree -> Branch("event",            &event_field,            event_field.list.data());
	// traj_tree -> Branch("module_on",        &traj_field.mod_on,      traj_field.mod_on.list.data());
	traj_tree -> Branch("module",           &traj_field.mod,      traj_field.mod.list.data());
	// traj_tree -> Branch("traj",             &traj_field,             "validhit/I:missing:lx/F:ly:res_dx:res_dz:lev:clust_near/I:hit_near:pass_effcuts");
	traj_tree -> Branch("traj",             &traj_field,             "validhit/I:missing/I:row/I:col/I:lx/F:ly/F");
	// traj_tree -> Branch("traj_occup",       &traj_field.nclu_mod,    "nclu_mod/I:nclu_roc:npix_mod:npix_roc");
	// traj_tree -> Branch("traj_alphabeta",   &traj_field.alpha,       "alpha/F:beta");
	// traj_tree -> Branch("traj_dxy_cl",      &traj_field.dx_cl,       "dx_cl[2]/F:dy_cl[2]");
	// traj_tree -> Branch("traj_dxy_hit",     &traj_field.dx_hit,      "dx_hit/F:dy_hit");
	// traj_tree -> Branch("traj_norm_charge", &traj_field.norm_charge, "norm_charge/F");
	// traj_tree -> Branch("clust_size",       &traj_field.clu.size,    "size/I");
	// traj_tree -> Branch("clust_sizeXY",     &traj_field.clu.sizeX,   "sizeX/I:sizeY");
	// traj_tree -> Branch("clust_adc",        &traj_field.clu.pix,     "adc[size]/F");
	// traj_tree -> Branch("track",            &traj_field.trk,         "validfpix[2]/I:validbpix[3]:strip:nstripmissing:nstriplost:nstriplayer:quality:d0/F:dz:pt");
	// traj_tree -> Branch("track_ndofchi2",   &traj_field.trk.ndof,    "ndof/F:chi2");
	// traj_tree -> Branch("track_eta",        &traj_field.trk.eta,     "eta/F");
	// traj_tree -> Branch("track_phi",        &traj_field.trk.phi,     "phi/F");
	LogDebug("tree_branching") << "Done defining traj tree branches..." << std::endl;
}

////////////////////////////////////
// Tree branch address assignment //
////////////////////////////////////

void PhaseIDataTrees::set_event_tree_data_fields(TTree*& event_tree, EventData& event_field)
{
	LogDebug("tree_branching") << "Setting branch addresses for event tree..." << std::endl;
	event_tree -> SetBranchAddress("event", &event_field);	
	LogDebug("tree_branching") << "Done setting branch addresses for event tree..." << std::endl;
}

void PhaseIDataTrees::set_cluster_tree_data_fields(TTree*& cluster_tree, EventData& event_field, Cluster& cluster_field)
{
	LogDebug("tree_branching") << "Setting branch addresses for cluster tree..." << std::endl;
	// cluster_tree -> SetBranchAddress("event",        &event_field);
	cluster_tree -> SetBranchAddress("module",    &cluster_field.mod);
	// cluster_tree -> SetBranchAddress("module_on",    &cluster_field.mod_on);
	// Serial number of cluster in the given module
	cluster_tree -> SetBranchAddress("clust_i",      &cluster_field.i);
	// // Set if there is a valid hits
	// cluster_tree -> SetBranchAddress("clust_edge",   &cluster_field.edge);
	// cluster_tree -> SetBranchAddress("clust_badpix", &cluster_field.badpix);
	// cluster_tree -> SetBranchAddress("clust_tworoc", &cluster_field.tworoc);
	// Position and size
	cluster_tree -> SetBranchAddress("clust_xy",     &cluster_field.x);
	cluster_tree -> SetBranchAddress("clust_size",   &cluster_field.size);
	cluster_tree -> SetBranchAddress("clust_sizeXY", &cluster_field.sizeX);
	// Charge
	cluster_tree -> SetBranchAddress("clust_charge", &cluster_field.charge);
	// Misc.
	cluster_tree -> SetBranchAddress("clust_adc",    &cluster_field.adc);
	cluster_tree -> SetBranchAddress("clust_pix",    &cluster_field.pix);
	LogDebug("tree_branching") << "Done setting branch addresses for cluster tree..." << std::endl;
}

void PhaseIDataTrees::set_traj_tree_data_fields(TTree*& traj_tree, EventData& event_field, TrajMeasurement& traj_field)
{
	LogDebug("tree_branching") << "Setting branch addresses for traj tree..." << std::endl;
	// traj_tree -> SetBranchAddress("event",            &event_field);
	traj_tree -> SetBranchAddress("module",           &traj_field.mod);
	// traj_tree -> SetBranchAddress("module_on",        &traj_field.mod_on);
	traj_tree -> SetBranchAddress("traj",             &traj_field);
	// traj_tree -> SetBranchAddress("traj_occup",       &traj_field.nclu_mod);
	// traj_tree -> SetBranchAddress("traj_alphabeta",   &traj_field.alpha);
	// traj_tree -> SetBranchAddress("traj_dxy_cl",      &traj_field.dx_cl);
	// traj_tree -> SetBranchAddress("traj_dxy_hit",     &traj_field.dx_hit);
	// traj_tree -> SetBranchAddress("traj_norm_charge", &traj_field.norm_charge);
	// traj_tree -> SetBranchAddress("clust_size",       &traj_field.clu.size);
	// traj_tree -> SetBranchAddress("clust_sizeXY",     &traj_field.clu.sizeX);
	// traj_tree -> SetBranchAddress("clust_adc",        &traj_field.clu.adc);
	// traj_tree -> SetBranchAddress("track",            &traj_field.trk);
	// traj_tree -> SetBranchAddress("track_ndofchi2",   &traj_field.trk.ndof);
	// traj_tree -> SetBranchAddress("track_eta",        &traj_field.trk.eta);
	// traj_tree -> SetBranchAddress("track_phi",        &traj_field.trk.phi);
	LogDebug("tree_branching") << "Done setting branch addresses for traj tree..." << std::endl;
}