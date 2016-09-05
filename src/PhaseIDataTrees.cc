#include "../interface/PhaseIDataTrees.h"

////////////////////
// Tree branching //
////////////////////

void PhaseIDataTrees::defineEventTreeBranches(TTree*& eventTree, EventData& eventField)
{
	eventTree -> Branch("event", &eventField, eventField.list.c_str());
}

void PhaseIDataTrees::defineClusterTreeBranches(TTree*& clusterTree, EventData& eventField, Cluster& clusterField)
{
	// clusterTree -> Branch("event",        &eventField,          eventField.list.c_str());
	clusterTree -> Branch("module",       &clusterField.mod,    ModuleData::list.c_str());
	clusterTree -> Branch("module_on",    &clusterField.mod_on, ModuleData::list.c_str());
	// Serial number of cluster in the given module
	clusterTree -> Branch("clust_i",      &clusterField.i,      "i/I");
	// // Set if there is a valid hits
	// clusterTree -> Branch("clust_edge",   &clusterField.edge,   "edge/I");
	// clusterTree -> Branch("clust_badpix", &clusterField.badpix, "badpix/I");
	// clusterTree -> Branch("clust_tworoc", &clusterField.tworoc, "tworoc/I");
	// Position and size
	clusterTree -> Branch("clust_xy",     &clusterField.x,      "x/F:y");
	clusterTree -> Branch("clust_size",   &clusterField.size,   "size/I");
	clusterTree -> Branch("clust_sizeXY", &clusterField.sizeX,  "sizeX/I:sizeY");
	// Charge
	clusterTree -> Branch("clust_charge", &clusterField.charge, "charge/F");
	// Misc.
	clusterTree -> Branch("clust_adc",    &clusterField.adc,    "adc[size]/F");
	clusterTree -> Branch("clust_pix",    &clusterField.pix,    "pix[size][2]/F");
}

void PhaseIDataTrees::defineTrajTreeBranches(TTree*& trajTree, EventData& eventField, TrajMeasurement& trajField)
{
	// trajTree -> Branch("event",            &eventField,            eventField.list.data());
	trajTree -> Branch("module",           &trajField.mod,         ModuleData::list.c_str());
	trajTree -> Branch("module_on",        &trajField.mod_on,      ModuleData::list.c_str());
	// trajTree -> Branch("traj",             &trajField,             "validhit/I:missing:lx/F:ly:res_dx:res_dz:lev:clust_near/I:hit_near:pass_effcuts");
	trajTree -> Branch("traj",             &trajField,             "validhit/I:missing/I:row/I:col/I:lx/F:ly/F:glx/F:gly/F:glz/F:pass_effcuts/I");
	// trajTree -> Branch("traj_occup",       &trajField.nclu_mod,    "nclu_mod/I:nclu_roc:npix_mod:npix_roc");
	// trajTree -> Branch("traj_alphabeta",   &trajField.alpha,       "alpha/F:beta");
	// trajTree -> Branch("traj_dxy_cl",      &trajField.dx_cl,       "dx_cl[2]/F:dy_cl[2]");
	// trajTree -> Branch("traj_dxy_hit",     &trajField.dx_hit,      "dx_hit/F:dy_hit");
	// trajTree -> Branch("traj_norm_charge", &trajField.norm_charge, "norm_charge/F");
	// trajTree -> Branch("clust_size",       &trajField.clu.size,    "size/I");
	// trajTree -> Branch("clust_sizeXY",     &trajField.clu.sizeX,   "sizeX/I:sizeY");
	// trajTree -> Branch("clust_adc",        &trajField.clu.pix,     "adc[size]/F");
	// trajTree -> Branch("track",            &trajField.trk,         trajField.trk.list.c_str());
	// trajTree -> Branch("track_ndofchi2",   &trajField.trk.ndof,    "ndof/F:chi2");
	// trajTree -> Branch("track_eta",        &trajField.trk.eta,     "eta/F");
	// trajTree -> Branch("track_phi",        &trajField.trk.phi,     "phi/F");
}

////////////////////////////////////
// Tree branch address assignment //
////////////////////////////////////

void PhaseIDataTrees::setEventTreeDataFields(TTree*& eventTree, EventData& eventField)
{
	eventTree -> SetBranchAddress("event", &eventField);	
}

void PhaseIDataTrees::setClusterTreeDataFields(TTree*& clusterTree, EventData& eventField, Cluster& clusterField)
{
	// clusterTree -> SetBranchAddress("event",        &eventField);
	clusterTree -> SetBranchAddress("module",       &clusterField.mod);
	clusterTree -> SetBranchAddress("module_on",    &clusterField.mod_on);
	// Serial number of cluster in the given module
	clusterTree -> SetBranchAddress("clust_i",      &clusterField.i);
	// // Set if there is a valid hits
	// clusterTree -> SetBranchAddress("clust_edge",   &clusterField.edge);
	// clusterTree -> SetBranchAddress("clust_badpix", &clusterField.badpix);
	// clusterTree -> SetBranchAddress("clust_tworoc", &clusterField.tworoc);
	// Position and size
	clusterTree -> SetBranchAddress("clust_xy",     &clusterField.x);
	clusterTree -> SetBranchAddress("clust_size",   &clusterField.size);
	clusterTree -> SetBranchAddress("clust_sizeXY", &clusterField.sizeX);
	// Charge
	clusterTree -> SetBranchAddress("clust_charge", &clusterField.charge);
	// Misc.
	clusterTree -> SetBranchAddress("clust_adc",    &clusterField.adc);
	clusterTree -> SetBranchAddress("clust_pix",    &clusterField.pix);
}

void PhaseIDataTrees::setTrajTreeDataFields(TTree*& trajTree, EventData& eventField, TrajMeasurement& trajField)
{
	// trajTree -> SetBranchAddress("event",            &eventField);
	trajTree -> SetBranchAddress("module",           &trajField.mod);
	trajTree -> SetBranchAddress("module_on",        &trajField.mod_on);
	trajTree -> SetBranchAddress("traj",             &trajField);
	// trajTree -> SetBranchAddress("traj_occup",       &trajField.nclu_mod);
	// trajTree -> SetBranchAddress("traj_alphabeta",   &trajField.alpha);
	// trajTree -> SetBranchAddress("traj_dxy_cl",      &trajField.dx_cl);
	// trajTree -> SetBranchAddress("traj_dxy_hit",     &trajField.dx_hit);
	// trajTree -> SetBranchAddress("traj_norm_charge", &trajField.norm_charge);
	// trajTree -> SetBranchAddress("clust_size",       &trajField.clu.size);
	// trajTree -> SetBranchAddress("clust_sizeXY",     &trajField.clu.sizeX);
	// trajTree -> SetBranchAddress("clust_adc",        &trajField.clu.adc);
	// trajTree -> SetBranchAddress("track",            &trajField.trk);
	// trajTree -> SetBranchAddress("track_ndofchi2",   &trajField.trk.ndof);
	// trajTree -> SetBranchAddress("track_eta",        &trajField.trk.eta);
	// trajTree -> SetBranchAddress("track_phi",        &trajField.trk.phi);
}