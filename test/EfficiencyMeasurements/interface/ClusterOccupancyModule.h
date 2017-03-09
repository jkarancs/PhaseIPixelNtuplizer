#pragma once

// Data structure
#include "../../../interface/DataStructures_v3.h"
#include "../interface/TimerColored.h"
#include "../interface/CommonActors.h"

#include "TH2D.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <memory>

class ClusterOccupancyModule
{
	using HistoMapType       = std::map<std::string, std::shared_ptr<TH1>>;
	private:
		HistoMapType&          histograms_;
		// const EventData&       eventField_;
		const Cluster&         clusterField_;
		// Shortcuts
		const int&       det_;
		const int&       layer_;
		const float&     ladderCoord_;
		const float&     moduleCoord_;
		const float&     bladePanelCoord_;
		const float&     diskRingCoord_;
		// Key list (for checks)
		const std::vector<std::string> histogramKeyList_ = 
		{
			"clusterOccupancy_fwd",
			"clusterOccupancy_l1",
			"clusterOccupancy_l2",
			"clusterOccupancy_l3",
			"clusterOccupancy_l4"
		};
	public:
		ClusterOccupancyModule(HistoMapType& histogramsArg, const Cluster& clusterFieldArg);
		~ClusterOccupancyModule() = default;
		void checkHistogramDependencies();
		void fillHistograms();
};