#include "../interface/ClusterOccupancyModule.h"

ClusterOccupancyModule::ClusterOccupancyModule(HistoMapType& histogramsArg, const Cluster& clusterFieldArg): 
	// eventField_     (eventFieldArg),
	clusterField_   (clusterFieldArg),
	det_            (clusterField_.mod_on.det),
	layer_          (clusterField_.mod_on.layer),
	ladderCoord_    (clusterField_.mod_on.ladder_coord),
	moduleCoord_    (clusterField_.mod_on.module_coord),
	bladePanelCoord_(clusterField_.mod_on.blade_panel_coord),
	diskRingCoord_  (clusterField_.mod_on.disk_ring_coord),
	glx_            (clusterField_.glx),
	gly_            (clusterField_.gly),
	glz_            (clusterField_.glz)
{
	auto checkGetHistoFromMap = [&] (const std::string& name) -> TH1*
	{
		try { return histogramsArg.at(name).get(); }
		catch(const std::out_of_range& e)
		{
			std::cout << error_prompt << e.what() << " in " << __PRETTY_FUNCTION__ << " while looking for: " << name << "." << std::endl;
			exit(-1);
			return nullptr;
		}
	};
	clusterOccupancy_l1  = checkGetHistoFromMap("clusterOccupancy_l1");
	clusterOccupancy_l2  = checkGetHistoFromMap("clusterOccupancy_l2");
	clusterOccupancy_l3  = checkGetHistoFromMap("clusterOccupancy_l3");
	clusterOccupancy_l4  = checkGetHistoFromMap("clusterOccupancy_l4");
	clusterOccupancy_fwd = checkGetHistoFromMap("clusterOccupancy_fwd");
	clusterPhiVsZ_fwd    = checkGetHistoFromMap("clusterPhiVsZ_fwd");
	clusterPhiVsZ_l1     = checkGetHistoFromMap("clusterPhiVsZ_l1");
	clusterPhiVsZ_l2     = checkGetHistoFromMap("clusterPhiVsZ_l2");
	clusterPhiVsZ_l3     = checkGetHistoFromMap("clusterPhiVsZ_l3");
	clusterPhiVsZ_l4     = checkGetHistoFromMap("clusterPhiVsZ_l4");
}

void ClusterOccupancyModule::fillHistograms()
{
	const float phi  = atan2(gly_, glx_);
	if(det_ == 0)
	{
		if(layer_ == 1)
		{
			clusterOccupancy_l1 -> Fill(moduleCoord_, ladderCoord_);
			clusterPhiVsZ_l1    -> Fill(glz_, phi);
			// std::cout << "glz_: " << glz_ << " phi: " << phi << std::endl; std::cin.get();
		}
		if(layer_ == 2)
		{
			clusterOccupancy_l2 -> Fill(moduleCoord_, ladderCoord_);
			clusterPhiVsZ_l2    -> Fill(glz_, phi);
		}
		if(layer_ == 3)
		{
			clusterOccupancy_l3 -> Fill(moduleCoord_, ladderCoord_);
			clusterPhiVsZ_l3    -> Fill(glz_, phi);
		}
		if(layer_ == 4)
		{
			clusterOccupancy_l4 -> Fill(moduleCoord_, ladderCoord_);
			clusterPhiVsZ_l4    -> Fill(glz_, phi);
		}

	}
	if(det_ == 1)
	{
		clusterOccupancy_fwd -> Fill(diskRingCoord_, bladePanelCoord_);
		clusterPhiVsZ_fwd -> Fill(glz_, phi);
	}
}
