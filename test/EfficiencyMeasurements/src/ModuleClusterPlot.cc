#include "../interface/ModuleClusterPlot.h"

constexpr std::array<const char*, 11> ModuleClusterPlot::histogramTypePrefixes;
std::vector<ModuleClusterPlot*> ModuleClusterPlot::moduleClusterPlotCollection;

ModuleClusterPlot::ModuleClusterPlot(Type typeArg, const int& layerArg, const int& moduleArg, const int& ladderArg, const int& startEventArg, const int& endEventArg):
	histogram(
		(std::string("ModuleClusterPlot") + "_" + std::to_string(moduleClusterPlotCollection.size()) + "_" + std::to_string(layerArg) + "_" + std::to_string(moduleArg) +  + "_" + std::to_string(ladderArg) + "_" + std::to_string(startEventArg) + "_" + std::to_string(endEventArg)).c_str(), 
		(std::string(histogramTypePrefixes[typeArg]) + " on layer " + std::to_string(layerArg) + ", module " + std::to_string(moduleArg) + ", ladder " + std::to_string(ladderArg) + ";module pix. (col);ladder pix. (row)").c_str(),
		416, 0, 416,
		160, 0, 160),
	canvas(histogram.GetName(), (histogram.GetTitle() + std::string(" ") + std::to_string(startEventArg) + " " + std::to_string(endEventArg)).c_str(), 50, 50, CANVAS_X_DIMENSION, CANVAS_Y_DIMENSION),
	type(typeArg),
	layer(layerArg),
	module(moduleArg),
	ladder(ladderArg),
	startEvent(startEventArg),
	endEvent(endEventArg)
{
	CanvasExtras::redesignCanvas(&canvas, &histogram);
	moduleClusterPlotCollection.push_back(this);
}

void ModuleClusterPlot::markerToRowColModifierArrays(const int& markerState, std::vector<int>& colModifiers, std::vector<int>& rowModifiers)
{
	colModifiers.clear();
	rowModifiers.clear();
	for(int colModifier = -1; colModifier <= 1; colModifier++)
	{
		for(int rowModifier = -1; rowModifier <= 1; rowModifier++)
		{
			if(colModifier == 0 && rowModifier == 0) continue;
			int bit = colModifier + 1 + (rowModifier + 1) * 3 - int(rowModifier > 0 || (colModifier > 0 && rowModifier == 0));
			if((markerState & (1 << bit)) == 0) continue;
			colModifiers.push_back(colModifier);
			rowModifiers.push_back(rowModifier);
		}
	}
}

void ModuleClusterPlot::fillMissingPixels(const int& col, const int& row, const int& markerState, const int& weight)
{
	std::vector<int> colModifiers;
	std::vector<int> rowModifiers;
	float xAxisBinWidth = histogram.GetXaxis() -> GetBinWidth(1);
	float yAxisBinWidth = histogram.GetYaxis() -> GetBinWidth(1);
	markerToRowColModifierArrays(markerState, colModifiers, rowModifiers);
	for(unsigned int markedNeighbourIndex = 0; markedNeighbourIndex < colModifiers.size(); ++markedNeighbourIndex)
	{
		int bin = histogram.GetBin(col + colModifiers[markedNeighbourIndex] * xAxisBinWidth, row + rowModifiers[markedNeighbourIndex] * yAxisBinWidth);
		if(histogram.GetBinContent(bin) == 0) histogram.SetBinContent(bin, weight);
	}
}

void ModuleClusterPlot::fillDigisFromCluster(const Cluster& cluster, const float& fillWeight)
{
	for(const int& digiIndex: range(cluster.pixelsCol.size()))
	{
		int col         = cluster.pixelsCol[digiIndex];
		int row         = cluster.pixelsRow[digiIndex];
		int markerState = cluster.pixelsMarker[digiIndex];
		switch(type)
		{
			case digis:
			case pairs:
			case pairsWithAngleLabels:
			case pairsWithIndividualAngleLabels:
				histogram.SetBinContent(col, row, BASE_DIGI_FILL_VALUE);
				break;
			case digisFromMarkers:
			case pairsWithMarkers:
			case fakePairsWithMarkers:
				if(markerState == 0) histogram.SetBinContent(col, row, BASE_DIGI_FILL_VALUE);
				else                 histogram.SetBinContent(col, row, markerState);
				break;
			case pairsWithNeighbours:
			case digisFromMarkersWithNeighbours:
				if(markerState == 0) histogram.SetBinContent(col, row, BASE_DIGI_FILL_VALUE);
				else                 histogram.SetBinContent(col, row, markerState);
				// histogram.SetBinContent(col, row, BASE_DIGI_FILL_VALUE);
				// fillMissingPixels(col, row, markerState, MISSING_NEIGHBOUR_VALUE);
				break;
			case pairsWithAngleColorCodes:
			case pairsWithIndividualAngleColors:
				// std::cout << "fillWeight: " << fillWeight << std::endl;
				histogram.SetBinContent(col, row, fillWeight);
				break;
			default:
				std::cerr << "Error in ModuleClusterPlot::fill(): Error deducing type for histogram type." << std::endl; 
				break;
		}
	}
}

int ModuleClusterPlot::isEventNumInRange(const int& eventNum)
{
	if(startEvent <= eventNum && eventNum <= endEvent) return true;
	return false;
}

void ModuleClusterPlot::fillDigisMarkers(const Cluster& cluster, const int& eventNum)
{
	auto filteredList = filter(moduleClusterPlotCollection, [&eventNum] (ModuleClusterPlot* plotToCheck) { return plotToCheck -> isEventNumInRange(eventNum); });
	filteredList      = filter(filteredList,                []          (ModuleClusterPlot* plotToCheck)
	{
		return 
			(plotToCheck -> type == digis) || 
			(plotToCheck -> type == digisFromMarkers) || 
			(plotToCheck -> type == digisFromMarkersWithNeighbours); 
	});
	if(filteredList.empty()) return;
	const ModuleData& mod_on = cluster.mod_on;
	if(mod_on.det != 0) return;
	filteredList = filter(filteredList, [&mod_on] (ModuleClusterPlot* plotToCheck)
	{
		return 
			mod_on.layer  == plotToCheck -> layer &&
			mod_on.module == plotToCheck -> module &&
			mod_on.ladder == plotToCheck -> ladder;
	});
	for(ModuleClusterPlot* plotDefinitionPtr: filteredList)
	{
		plotDefinitionPtr -> fillDigisFromCluster(cluster);
	}
}
void ModuleClusterPlot::fillDigisMarkers(const std::vector<Cluster>& clusterCollection, const int& eventNum)
{
	for(const auto& cluster: clusterCollection)
	{
		fillDigisMarkers(cluster, eventNum);
	}
}

void ModuleClusterPlot::fillAllPairs(const std::vector<Cluster>& clusterCollection, const int& eventNum)
{
	auto plotsToFillInTheEvent = filter(moduleClusterPlotCollection, [&eventNum] (ModuleClusterPlot* plotToCheck) { return plotToCheck -> isEventNumInRange(eventNum); });
	plotsToFillInTheEvent      = filter(plotsToFillInTheEvent,       []          (ModuleClusterPlot* plotToCheck)
	{ 
		return 
			(plotToCheck -> type == pairs                         ) || 
			(plotToCheck -> type == pairsWithMarkers              ) || 
			(plotToCheck -> type == fakePairsWithMarkers          ) || 
			(plotToCheck -> type == pairsWithNeighbours           ) || 
			(plotToCheck -> type == pairsWithAngleLabels          ) ||
			(plotToCheck -> type == pairsWithAngleColorCodes      ) ||
			(plotToCheck -> type == pairsWithIndividualAngleLabels) ||
			(plotToCheck -> type == pairsWithIndividualAngleColors);
	});
	if(plotsToFillInTheEvent.empty()) return;
	for(auto firstClusterIt = clusterCollection.begin(); firstClusterIt != clusterCollection.end(); ++firstClusterIt)
	{
		const ModuleData& mod1 = firstClusterIt -> mod_on;
		if(mod1.det != 0) continue;
		auto filteredList = filter(plotsToFillInTheEvent, [&mod1] (ModuleClusterPlot* plotToCheck) 
		{
			return 
				mod1.layer  == plotToCheck -> layer  &&
				mod1.module == plotToCheck -> module &&
				mod1.ladder == plotToCheck -> ladder;
		});
		if(filteredList.size() == 0) continue;
		for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterCollection.end(); ++secondClusterIt)
		{
			const ModuleData& mod2 = secondClusterIt -> mod_on;
			if(!(mod1 == mod2)) continue;
			if(!ClusterPairFunctions::areClustersPair(*firstClusterIt, *secondClusterIt)) continue;
			for(ModuleClusterPlot* plotDefinitionPtr: filteredList)
			{
				float relativeAngle = 0.0f;
				float angle1        = 0.0f;
				float angle2        = 0.0f;
				// Filter for fake pairs when necessary
				if(plotDefinitionPtr -> type == fakePairsWithMarkers)
				{
					if(ClusterPairFunctions::areClustersEndTaggedPair(*firstClusterIt, *secondClusterIt)) continue;
				}
				// Calculate relative angle when necessary
				if(plotDefinitionPtr -> type == pairsWithAngleColorCodes || plotDefinitionPtr -> type == pairsWithAngleLabels)
				{
					relativeAngle = ClusterPairFunctions::getClusterPairAngle(std::make_pair(
						std::make_shared<Cluster>(*firstClusterIt),
						std::make_shared<Cluster>(*secondClusterIt)));
				}
				// Calculate individual angles when necessary
				if(plotDefinitionPtr -> type == pairsWithIndividualAngleColors || plotDefinitionPtr -> type == pairsWithIndividualAngleLabels)
				{
					angle1 = ClusterPairFunctions::getClusterIndAngle(*firstClusterIt);
					angle2 = ClusterPairFunctions::getClusterIndAngle(*secondClusterIt);
				}
				// Fill clusters whith the appropriate weights
				// Relative angle color codes
				if(plotDefinitionPtr -> type == pairsWithAngleColorCodes)
				{
					plotDefinitionPtr -> fillDigisFromCluster(*firstClusterIt,  relativeAngle);
					plotDefinitionPtr -> fillDigisFromCluster(*secondClusterIt, relativeAngle);
				}
				// Individual angle color codes
				else if(plotDefinitionPtr -> type == pairsWithIndividualAngleColors)
				{
					plotDefinitionPtr -> fillDigisFromCluster(*firstClusterIt,  angle1);
					plotDefinitionPtr -> fillDigisFromCluster(*secondClusterIt, angle2);
				}
				// Normal histogram filling
				else
				{
					plotDefinitionPtr -> fillDigisFromCluster(*firstClusterIt);
					plotDefinitionPtr -> fillDigisFromCluster(*secondClusterIt);
				}
				// Relative angle labels
				if(plotDefinitionPtr -> type == pairsWithAngleLabels)
				{
					plotDefinitionPtr -> labels.emplace_back(std::make_shared<TText>(firstClusterIt -> pixelsCol[0] + 4, firstClusterIt -> pixelsRow[0] + 2, std::to_string(relativeAngle).c_str()));
					CanvasExtras::setLabelStyleComment(*(plotDefinitionPtr -> labels.back()));
				}
				// Individual angle labels
				if(plotDefinitionPtr -> type == pairsWithIndividualAngleLabels)
				{
					plotDefinitionPtr -> labels.emplace_back(std::make_shared<TText>(firstClusterIt -> pixelsCol[0] + 5, firstClusterIt -> pixelsRow[0] + 3, std::to_string(angle1).c_str()));
					CanvasExtras::setLabelStyleComment(*(plotDefinitionPtr -> labels.back()));	
					plotDefinitionPtr -> labels.emplace_back(std::make_shared<TText>(firstClusterIt -> pixelsCol[0] - 8, firstClusterIt -> pixelsRow[0] - 5, std::to_string(angle2).c_str()));
					CanvasExtras::setLabelStyleComment(*(plotDefinitionPtr -> labels.back()));	
				}
			}
		}
	}
}

void ModuleClusterPlot::saveAllFinished(const int& eventNum)
{
	auto filteredList = filter(moduleClusterPlotCollection, [&eventNum] (ModuleClusterPlot* plotToCheck) { return plotToCheck -> endEvent == eventNum; });
	if(filteredList.size() == 0) return;
	// Quick and dirty palette
	CanvasExtras::setMulticolorColzPalette();
	for(auto moduleClusterPlotToSave: filteredList)
	{
		TH2D& histogram = moduleClusterPlotToSave -> histogram;
		TCanvas& canvas = moduleClusterPlotToSave -> canvas;
		canvas.cd();
		// Angle identifier colors
		if(moduleClusterPlotToSave -> type == pairsWithAngleColorCodes)
		{
			histogram.GetZaxis() -> SetRangeUser(ANGLE_PALETTE_MINIMUM, ANGLE_PALETTE_MAXIMUM);
			histogram.GetZaxis() -> SetRangeUser(ANGLE_PALETTE_MINIMUM, ANGLE_PALETTE_MAXIMUM);
			histogram.GetZaxis() -> SetRangeUser(ANGLE_PALETTE_MINIMUM, ANGLE_PALETTE_MAXIMUM);			
		}
		// Marker identifier colors
		else
		{
			histogram.GetZaxis() -> SetRangeUser(NORMAL_PALETTE_MINIMUM, NORMAL_PALETTE_MAXIMUM);
			histogram.GetZaxis() -> SetRangeUser(NORMAL_PALETTE_MINIMUM, NORMAL_PALETTE_MAXIMUM);
			histogram.GetZaxis() -> SetRangeUser(NORMAL_PALETTE_MINIMUM, NORMAL_PALETTE_MAXIMUM);
			canvas.SetLogz();
		}
		// Slow, but a bit more accurate palette
		// CanvasExtras::setMulticolorColzPalette();
		histogram.Draw("COLZ");
		TText eventlabel;
		CanvasExtras::setLabelStyleNote(eventlabel);		
		eventlabel.DrawText(0.22, 0.98, (
			"Events: [" + 
			std::to_string(moduleClusterPlotToSave -> startEvent) + " - " +
			std::to_string(moduleClusterPlotToSave -> endEvent  ) + "]").c_str());
		for(std::shared_ptr<TText>& label: moduleClusterPlotToSave -> labels)
		{
			label -> Draw();
		}
		canvas.Update();
		// Just to make sure...
		canvas.Update();
		canvas.Update();
		std::string filename = canvas.GetTitle();
		std::transform(filename.begin(), filename.end(), filename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
		filename =  "results/" + filename + ".eps";
		canvas.SaveAs(filename.c_str());
	}
}