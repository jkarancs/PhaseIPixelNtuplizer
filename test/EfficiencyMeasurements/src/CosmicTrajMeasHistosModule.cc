#include "../interface/CosmicTrajMeasHistosModule.h"

constexpr float CosmicTrajMeasHistosModule::HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
constexpr float CosmicTrajMeasHistosModule::BARREL_MODULE_EDGE_X_CUT;
constexpr float CosmicTrajMeasHistosModule::BARREL_MODULE_EDGE_Y_CUT;

CosmicTrajMeasHistosModule::CosmicTrajMeasHistosModule(const HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg): 
	TrajMeasHistogramFillerModule(histogramsArg, eventFieldArg, trajFieldArg)
{
	getHistogramsFromHistoMap(histogramsArg);
}

void CosmicTrajMeasHistosModule::getHistogramsFromHistoMap(const HistoMapType& histogramsArg)
{
	auto checkGetHistoFromMap = [&] (const std::string& name) -> TH1* {return this -> checkGetHistoFromMap(histogramsArg, name);};
	// Closest cluster distance: Total/BPix/FPix
	cluDistNumhitsPreCuts                   = checkGetHistoFromMap("cluDistNumhitsPreCuts");
	cluDistNumhitsBarrelPreCuts             = checkGetHistoFromMap("cluDistNumhitsBarrelPreCuts");
	cluDistNumhitsForwardPreCuts            = checkGetHistoFromMap("cluDistNumhitsForwardPreCuts");
	// Rechit occupancy
	// Phi vs Z plots
	rechitPhiVsZ_l1                         = checkGetHistoFromMap("rechitPhiVsZ_l1");
	rechitPhiVsZ_l2                         = checkGetHistoFromMap("rechitPhiVsZ_l2");
	rechitPhiVsZ_l3                         = checkGetHistoFromMap("rechitPhiVsZ_l3");
	rechitPhiVsZ_l4                         = checkGetHistoFromMap("rechitPhiVsZ_l4");
	rechitPhiVsZ_fwd                        = checkGetHistoFromMap("rechitPhiVsZ_fwd");
	// ROC based plots
	rechitOccupancy_l1                      = checkGetHistoFromMap("rechitOccupancy_l1");
	rechitOccupancy_l2                      = checkGetHistoFromMap("rechitOccupancy_l2");
	rechitOccupancy_l3                      = checkGetHistoFromMap("rechitOccupancy_l3");
	rechitOccupancy_l4                      = checkGetHistoFromMap("rechitOccupancy_l4");
	rechitOccupancy_fwd                     = checkGetHistoFromMap("rechitOccupancy_fwd");
	// Efficiency
	// Efficiency numerator
	// Phi vs Z plots
	sensorNumhitsWithCutsPhiVsZ_l1          = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l1");
	sensorNumhitsWithCutsPhiVsZ_l2          = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l2");
	sensorNumhitsWithCutsPhiVsZ_l3          = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l3");
	sensorNumhitsWithCutsPhiVsZ_l4          = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l4");
	sensorNumhitsWithCutsPhiVsZ_fwd         = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_fwd");
	// ROC based plots
	rocNumhitsWithCuts_l1                   = checkGetHistoFromMap("rocNumhitsWithCuts_l1");
	rocNumhitsWithCuts_l2                   = checkGetHistoFromMap("rocNumhitsWithCuts_l2");
	rocNumhitsWithCuts_l3                   = checkGetHistoFromMap("rocNumhitsWithCuts_l3");
	rocNumhitsWithCuts_l4                   = checkGetHistoFromMap("rocNumhitsWithCuts_l4");
	rocNumhitsWithCuts_fwd                  = checkGetHistoFromMap("rocNumhitsWithCuts_fwd");
	// Efficiency
	// Phi vs Z plots
	sensorEfficiencyWithCutsPhiVsZ_l1       = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l1");
	sensorEfficiencyWithCutsPhiVsZ_l2       = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l2");
	sensorEfficiencyWithCutsPhiVsZ_l3       = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l3");
	sensorEfficiencyWithCutsPhiVsZ_l4       = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l4");
	sensorEfficiencyWithCutsPhiVsZ_fwd      = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_fwd");
	// ROC based plots
	rocEfficiencyWithCuts_l1                = checkGetHistoFromMap("rocEfficiencyWithCuts_l1");
	rocEfficiencyWithCuts_l2                = checkGetHistoFromMap("rocEfficiencyWithCuts_l2");
	rocEfficiencyWithCuts_l3                = checkGetHistoFromMap("rocEfficiencyWithCuts_l3");
	rocEfficiencyWithCuts_l4                = checkGetHistoFromMap("rocEfficiencyWithCuts_l4");
	rocEfficiencyWithCuts_fwd               = checkGetHistoFromMap("rocEfficiencyWithCuts_fwd");
	// Ring num hits.
	cosmicsRingNumhits                      = checkGetHistoFromMap("cosmicsRingNumhits");
	cosmicsRingNumhitsWithAssociatedCluster = checkGetHistoFromMap("cosmicsRingNumhitsWithAssociatedCluster");
	cosmicsRingNumhitsDxyClLessThan1_0      = checkGetHistoFromMap("cosmicsRingNumhitsDxyClLessThan1_0");
	// Ring  efficiency and average
	cosmicsRingEffDxyClLessThan0_5          = checkGetHistoFromMap("cosmicsRingEffDxyClLessThan0_5");
	cosmicsRingsAverageDx                   = checkGetHistoFromMap("cosmicsRingsAverageDx");
	cosmicsRingsAverageDy                   = checkGetHistoFromMap("cosmicsRingsAverageDy");
	cosmicsRowVsColDxyClLessThan0_5         = checkGetHistoFromMap("cosmicsRowVsColDxyClLessThan0_5");
	// Gly vs glx, barrel
	rechitGlyVsGlx_barrel                   = checkGetHistoFromMap("rechitGlyVsGlx_barrel");
	// Gly vs glx, fwd
	rechitGlyVsGlx_fwd_disk1                = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk1");
	rechitGlyVsGlx_positiveZ_fwd_disk1      = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk1");
	rechitGlyVsGlx_negativeZ_fwd_disk1      = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk1");
	rechitGlyVsGlx_fwd_disk2                = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk2");
	rechitGlyVsGlx_positiveZ_fwd_disk2      = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk2");
	rechitGlyVsGlx_negativeZ_fwd_disk2      = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk2");
	rechitGlyVsGlx_fwd_disk3                = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk3");
	rechitGlyVsGlx_positiveZ_fwd_disk3      = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk3");
	rechitGlyVsGlx_negativeZ_fwd_disk3      = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk3");
	// Assodiated cluster distance
	associatedClusterXDistanceTotal              = checkGetHistoFromMap("associatedClusterXDistanceTotal");
	associatedClusterYDistanceTotal              = checkGetHistoFromMap("associatedClusterYDistanceTotal");
	associatedClusterXDistanceBarrel              = checkGetHistoFromMap("associatedClusterXDistanceBarrel");
	associatedClusterYDistanceBarrel              = checkGetHistoFromMap("associatedClusterYDistanceBarrel");
	associatedClusterXDistanceForward              = checkGetHistoFromMap("associatedClusterXDistanceForward");
	associatedClusterYDistanceForward              = checkGetHistoFromMap("associatedClusterYDistanceForward");
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void CosmicTrajMeasHistosModule::fillHistograms()
{
	const float  phi                     = atan2(gly_, glx_);
	const float& d_cl                    = trajField_.d_cl;
	const float  dx_cl                   = lx_ - trajField_.clu.lx;
	const float  dy_cl                   = ly_ - trajField_.clu.ly;
	const int    hasAssociatedCluster    = d_cl != NOVAL_F; 
	const float  absDx_cl                = std::abs(dx_cl);
	const float  absDy_cl                = std::abs(dy_cl);
	const int    clustWithin0_5X         = absDx_cl < 0.5f;
	const int    clustWithin0_5Y         = absDy_cl < 0.5f;
	const int    clustWithin_0_5XY       = hasAssociatedCluster && clustWithin0_5X && clustWithin0_5Y;
	const int    clustWithin1_0X         = absDx_cl < 1.0f;
	const int    clustWithin1_0Y         = absDy_cl < 1.0f;
	const int    clustWithin_1_0XY       = hasAssociatedCluster && clustWithin1_0X && clustWithin1_0Y;
	const int    clust_near              = d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
	const int    fillEfficiencyCondition = clust_near;
	calculateCuts();
	cluDistNumhitsPreCuts -> Fill(d_cl);
	for(const auto& rowCol: trajField_.clu.pix)
	{
		if(clustWithin_0_5XY)
		{
			cosmicsRowVsColDxyClLessThan0_5 -> Fill(rowCol[1], rowCol[0]);
		}
	}
	if(clustWithin_1_0XY)
	{
		associatedClusterXDistanceTotal -> Fill(dx_cl);
		associatedClusterYDistanceTotal -> Fill(dy_cl);
	}
	if(det_ == 0)
	{
		cluDistNumhitsBarrelPreCuts -> Fill(d_cl);
		rechitGlyVsGlx_barrel       -> Fill(glx_, gly_);
		cosmicsRingNumhits          -> Fill(module_);
		if(hasAssociatedCluster)
		{
			cosmicsRingNumhitsWithAssociatedCluster -> Fill(module_);
			associatedClusterXDistanceBarrel        -> Fill(dx_cl);
			associatedClusterYDistanceBarrel        -> Fill(dy_cl);
		}
		if(clustWithin_1_0XY)
		{
			cosmicsRingNumhitsDxyClLessThan1_0      -> Fill(module_);
			cosmicsRingsAverageDx                   -> Fill(module_, absDx_cl);
			cosmicsRingsAverageDy                   -> Fill(module_, absDy_cl);
		}
		if(clustWithin_0_5XY)
		{
			cosmicsRingEffDxyClLessThan0_5  -> Fill(module_); // for det == 1, module is also 1
		}
		if(layer_ == 1)
		{
			rechitPhiVsZ_l1    -> Fill(glz_, phi);
			rechitOccupancy_l1 -> Fill(moduleCoord_, ladderCoord_);
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l1, sensorEfficiencyWithCutsPhiVsZ_l1, glz_,         phi,          fillEfficiencyCondition, effCutAll);
			fillPairs(rocNumhitsWithCuts_l1,          rocEfficiencyWithCuts_l1,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
		}
		if(layer_ == 2)
		{
			rechitPhiVsZ_l2    -> Fill(glz_, phi);
			rechitOccupancy_l2 -> Fill(moduleCoord_, ladderCoord_);
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l2, sensorEfficiencyWithCutsPhiVsZ_l2, glz_,         phi,          fillEfficiencyCondition, effCutAll);
			fillPairs(rocNumhitsWithCuts_l2,          rocEfficiencyWithCuts_l2,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
		}
		if(layer_ == 3)
		{
			rechitPhiVsZ_l3    -> Fill(glz_, phi);
			rechitOccupancy_l3 -> Fill(moduleCoord_, ladderCoord_);
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l3, sensorEfficiencyWithCutsPhiVsZ_l3, glz_,         phi,          fillEfficiencyCondition, effCutAll);
			fillPairs(rocNumhitsWithCuts_l3,          rocEfficiencyWithCuts_l3,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
		}
		if(layer_ == 4)
		{
			rechitPhiVsZ_l4    -> Fill(glz_, phi);
			rechitOccupancy_l4 -> Fill(moduleCoord_, ladderCoord_);
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l4, sensorEfficiencyWithCutsPhiVsZ_l4, glz_,         phi,          fillEfficiencyCondition, effCutAll);
			fillPairs(rocNumhitsWithCuts_l4,          rocEfficiencyWithCuts_l4,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
		}
	}
	if(det_ == 1)
	{
		cluDistNumhitsForwardPreCuts -> Fill(d_cl);
		rechitPhiVsZ_fwd             -> Fill(glz_, phi);
		rechitOccupancy_fwd          -> Fill(diskRingCoord_, bladePanelCoord_);
		fillPairs(sensorNumhitsWithCutsPhiVsZ_fwd, sensorEfficiencyWithCutsPhiVsZ_fwd, glz_,         phi,          fillEfficiencyCondition, effCutAll);
		fillPairs(rocNumhitsWithCuts_fwd,          rocEfficiencyWithCuts_fwd,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
		if(hasAssociatedCluster)
		{
			associatedClusterXDistanceForward -> Fill(dx_cl);
			associatedClusterYDistanceForward -> Fill(dy_cl);
		}
		if(std::abs(disk_) == 1)
		{
			rechitGlyVsGlx_fwd_disk1 -> Fill(glx_, gly_);
			if(0   <= glz_) rechitGlyVsGlx_positiveZ_fwd_disk1 -> Fill(glx_, gly_);
			if(glz_ <  0  ) rechitGlyVsGlx_negativeZ_fwd_disk1 -> Fill(glx_, gly_);
		}
		if(std::abs(disk_) == 2)
		{
			rechitGlyVsGlx_fwd_disk2 -> Fill(glx_, gly_);
			if(0   <= glz_) rechitGlyVsGlx_positiveZ_fwd_disk2 -> Fill(glx_, gly_);
			if(glz_ <  0  ) rechitGlyVsGlx_negativeZ_fwd_disk2 -> Fill(glx_, gly_);
		}
		if(std::abs(disk_) == 3)
		{
			rechitGlyVsGlx_fwd_disk3 -> Fill(glx_, gly_);
			if(0   <= glz_) rechitGlyVsGlx_positiveZ_fwd_disk3 -> Fill(glx_, gly_);
			if(glz_ <  0  ) rechitGlyVsGlx_negativeZ_fwd_disk3 -> Fill(glx_, gly_);
		}
	}
	incrementCounters();
}

void CosmicTrajMeasHistosModule::postLoopScaleAveragesSpecialEfficiencies()
{
	// FIXME separate the TestsCommon into header and src file
	auto downscale1DHistogram = [] (TH1D* toDownscale, const TH1D* downscaleFactors)
	{
		const auto& nBins = toDownscale -> GetNbinsX();
		if(nBins != downscaleFactors -> GetNbinsX()) throw std::runtime_error(std::string("Error downscaling histograms: ") + toDownscale -> GetName() + ", " + downscaleFactors -> GetName() + " bin numbers do not match.");
		for(int binNum = 0; binNum <= nBins; ++binNum)
		{
			const auto originalBinContent = toDownscale      -> GetBinContent(binNum);
			const auto binDownscaleFactor = downscaleFactors -> GetBinContent(binNum);
			if(binDownscaleFactor == 0) continue;
			// std::cout << "Efficiency bin content before scaling: " << originalBinContent << std::endl;
			toDownscale -> SetBinContent(binNum, originalBinContent / binDownscaleFactor);
			// std::cout << "Efficiency bin content after scaling : " << toDownscale -> GetBinContent(binNum) << std::endl;
		}
	};
	downscale1DHistogram(dynamic_cast<TH1D*>(cosmicsRingEffDxyClLessThan0_5), dynamic_cast<TH1D*>(cosmicsRingNumhits));
	downscale1DHistogram(dynamic_cast<TH1D*>(cosmicsRingsAverageDx), dynamic_cast<TH1D*>(cosmicsRingNumhitsWithAssociatedCluster));
	downscale1DHistogram(dynamic_cast<TH1D*>(cosmicsRingsAverageDy), dynamic_cast<TH1D*>(cosmicsRingNumhitsWithAssociatedCluster));
}

void CosmicTrajMeasHistosModule::printCounters()
{
	std::cout << " --- Start cut counter values --- " << std::endl;
	std::cout << "lxFidCut:    " << std::setw(12) << lxFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lxFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "lyFidCut:    " << std::setw(12) << lyFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lyFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "total:       " << std::setw(12) << effCutAll_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << effCutAll_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << " --- End cut counter values --- " << std::endl;
}

void CosmicTrajMeasHistosModule::printCutValues()
{
	std::cout << " --- Start cut values --- " << std::endl;
	std::cout << "lxFidCut:    " << lxFidCut    << std::endl;
	std::cout << "lyFidCut:    " << lyFidCut    << std::endl;
	std::cout << " --- End cut values --- " << std::endl;
}

void CosmicTrajMeasHistosModule::calculateCuts()
{
	// Fidicual cuts
	lxFidCut = 1;
	lyFidCut = 1;
	if(det_ == 0)
	{
		lxFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_X_CUT;
		lyFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_Y_CUT;
	}
	effCutAll = lxFidCut && lyFidCut;
}

void CosmicTrajMeasHistosModule::incrementCounters()
{
	lxFidCut_counter_    += lxFidCut;
	lyFidCut_counter_    += lyFidCut;
	effCutAll_counter_   += effCutAll;
	entry_counter_++;
}