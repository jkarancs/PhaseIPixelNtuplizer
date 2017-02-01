#ifndef MODULE_CLUSTER_PLOT_H
#define MODULE_CLUSTER_PLOT_H

#include "../../../interface/Cluster.h"
#include "../../../interface/HelperFunctionsCommon.h"
#include "../../../interface/CanvasExtras.h"
#include "../interface/ClusterPairFunctions.h"

#include <TH2D.h>
#include <TCanvas.h>
#include <TText.h>

#include <string>
#include <memory>
#include <vector>
#include <array>
#include <functional>

class ModuleClusterPlot
{
	public:
		enum Type
		{
			digis = 0,
			digisFromMarkers,
			digisFromMarkersWithNeighbours,
			pairs,
			pairsWithMarkers,
			fakePairsWithMarkers,
			pairsWithNeighbours,
			pairsWithAngleLabels,
			pairsWithAngleColorCodes,
			pairsWithIndividualAngleLabels,
			pairsWithIndividualAngleColors,
		};
	private:
		static constexpr std::array<const char*, 11> histogramTypePrefixes = 
		{{ 
			"Digis",
			"Marked digis",
			"Ntuple markers with neighbours",
			"Cluster pairs",
			"Cluster pairs with marker positions",
			"Fake cluster pairs with marker positions",
			"Cluster pairs with neighbours",
			"Cluster pairs with angle labels",
			"Cluster pairs with angle color codes",
			"Cluster pairs with individual angle labels",
			"Cluster pairs with individual angle color codes"
		}};
		static constexpr float CANVAS_X_DIMENSION    = 400;
		static constexpr float CANVAS_Y_DIMENSION    = 300;
		static constexpr float BASE_DIGI_FILL_VALUE    = 0.5f;
		static constexpr float MISSING_NEIGHBOUR_VALUE = 1000.0f;
		static constexpr float NORMAL_PALETTE_MINIMUM  = 0.5f;
		static constexpr float NORMAL_PALETTE_MAXIMUM  = 1500.0f;
		static constexpr float ANGLE_PALETTE_MINIMUM   = 0.0;
		static constexpr float ANGLE_PALETTE_MAXIMUM   = 3.15;
		TH2D histogram;
		std::vector<std::shared_ptr<TText>> labels;
		TCanvas canvas;
		Type type;
		int layer;
		int module;
		int ladder;
		int startEvent;
		int endEvent;
		static std::vector<ModuleClusterPlot*> moduleClusterPlotCollection;
	private:
		static void markerToRowColModifierArrays(const int& markerState, std::vector<int>& colModifiers, std::vector<int>& rowModifiers);
		void        fillMissingPixels(const int& col, const int& row, const int& markerState, const int& weight);
		void        fillDigisFromCluster(const Cluster& cluster, const float& fillWeight = 0);
	public:
		ModuleClusterPlot(Type typeArg, const int& layer, const int& module, const int& ladder, const int& startEventArg, const int& endEventArg);
		int         isEventNumInRange(const int& eventNum);
		static void fillDigisMarkers(const Cluster& Cluster, const int& eventNum);
		static void fillDigisMarkers(const std::vector<Cluster>& clusterCollection, const int& eventNum);
		static void fillAllPairs(const std::vector<Cluster>& clusterCollection, const int& eventNum);
		static void saveAllFinished(const int& eventNum);
};

#endif