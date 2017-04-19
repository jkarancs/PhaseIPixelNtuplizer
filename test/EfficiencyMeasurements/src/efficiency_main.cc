// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../../interface/DataStructures_v2.h"

// Utility
#include "../interface/TestsCommon.h"
#include "../interface/TTreeTools.h"
#include "../interface/HelperFunctionsCommon.h"
#include "../interface/CanvasExtras.h"
#include "../interface/TimerColored.h"

// #include "../../../interface/CanvasExtras.h"

// Root
#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2D.h>
// #include <TRandom3.h>

// C++ libraries
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>
#include <functional>
#include <algorithm>

constexpr float HALF_PI = 0.5 * 3.141592653589793238462;

const bool                                 CLUSTER_LOOP_REQUESTED = false;
const bool                                 TRAJ_LOOP_REQUESTED    = true;

const std::vector<int>                     LADDER_NUM_BINS_ON_LAYERS = {13, 29, 45, 65};
const std::vector<int>                     MODULE_NUM_BINS_ON_LAYERS = {9, 9, 9, 9};
const std::vector<int>                     ALPHA_NUM_BINS_ON_LAYERS  = {100, 100, 100, 100};
const std::vector<int>                     BETA_NUM_BINS_ON_LAYERS   = {100, 100, 100, 100};

const std::vector<std::pair<float, float>> LADDER_RANGE_ON_LAYERS    = {{-6.0f, 7.0f }, {-14.0f, 15.0f }, {-22.0f, 23.0f }, {-32.0f, 33.0f }};
const std::vector<std::pair<float, float>> MODULE_RANGE_ON_LAYERS    = {{-4.0f, 5.0f }, { -4.0f,  5.0f }, { -4.0f,  5.0f }, { -4.0f,  5.0f }};
const std::vector<std::pair<float, float>> ALPHA_RANGE_ON_LAYERS     = {{ 0.0f, 3.14f}, {  0.0f,  3.14f}, {  0.0f,  3.14f}, {  0.0f,  3.14f}};
const std::vector<std::pair<float, float>> BETA_RANGE_ON_LAYERS      = {{ 0.0f, 3.14f}, {  0.0f,  3.14f}, {  0.0f,  3.14f}, {  0.0f,  3.14f}};

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);

int main(int argc, char** argv) try
{
	int savePlots = 0;
	std::string inputFileName;
	processArgs(argc, argv, inputFileName, savePlots);
	std::cout << process_prompt << argv[0] << " started..." << std::endl;
	TimerColored timer(timer_prompt);
	TApplication* theApp = new TApplication("App", &argc, argv);
	TFile* inputFile = new TFile(inputFileName.c_str());
	TTree* clustTree = (TTree*)(inputFile -> Get("clustTree"));
	TTree* trajTree  = (TTree*)(inputFile -> Get("trajTree" ));
	TTreeTools::treeCheck(clustTree, "Tree missing: clustTree.", true);
	TTreeTools::treeCheck(trajTree, "Tree missing: trajTree.", true);
	Cluster         clusterField;
	TrajMeasurement trajField;
	EventData       eventField;
	// Cluster tree
	TTreeTools::checkGetBranch(clustTree, "mod")    -> SetAddress(&(clusterField.mod));
	TTreeTools::checkGetBranch(clustTree, "mod_on") -> SetAddress(&(clusterField.mod_on));
	TTreeTools::checkGetBranch(clustTree, "clust")  -> SetAddress(&clusterField);
	// Trajectory measurement tree
	TTreeTools::checkGetBranch(trajTree, "event")   -> SetAddress(&eventField);
	TTreeTools::checkGetBranch(trajTree, "mod")     -> SetAddress(&(trajField.mod));
	TTreeTools::checkGetBranch(trajTree, "mod_on")  -> SetAddress(&(trajField.mod_on));
	TTreeTools::checkGetBranch(trajTree, "clust")   -> SetAddress(&(trajField.clu));
	TTreeTools::checkGetBranch(trajTree, "track")   -> SetAddress(&(trajField.trk));
	TTreeTools::checkGetBranch(trajTree, "traj")    -> SetAddress(&trajField);
	// Get number of entries
	Int_t clustTreeNumEntries = clustTree -> GetEntries();
	Int_t trajTreeNumEntries  = trajTree  -> GetEntries();
	// Check if data is present
	std::cout << debug_prompt << "Total entries in the cluster tree: " << clustTreeNumEntries << std::endl;
	if(clustTreeNumEntries == 0) throw std::runtime_error("No entries found in tree: clustTree.");
	std::cout << debug_prompt << "Total entries in the trajTree tree: " << trajTreeNumEntries << std::endl;
	if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
	// Histogram definitions
	std::vector<std::shared_ptr<TH1>> histograms;
	auto histo1DForEachLayer = [&histograms] 
		(const std::string& baseName,
		 const std::string& titlePrefix, const std::string& titlePostFix,
		 const std::string& xAxisName, const std::string& yAxisName, 
		 const std::vector<int>& numXBins, const std::vector<std::pair<float, float>>& xRanges)
	{
		std::vector<std::shared_ptr<TH1D>> histoForEachLayer;
		for(int layNum: range(1, 5))
		{
			histoForEachLayer.emplace_back(std::make_shared<TH1D>(
				(baseName + "lay" + std::to_string(layNum)).c_str(), 
				(titlePrefix + "[lay" + std::to_string(layNum) +"]" + titlePostFix + ";" + xAxisName + ";" + yAxisName + ";").c_str(), 
				numXBins[layNum - 1], xRanges[layNum - 1].first, xRanges[layNum - 1].second));
		}
		for(auto histo: histoForEachLayer) histograms.push_back(histo); 
		return histoForEachLayer;
	};
	auto histo2DForEachLayer = [&histograms] 
		(const std::string& baseName,
		 const std::string& titlePrefix, const std::string& titlePostFix,
		 const std::string& xAxisName, const std::string& yAxisName, const std::string& zAxisName, 
		 const std::vector<int>& numXBins, const std::vector<std::pair<float, float>>& xRanges, 
		 const std::vector<int>& numYBins, const std::vector<std::pair<float, float>>& yRanges)
	{
		std::vector<std::shared_ptr<TH2D>> histoForEachLayer;
		for(int layNum: range(1, 5))
		{
			histoForEachLayer.emplace_back(std::make_shared<TH2D>(
				(baseName + "lay" + std::to_string(layNum)).c_str(), 
				(titlePrefix + "[lay" + std::to_string(layNum) +"]" + titlePostFix + ";" + xAxisName + ";" + yAxisName + ";" + zAxisName + ";").c_str(), 
				numXBins[layNum - 1], xRanges[layNum - 1].first, xRanges[layNum - 1].second, 
				numYBins[layNum - 1], yRanges[layNum - 1].first, yRanges[layNum - 1].second));
		}
		for(auto histo: histoForEachLayer) histograms.push_back(histo); 
		return histoForEachLayer;
	};
	// Cluster occupancy
	auto clusterOccupancyVsLadderVsModule = histo2DForEachLayer("clusterOccupancyVsLadderVsModule", "Cluster occupancy vs ladder vs module", "", "ladder", "module", "num. clusts.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS, MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	auto clusterOccupancyVsLadder         = histo1DForEachLayer("clusterOccupancyVsLadder",         "Cluster occupancy vs ladder", "", "module", "num. clusts.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS);
	auto clusterOccupancyVsModule         = histo1DForEachLayer("clusterOccupancyVsModule",         "Cluster occupancy vs module", "", "module", "num. clusts.", MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	// Hit efficiency
	auto hitEfficiencyVsLadderVsModule = histo2DForEachLayer("hitEfficiencyVsLadderVsModule", "Hit efficiency vs ladder vs module", "", "ladder", "module", "hit eff.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS, MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	auto hitEfficiencyVsLadder         = histo1DForEachLayer("hitEfficiencyVsLadder",         "Hit efficiency vs ladder", "", "module", "hit eff.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS);
	auto hitEfficiencyVsModule         = histo1DForEachLayer("hitEfficiencyVsModule",         "Hit efficiency vs module", "", "module", "hit eff.", MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	auto hitEfficiencyVsAlpha          = histo1DForEachLayer("hitEfficiencyVsAlpha",          "Hit efficiency vs alpha",  "", "module", "hit eff.", ALPHA_NUM_BINS_ON_LAYERS , ALPHA_RANGE_ON_LAYERS );
	auto hitEfficiencyVsBeta           = histo1DForEachLayer("hitEfficiencyVsBeta",           "Hit efficiency vs beta",   "", "module", "hit eff.", BETA_NUM_BINS_ON_LAYERS  , BETA_RANGE_ON_LAYERS  );
	// Number of hits
	auto totalHitsVsLadderVsModule = histo2DForEachLayer("totalHitsVsLadderVsModule", "Number of hits vs ladder vs module", "", "ladder", "module", "hit eff.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS, MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	auto totalHitsVsLadder         = histo1DForEachLayer("totalHitsVsLadder",         "Number of hits vs ladder", "", "module", "hit eff.", LADDER_NUM_BINS_ON_LAYERS, LADDER_RANGE_ON_LAYERS);
	auto totalHitsVsModule         = histo1DForEachLayer("totalHitsVsModule",         "Number of hits vs module", "", "module", "hit eff.", MODULE_NUM_BINS_ON_LAYERS, MODULE_RANGE_ON_LAYERS);
	auto totalHitsVsAlpha          = histo1DForEachLayer("totalHitsVsAlpha",          "Number of hits vs alpha",  "", "module", "hit eff.", ALPHA_NUM_BINS_ON_LAYERS,  ALPHA_RANGE_ON_LAYERS );
	auto totalHitsVsBeta           = histo1DForEachLayer("totalHitsVsBeta",           "Number of hits vs beta",   "", "module", "hit eff.", BETA_NUM_BINS_ON_LAYERS ,  BETA_RANGE_ON_LAYERS  );
	// Cluster loop, scoping local variables
	if(CLUSTER_LOOP_REQUESTED)
	{
		timer.restart("Measuring the time required for looping on the clusters...");
		const int& layer  = clusterField.mod_on.layer;
		const int& ladder = clusterField.mod_on.ladder;
		const int& module = clusterField.mod_on.module;
		for(int entryIndex = 0; entryIndex < clustTreeNumEntries; ++entryIndex)
		{
			clustTree -> GetEntry(entryIndex);
			// Checking the layer number
			if(layer == NOVAL_I) continue;
			if(layer < 1 || 5 < layer) { std::cout << error_prompt << "Bad layer number: " << layer << "." << std::endl; continue; }
			clusterOccupancyVsLadderVsModule[layer - 1] -> Fill(ladder, module);
			clusterOccupancyVsLadder        [layer - 1] -> Fill(ladder);
			clusterOccupancyVsModule        [layer - 1] -> Fill(module);
		}
		timer.printSeconds("Loop done. Took about: ", " second(s).");
	}
	// Trajector measurement loop, scoping local variables
	if(TRAJ_LOOP_REQUESTED)
	{
		timer.restart("Measuring the time required for looping on the trajectory measurements...");
		const int&       layer  = trajField.mod_on.layer;
		const int&       ladder = trajField.mod_on.ladder;
		const int&       module = trajField.mod_on.module;
		const float&     alpha  = trajField.alpha;
		const float&     beta   = trajField.beta;
		// const int&   validhit = trajField.validhit;
		const int&   missing  = trajField.missing;
		for(int entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
		{
			trajTree -> GetEntry(entryIndex);
			// printTrajFieldInfo(trajField);
			// std::cout << "quality_1:      "  << (&trajField) -> trk.quality << "." << std::endl;
			// std::cout << "Ptr trajField: " << &trajField << std::endl;
			// PhaseITrackingEfficiencyFilters efficiencyFilters(&eventField, &trajField, &(trajField.trk));
			// std::cin.get();
			// Checking the layer number
			if(layer == NOVAL_I) continue;
			if(layer < 1 || 5 < layer) { std::cout << error_prompt << "Bad layer number: " << layer << "." << std::endl; continue; }
			// if(efficiencyFilters.performAllEfficiencyCuts() == false) continue;
			std::cout << "No efficiency filters were set up." << std::endl; exit(-1);
			// Efficiency denominators
			totalHitsVsLadderVsModule[layer - 1] -> Fill(ladder, module);
			totalHitsVsLadder        [layer - 1] -> Fill(ladder);
			totalHitsVsModule        [layer - 1] -> Fill(module);
			totalHitsVsAlpha         [layer - 1] -> Fill(alpha);
			totalHitsVsBeta          [layer - 1] -> Fill(beta);
			// Efficiency numerators
			if(missing && !trajField.hit_near) continue;
			hitEfficiencyVsLadderVsModule[layer - 1] -> Fill(ladder, module);
			hitEfficiencyVsLadder        [layer - 1] -> Fill(ladder);
			hitEfficiencyVsModule        [layer - 1] -> Fill(module);
			hitEfficiencyVsAlpha         [layer - 1] -> Fill(alpha);
			hitEfficiencyVsBeta          [layer - 1] -> Fill(beta);
		}
		timer.printSeconds("Loop done. Took about: ", " second(s).");
	}
	// CanvasExtras::setMulticolorColzPalette();
	gStyle -> SetPalette(1);
	gStyle -> SetNumberContours(999);
	gROOT->SetBatch(kTRUE);
	std::vector<std::shared_ptr<TCanvas>> canvases;
	for(const auto& i: range(histograms.size()))
	{
		auto& histogram = *(histograms[i]);
		canvases.emplace_back(std::make_shared<TCanvas>(("canvas_" + std::to_string(i)).c_str(), histogram.GetTitle(), 50 + i * 403, 50, 400, 300));
		canvases.back() -> cd();
		CanvasExtras::redesignCanvas(canvases.back().get(), &histogram);
		histogram.SetTitleSize(22);
		std::string histoTitle(histogram.GetTitle());
		std::string newTitle(histogram.GetTitle());
		canvases.back() -> SetRightMargin(0.15);
		// histogram.GetYaxis() -> SetNdivisions(507);
		histogram.SetFillColor(38);
		if(dynamic_cast<TH2D*>(histograms[i].get())) histogram.Draw("COLZ");
		// if(dynamic_cast<TH2D*>(*histograms[i])) histogram.Draw("CONTZ");
		else                                    histogram.Draw("BHIST");
		histogram.SetTitle(histoTitle.c_str());
		TText label;
		CanvasExtras::setLabelStyleNote(label);		
	}
	for(const auto& canvas: canvases)
	{
		canvas -> Update();
		if(savePlots)
		{
			std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
			std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
			canvas -> SaveAs(("results/" + epsFilename).c_str());
		}
	}
	inputFile -> Close();
	std::cout << process_prompt << inputFileName << " has been processed succesfully." << std::endl;
	std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
	theApp -> Run();
	return 0;
}
catch(const std::exception& e)
{
	std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
	return -1;
}
