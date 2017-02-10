// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../../interface/DataStructures_v2.h"

// Hitt efficiency calculation
#include "../../../interface/PhaseITrackingEfficiencyFilters.h"

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

const std::vector<int>                     NUMTRAJMEAS_NUM_BINS_ON_LAYERS = {10, 10, 10, 10};
const std::vector<int>                     LX_NUM_BINS_ON_LAYERS          = {100, 100, 100, 100};
const std::vector<int>                     LY_NUM_BINS_ON_LAYERS          = {100, 100, 100, 100};

const std::vector<std::pair<float, float>> NUMTRAJMEAS_RANGE_ON_LAYERS    = {{0.0f, 10.0f}, {0.0f, 10.0f}, {0.0f, 10.0f}, {0.0f, 10.0f}};
const std::vector<std::pair<float, float>> LX_RANGE_ON_LAYERS             = {{0.0f,  3.0f}, {0.0f,  3.0f}, {0.0f,  3.0f}, {0.0f,  3.0f}};
const std::vector<std::pair<float, float>> LY_RANGE_ON_LAYERS             = {{0.0f,  4.5f}, {0.0f,  4.5f}, {0.0f,  4.5f}, {0.0f,  4.5f}};


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
	// Separating the date into tracks with their associated trajectory measurements
	timer.restart("Measuring the time required for separating the traj measurements by tracks...");
	using UniqueTrackIdMap = std::map<int, std::map<int, std::vector<TrajMeasurement>>>;
	UniqueTrackIdMap trackTrajMeasMap = [] (TTree* const trajTree, const TrajMeasurement& trajField, const EventData& eventField) -> UniqueTrackIdMap
	{
		UniqueTrackIdMap trackTrajMeasMap;
		Int_t trajTreeNumEntries  = trajTree  -> GetEntries();
		for(Int_t entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex) 
		{
			trajTree -> GetEntry(entryIndex);
			int eventNum = eventField.evt;
			auto eventTracksIt = trackTrajMeasMap.find(eventNum);
			// If key does not exist yet: add key
			if(eventTracksIt == trackTrajMeasMap.end())
			{
				eventTracksIt = trackTrajMeasMap.emplace(eventField.evt, std::map<int, std::vector<TrajMeasurement>>()).first;
			}
			int trackNum = trajField.trk.i;
			auto& eventTracks = eventTracksIt -> second;
			auto trackTrajMeasIt = eventTracks.find(trackNum);
			// If key does not exist yet: add key
			if(trackTrajMeasIt == eventTracks.end())
			{
				trackTrajMeasIt = eventTracks.emplace(trackNum, std::vector<TrajMeasurement>()).first;
			}
			trackTrajMeasIt -> second.push_back(trajField);
		}
		return trackTrajMeasMap;
	}(trajTree, trajField, eventField);
	timer.printSeconds("Loop done. Took about: ", " second(s).");
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
	std::shared_ptr<TH1D> trajMeasNumberDist = std::make_shared<TH1D>("trajMeasNumberDist", "Num. traj measurements per track;num. traj. meas.;num. tracks", 10, 0, 10);
	histograms.push_back(trajMeasNumberDist);
	std::shared_ptr<TH2D> trajMeasOnBarrelGlXGlY                 = std::make_shared<TH2D>("trajMeasOnBarrelGlXGlY",                 "traj meas | det == 0 gly:glx;glx;gly",                             1000, -20.0, 20.0, 1000, -20.0, 20.0);
	std::shared_ptr<TH2D> trajMeasOnBarrelWithMultiplicityGlXGlY = std::make_shared<TH2D>("trajMeasOnBarrelWithMultiplicityGlXGlY", "trajMeas gly:glx | det == 0 && 1 < trajMeas multiplicity;glx;gly", 1000, -20.0, 20.0, 1000, -20.0, 20.0);
	// Histograms for each layer
	auto trajMeasNumberDistOnLayers        = histo1DForEachLayer("trajMeasNumberDist",            "Num. traj measurements per track",        "", "num. traj. meas.", "num. tracks", NUMTRAJMEAS_NUM_BINS_ON_LAYERS, NUMTRAJMEAS_RANGE_ON_LAYERS);
	auto lxForTrajMeasWithMultiplicity     = histo1DForEachLayer("lxForTrajMeasWithMultiplicity", "trajMeas lx | 1 < trajMeas multiplicity", "", "lx",               "num. tracks", LX_NUM_BINS_ON_LAYERS,          LX_RANGE_ON_LAYERS);
	auto lyForTrajMeasWithMultiplicity     = histo1DForEachLayer("lyForTrajMeasWithMultiplicity", "trajMeas ly | 1 < trajMeas multiplicity", "", "ly",               "num. tracks", LY_NUM_BINS_ON_LAYERS,          LY_RANGE_ON_LAYERS);
	auto lyVsLxForTrajMeasWithMultiplicity = histo2DForEachLayer("lyVsLxForTrajMeasWithMultiplicity", "trajMeas ly:lx | 1 < trajMeas multiplicity", "", "ly", "lx", "num. tracks", LX_NUM_BINS_ON_LAYERS, LX_RANGE_ON_LAYERS, LY_NUM_BINS_ON_LAYERS, LY_RANGE_ON_LAYERS);
	timer.restart("Measuring the time required for looping on the tracks...");
	// Trajector measurement loop, scoping local variables
	{
		for(const auto& eventTrackPair: trackTrajMeasMap)
		{
			const auto& eventTracks = eventTrackPair.second;
			for(const auto& trackTrajMeasPair: eventTracks)
			{
				const auto& currentTrackTrajMeasurements = trackTrajMeasPair.second;
				// std::cout << currentTrackTrajMeasurements.size() << std::endl;
				trajMeasNumberDist -> Fill(currentTrackTrajMeasurements.size());
				std::map<int, std::vector<TrajMeasurement const*>> hitsOnLayers;
				for(int i: range(1, 5)) hitsOnLayers.emplace(std::pair<int, std::vector<TrajMeasurement const*>>(i, {}));
				for(const auto& trajMeas: currentTrackTrajMeasurements)
				{
					const int& layer = trajMeas.mod_on.layer;
					if(trajMeas.mod_on.det == 0)
					{
						trajMeasOnBarrelGlXGlY -> Fill(trajMeas.glx, trajMeas.gly);
						if(layer == NOVAL_I) continue;
						hitsOnLayers.at(layer).push_back(&trajMeas);
					}
				}
				for(auto layerNum: range(1, 5))
				{
					const auto& hitsOnCurrentLayer    = hitsOnLayers.at(layerNum);
					const int&  numHitsOnCurrentLayer = hitsOnCurrentLayer.size();
					trajMeasNumberDistOnLayers[layerNum - 1] -> Fill(numHitsOnCurrentLayer);
					if(1 < numHitsOnCurrentLayer)
					{
						for(const auto hitPtr: hitsOnCurrentLayer)
						{
							trajMeasOnBarrelWithMultiplicityGlXGlY -> Fill(hitPtr -> glx, hitPtr -> gly);
							lxForTrajMeasWithMultiplicity[layerNum - 1] -> Fill(hitPtr -> lx);
							lyForTrajMeasWithMultiplicity[layerNum - 1] -> Fill(hitPtr -> ly);
							lyVsLxForTrajMeasWithMultiplicity[layerNum - 1] -> Fill(hitPtr -> lx, hitPtr -> ly);
						}
						
					}
				}
			}
		}
	}
	timer.printSeconds("Loop done. Took about: ", " second(s).");
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
		gStyle->SetOptStat(1);
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
	TCanvas glXGlYCanvas("canvas_glyglx", "trajMeas gly:glx | det == 0 && 1 < trajMeas multiplicity;glx;gly", 200, 450, 400, 400);
	glXGlYCanvas.cd();
	CanvasExtras::redesignCanvas(&glXGlYCanvas, static_cast<TH1*>((trajMeasOnBarrelWithMultiplicityGlXGlY.get())));
	trajMeasOnBarrelGlXGlY -> SetMarkerColor(kGray + 1);
	trajMeasOnBarrelWithMultiplicityGlXGlY -> SetMarkerColor(kRed);
	// trajMeasOnBarrelWithMultiplicityGlXGlY -> SetMarkerStyle(20);
	// trajMeasOnBarrelWithMultiplicityGlXGlY -> SetMarkerSize(0.15);
	trajMeasOnBarrelGlXGlY -> Draw();
	trajMeasOnBarrelWithMultiplicityGlXGlY -> Draw("SAME");
	glXGlYCanvas.SaveAs("results/trajMeas_gly:glx_|_det_==_0_&&_1_<_trajMeas_multiplicity.eps");
	inputFile -> Close();
	std::cout << process_prompt << inputFileName << " has been processed succesfully." << std::endl;
	std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
	// theApp -> Run();
	return 0;
}
catch(const std::exception& e)
{
	std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
	return -1;
}
