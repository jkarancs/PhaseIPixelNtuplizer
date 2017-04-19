#pragma once

// Data structure
#include "../../../interface/DataStructures_v4.h"
#include "../interface/TimerColored.h"
#include "../interface/CommonActors.h"
#include "../interface/TrajMeasHistogramFillerModule.h"

#include "TH2D.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <memory>

class CosmicTrajMeasHistosModule: public TrajMeasHistogramFillerModule
{
	using HistoMapType       = std::map<std::string, std::shared_ptr<TH1>>;
	private:
		static constexpr float HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL = 0.10f;
		static constexpr float BARREL_MODULE_EDGE_X_CUT         = 0.6f;
		static constexpr float BARREL_MODULE_EDGE_Y_CUT         = 3.0f;
		// Cut values
		int lxFidCut    = 0;
		int lyFidCut    = 0;
		int effCutAll   = 0;
		// Counters
		int entry_counter_       = 0;
		int pixhitCut_counter_   = 0;
		int lxFidCut_counter_    = 0;
		int lyFidCut_counter_    = 0;
		int effCutAll_counter_   = 0;
	public:
		CosmicTrajMeasHistosModule(HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
		~CosmicTrajMeasHistosModule() = default;
		void getHistogramsFromHistoMap(HistoMapType& histogramsArg);
		void fillHistograms();
		void printCounters();
		void printCutValues();
	private:
		void calculateCuts();
		void incrementCounters();
};