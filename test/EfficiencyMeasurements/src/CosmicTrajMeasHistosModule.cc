#include "../interface/CosmicTrajMeasHistosModule.h"

constexpr float CosmicTrajMeasHistosModule::HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
constexpr float CosmicTrajMeasHistosModule::BARREL_MODULE_EDGE_X_CUT;
constexpr float CosmicTrajMeasHistosModule::BARREL_MODULE_EDGE_Y_CUT;

CosmicTrajMeasHistosModule::CosmicTrajMeasHistosModule(HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg): 
	TrajMeasHistogramFillerModule(histogramsArg, eventFieldArg, trajFieldArg)
{
	getHistogramsFromHistoMap(histogramsArg);
}

void CosmicTrajMeasHistosModule::getHistogramsFromHistoMap(HistoMapType& histogramsArg)
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
	// Closest cluster distance: Total/BPix/FPix
	cluDistNumhitsPreCuts              = checkGetHistoFromMap("cluDistNumhitsPreCuts");
	cluDistNumhitsBarrelPreCuts        = checkGetHistoFromMap("cluDistNumhitsBarrelPreCuts");
	cluDistNumhitsForwardPreCuts       = checkGetHistoFromMap("cluDistNumhitsForwardPreCuts");
	// Rechit occupancy
	// Phi vs Z plots
	rechitPhiVsZ_l1                    = checkGetHistoFromMap("rechitPhiVsZ_l1");
	rechitPhiVsZ_l2                    = checkGetHistoFromMap("rechitPhiVsZ_l2");
	rechitPhiVsZ_l3                    = checkGetHistoFromMap("rechitPhiVsZ_l3");
	rechitPhiVsZ_l4                    = checkGetHistoFromMap("rechitPhiVsZ_l4");
	rechitPhiVsZ_fwd                   = checkGetHistoFromMap("rechitPhiVsZ_fwd");
	// ROC based plots
	rechitOccupancy_l1                 = checkGetHistoFromMap("rechitOccupancy_l1");
	rechitOccupancy_l2                 = checkGetHistoFromMap("rechitOccupancy_l2");
	rechitOccupancy_l3                 = checkGetHistoFromMap("rechitOccupancy_l3");
	rechitOccupancy_l4                 = checkGetHistoFromMap("rechitOccupancy_l4");
	rechitOccupancy_fwd                = checkGetHistoFromMap("rechitOccupancy_fwd");
	// Efficiency
	// Efficiency numerator
	// Phi vs Z plots
	sensorNumhitsWithCutsPhiVsZ_l1     = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l1");
	sensorNumhitsWithCutsPhiVsZ_l2     = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l2");
	sensorNumhitsWithCutsPhiVsZ_l3     = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l3");
	sensorNumhitsWithCutsPhiVsZ_l4     = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l4");
	sensorNumhitsWithCutsPhiVsZ_fwd    = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_fwd");
	// ROC based plots
	rocNumhitsWithCuts_l1              = checkGetHistoFromMap("rocNumhitsWithCuts_l1");
	rocNumhitsWithCuts_l2              = checkGetHistoFromMap("rocNumhitsWithCuts_l2");
	rocNumhitsWithCuts_l3              = checkGetHistoFromMap("rocNumhitsWithCuts_l3");
	rocNumhitsWithCuts_l4              = checkGetHistoFromMap("rocNumhitsWithCuts_l4");
	rocNumhitsWithCuts_fwd             = checkGetHistoFromMap("rocNumhitsWithCuts_fwd");
	// Efficiency
	// Phi vs Z plots
	sensorEfficiencyWithCutsPhiVsZ_l1  = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l1");
	sensorEfficiencyWithCutsPhiVsZ_l2  = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l2");
	sensorEfficiencyWithCutsPhiVsZ_l3  = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l3");
	sensorEfficiencyWithCutsPhiVsZ_l4  = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l4");
	sensorEfficiencyWithCutsPhiVsZ_fwd = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_fwd");
	// ROC based plots
	rocEfficiencyWithCuts_l1           = checkGetHistoFromMap("rocEfficiencyWithCuts_l1");
	rocEfficiencyWithCuts_l2           = checkGetHistoFromMap("rocEfficiencyWithCuts_l2");
	rocEfficiencyWithCuts_l3           = checkGetHistoFromMap("rocEfficiencyWithCuts_l3");
	rocEfficiencyWithCuts_l4           = checkGetHistoFromMap("rocEfficiencyWithCuts_l4");
	rocEfficiencyWithCuts_fwd          = checkGetHistoFromMap("rocEfficiencyWithCuts_fwd");
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void CosmicTrajMeasHistosModule::fillHistograms()
{
	const float phi        = atan2(gly_, glx_);
	const float& d_cl      = trajField_.d_cl;
	const int   clust_near = d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;
	const int   fillEfficiencyCondition = clust_near;
	calculateCuts();
	cluDistNumhitsPreCuts -> Fill(d_cl);
	if(det_ == 0)
	{
		cluDistNumhitsBarrelPreCuts -> Fill(d_cl);
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
		rechitOccupancy_fwd          -> Fill(moduleCoord_, ladderCoord_);
		fillPairs(sensorNumhitsWithCutsPhiVsZ_fwd, sensorEfficiencyWithCutsPhiVsZ_fwd, glz_,         phi,          fillEfficiencyCondition, effCutAll);
		fillPairs(rocNumhitsWithCuts_fwd,          rocEfficiencyWithCuts_fwd,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll);
	}
	incrementCounters();
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