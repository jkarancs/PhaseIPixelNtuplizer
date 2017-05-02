#include "../interface/ClusterOccupancyModule.h"

ClusterOccupancyModule::ClusterOccupancyModule(HistoMapType& histogramsArg, const Cluster& clusterFieldArg): 
	// eventField_     (eventFieldArg),
	clusterField_   (clusterFieldArg),
	det_            (clusterField_.mod_on.det),
	layer_          (clusterField_.mod_on.layer),
	disk_           (clusterField_.mod_on.disk),
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
	clusterOccupancy_l1                 = checkGetHistoFromMap("clusterOccupancy_l1");
	clusterOccupancy_l2                 = checkGetHistoFromMap("clusterOccupancy_l2");
	clusterOccupancy_l3                 = checkGetHistoFromMap("clusterOccupancy_l3");
	clusterOccupancy_l4                 = checkGetHistoFromMap("clusterOccupancy_l4");
	clusterOccupancy_fwd                = checkGetHistoFromMap("clusterOccupancy_fwd");
	clusterPhiVsZ_fwd                   = checkGetHistoFromMap("clusterPhiVsZ_fwd");
	clusterPhiVsZ_l1                    = checkGetHistoFromMap("clusterPhiVsZ_l1");
	clusterPhiVsZ_l2                    = checkGetHistoFromMap("clusterPhiVsZ_l2");
	clusterPhiVsZ_l3                    = checkGetHistoFromMap("clusterPhiVsZ_l3");
	clusterPhiVsZ_l4                    = checkGetHistoFromMap("clusterPhiVsZ_l4");
	clusterGlyVsGlx_barrel              = checkGetHistoFromMap("clusterGlyVsGlx_barrel");
	clusterGlyVsGlx_fwd_disk1           = checkGetHistoFromMap("clusterGlyVsGlx_fwd_disk1");
	clusterGlyVsGlx_positiveZ_fwd_disk1 = checkGetHistoFromMap("clusterGlyVsGlx_positiveZ_fwd_disk1");
	clusterGlyVsGlx_negativeZ_fwd_disk1 = checkGetHistoFromMap("clusterGlyVsGlx_negativeZ_fwd_disk1");
	clusterGlyVsGlx_fwd_disk2           = checkGetHistoFromMap("clusterGlyVsGlx_fwd_disk2");
	clusterGlyVsGlx_positiveZ_fwd_disk2 = checkGetHistoFromMap("clusterGlyVsGlx_positiveZ_fwd_disk2");
	clusterGlyVsGlx_negativeZ_fwd_disk2 = checkGetHistoFromMap("clusterGlyVsGlx_negativeZ_fwd_disk2");
	clusterGlyVsGlx_fwd_disk3           = checkGetHistoFromMap("clusterGlyVsGlx_fwd_disk3");
	clusterGlyVsGlx_positiveZ_fwd_disk3 = checkGetHistoFromMap("clusterGlyVsGlx_positiveZ_fwd_disk3");
	clusterGlyVsGlx_negativeZ_fwd_disk3 = checkGetHistoFromMap("clusterGlyVsGlx_negativeZ_fwd_disk3");
	clusterZ_total                      = checkGetHistoFromMap("clusterZ_total");
	clusterZ_barrel                     = checkGetHistoFromMap("clusterZ_barrel");
	clusterZ_forward                    = checkGetHistoFromMap("clusterZ_forward");
}

void ClusterOccupancyModule::fillHistograms()
{
	const float phi  = atan2(gly_, glx_);
	clusterZ_total -> Fill(glz_);
	if(det_ == 0)
	{
		clusterGlyVsGlx_barrel -> Fill(glx_, gly_);
		clusterZ_barrel        -> Fill(glz_);
		if(layer_ == 1)
		{
			clusterOccupancy_l1 -> Fill(moduleCoord_, ladderCoord_);
			clusterPhiVsZ_l1    -> Fill(glz_, phi);
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
		clusterPhiVsZ_fwd    -> Fill(glz_, phi);
		clusterZ_forward     -> Fill(glz_);
		if(std::abs(disk_) == 1)
		{
			clusterGlyVsGlx_fwd_disk1 -> Fill(glx_, gly_);
			if(0   <= glz_) clusterGlyVsGlx_positiveZ_fwd_disk1 -> Fill(glx_, gly_);
			if(glz_ <  0  ) clusterGlyVsGlx_negativeZ_fwd_disk1 -> Fill(glx_, gly_);
		}
		if(std::abs(disk_) == 2)
		{
			clusterGlyVsGlx_fwd_disk2 -> Fill(glx_, gly_);
			if(0   <= glz_) clusterGlyVsGlx_positiveZ_fwd_disk2 -> Fill(glx_, gly_);
			if(glz_ <  0  ) clusterGlyVsGlx_negativeZ_fwd_disk2 -> Fill(glx_, gly_);
		}
		if(std::abs(disk_) == 3)
		{
			clusterGlyVsGlx_fwd_disk3 -> Fill(glx_, gly_);
			if(0   <= glz_) clusterGlyVsGlx_positiveZ_fwd_disk3 -> Fill(glx_, gly_);
			if(glz_ <  0  ) clusterGlyVsGlx_negativeZ_fwd_disk3 -> Fill(glx_, gly_);
		}
	}
}
