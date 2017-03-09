#include "../interface/ClusterOccupancyModule.h"

ClusterOccupancyModule::ClusterOccupancyModule(HistoMapType& histogramsArg, const Cluster& clusterFieldArg): 
	histograms_     (histogramsArg),
	// eventField_     (eventFieldArg),
	clusterField_   (clusterFieldArg),
	det_            (clusterField_.mod_on.det),
	layer_          (clusterField_.mod_on.layer),
	ladderCoord_    (clusterField_.mod_on.ladder_coord),
	moduleCoord_    (clusterField_.mod_on.module_coord),
	bladePanelCoord_(clusterField_.mod_on.blade_panel_coord),
	diskRingCoord_  (clusterField_.mod_on.disk_ring_coord) {}

void ClusterOccupancyModule::checkHistogramDependencies()
{
	for(const auto& key: histogramKeyList_)
	{
		auto findResults = histograms_.find(std::string(key));
		if(findResults == histograms_.end())
		{
			std::cout << error_prompt << "cannot find histogram: " << key << " required by PileupScanModule. Terminating..." << std::endl;
			exit(-1);
		}
	}
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void ClusterOccupancyModule::fillHistograms()
{
	if(det_ == 0)
	{
		if(layer_ == 1) histograms_.at("clusterOccupancy_l1") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 2) histograms_.at("clusterOccupancy_l2") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 3) histograms_.at("clusterOccupancy_l3") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 4) histograms_.at("clusterOccupancy_l4") -> Fill(moduleCoord_, ladderCoord_);

	}
	if(det_ == 1)
	{
		histograms_.at("clusterOccupancy_fwd") -> Fill(diskRingCoord_, bladePanelCoord_);
	}
}
