#pragma once

// Data structure
#include "../../../interface/DataStructures_v4.h"
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
		// const EventData&       eventField_;
		const Cluster&         clusterField_;
		// Shortcuts
		const int&       det_;
		const int&       layer_;
		const float&     ladderCoord_;
		const float&     moduleCoord_;
		const float&     bladePanelCoord_;
		const float&     diskRingCoord_;
		const float&     glx_;
		const float&     gly_;
		const float&     glz_;
		// Key list (for checks)
		TH1* clusterOccupancy_fwd;
		TH1* clusterOccupancy_l1;
		TH1* clusterOccupancy_l2;
		TH1* clusterOccupancy_l3;
		TH1* clusterOccupancy_l4;
		TH1* clusterPhiVsZ_fwd;
		TH1* clusterPhiVsZ_l1;
		TH1* clusterPhiVsZ_l2;
		TH1* clusterPhiVsZ_l3;
		TH1* clusterPhiVsZ_l4;
	public:
		ClusterOccupancyModule(HistoMapType& histogramsArg, const Cluster& clusterFieldArg);
		~ClusterOccupancyModule() = default;
		void fillHistograms();
};