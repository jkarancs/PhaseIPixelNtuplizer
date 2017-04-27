// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../../interface/DataStructures_v4.h"

// Utility
#include "../interface/TestsCommon.h"
#include "../interface/TTreeTools.h"
#include "../interface/HelperFunctionsCommon.h"
#include "../interface/CanvasExtras.h"
#include "../interface/TimerColored.h"
#include "../interface/ProgressBar.h"
#include "../interface/common_functions_jkarancs.h"
#include "../interface/CosmicTrajMeasHistosModule.h"
#include "../interface/ClusterOccupancyModule.h"
#include "../interface/WilsonScoreInterval.h"

// #include "../../../interface/CanvasExtras.h"

// Root
#include <TROOT.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TH2D.h>
// #include <TRandom3.h>

// C++ libraries
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>
#include <functional>
#include <algorithm>
#include <initializer_list>

#include "../interface/json.hpp"
using JSON = nlohmann::json;

constexpr float HALF_PI = 0.5 * 3.141592653589793238462;

constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE_1D = {0.95,  1.005};
constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE_2D = {0.945, 1.000};

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                    CONFIG_FILE_PATH                = "./config_cosmics_main.json"; 
const     std::string             EFFICIENCY_PLOT_IDENTIFIER      = "Efficiency";
const     std::string             EFFICIENCY_NUMERATOR_IDENTIFIER = "Numhits";

const std::vector<std::string>    HISTOGRAMS_TO_SAVE_NAMES = {"clusterOccupancy_l1", "clusterOccupancy_l2", "clusterOccupancy_l3", "clusterOccupancy_l4", "clusterOccupancy_fwd", "clusterPhiVsZ_fwd", "clusterPhiVsZ_l1", "clusterPhiVsZ_l2", "clusterPhiVsZ_l3", "clusterPhiVsZ_l4", "rechitPhiVsZ_l1", "clusterGlyVsGlx_barrel", "clusterGlyVsGlx_fwd_disk1", "clusterGlyVsGlx_positiveZ_fwd_disk1", "clusterGlyVsGlx_negativeZ_fwd_disk1", "clusterGlyVsGlx_fwd_disk2", "clusterGlyVsGlx_positiveZ_fwd_disk2", "clusterGlyVsGlx_negativeZ_fwd_disk2", "clusterGlyVsGlx_fwd_disk3", "clusterGlyVsGlx_positiveZ_fwd_disk3", "clusterGlyVsGlx_negativeZ_fwd_disk3", "rechitPhiVsZ_l2", "rechitPhiVsZ_l3", "rechitPhiVsZ_l4", "rechitPhiVsZ_fwd", "rechitOccupancy_l1", "rechitOccupancy_l2", "rechitOccupancy_l3", "rechitOccupancy_l4", "rechitOccupancy_fwd", "sensorEfficiencyWithCutsPhiVsZ_l1", "sensorEfficiencyWithCutsPhiVsZ_l2", "sensorEfficiencyWithCutsPhiVsZ_l3", "sensorEfficiencyWithCutsPhiVsZ_l4", "sensorEfficiencyWithCutsPhiVsZ_fwd", "rocEfficiencyWithCuts_l1", "rocEfficiencyWithCuts_l2", "rocEfficiencyWithCuts_l3", "rocEfficiencyWithCuts_l4", "rocEfficiencyWithCuts_fwd", "cosmicsRingNumhits", "cosmicsRingNumhitsWithAssociatedCluster", "cosmicsRingNumhitsDxyClLessThan1_0", "cosmicsRingEffDxyClLessThan0_5", "cosmicsRingsAverageDx", "cosmicsRingsAverageDy", "cosmicsRowVsColDxyClLessThan0_5", "rechitGlyVsGlx_barrel", "rechitGlyVsGlx_fwd_disk1", "rechitGlyVsGlx_positiveZ_fwd_disk1", "rechitGlyVsGlx_negativeZ_fwd_disk1", "rechitGlyVsGlx_fwd_disk2", "rechitGlyVsGlx_positiveZ_fwd_disk2", "rechitGlyVsGlx_negativeZ_fwd_disk2", "rechitGlyVsGlx_fwd_disk3", "rechitGlyVsGlx_positiveZ_fwd_disk3", "rechitGlyVsGlx_negativeZ_fwd_disk3", "associatedClusterXDistanceTotal", "associatedClusterYDistanceTotal", "associatedClusterXDistanceBarrel", "associatedClusterYDistanceBarrel", "associatedClusterXDistanceForward", "associatedClusterYDistanceForward"};
// Removed:
// "cluDistNumhitsPreCuts",
// "cluDistNumhitsBarrelPreCuts",
// "cluDistNumhitsForwardPreCuts",

const bool CLUST_LOOP_REQUESTED = true;
const bool TRAJ_LOOP_REQUESTED  = true;

void                                        testSaveFolders(const JSON& config);
TFile*                                      generateOutputNtuple(const JSON& config);
std::vector<std::string>                    getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey);
TH1*                                        getEfficiencyNumeratorHisto(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& efficiencyHistoName);
TGraphAsymmErrors*                          getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram);
void                                        addLegend(TH1* histogram);
void                                        addLegend(TH1* histogram, TGraphAsymmErrors* graph);

int main(int argc, char** argv) try
{
	std::cout << process_prompt << argv[0] << " started..." << std::endl;
	std::time_t processStarted = std::time(nullptr);
	std::cout << timer_prompt << "Time: " << std::asctime(std::localtime(&processStarted)) << std::flush;
	std::cout << process_prompt << "Loading config file... ";
	JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
	std::cout << "Done." << std::endl;
	testSaveFolders(config);
	TimerColored timer(timer_prompt);
	TFile* histogramsNtuple = generateOutputNtuple(config);
	gROOT -> SetBatch(kFALSE);
	EventData       eventField;
	Cluster         clusterField;
	TrajMeasurement trajField;
	// trajTreeChain -> Draw("d_cl >> hsqrt(1000, 0.0, 500.0)");
	// std::cout << "Running the app." << std::endl;
	// theApp -> Run();
	// Run number and wbc setting
	bool filterForRunNumberPresent = config.at("filter_for_runs");
	int  runNumberLowerBound = NOVAL_I;
	int  runNumberUpperBound = NOVAL_I;
	bool singleRun = false;
	std::string WBCSetting  = "WBC";
	if(filterForRunNumberPresent)
	{
		runNumberLowerBound  = config.at("run_filter_lower_bound").get<int>();
		runNumberUpperBound  = config.at("run_filter_upper_bound").get<int>();
		WBCSetting          += config.at("WBC_setting");
		if(runNumberLowerBound + 1 == runNumberUpperBound) singleRun = true;
		if(runNumberUpperBound <= runNumberLowerBound)
		{
			std::cout << error_prompt << "Inconsistent run filter setup." << std::endl;
			exit(-1);
		}
	}
	// Histogram definitions
	std::cout << process_prompt << "Loading histogram definitions... ";
	std::map<std::string, std::shared_ptr<TH1>> histograms(processHistogramDefinitions(config, "histogram_definition_list", "histogram_definitions"));
	std::cout << "Done." << std::endl;
	// Modules
	ClusterOccupancyModule     clusterOccupancyModule (histograms, clusterField);
	CosmicTrajMeasHistosModule cosmicTrajMeasHistosModule(histograms, eventField, trajField);
	//////////////////
	// Cluster loop //
	//////////////////
	if(CLUST_LOOP_REQUESTED) try
	{
		TChain* clustTreeChain = new TChain("clustTree", "List of the clusters.");
		readInFilesAndAddToChain(config, "input_files_list", "input_files", clustTreeChain);
		clustTreeChain -> SetBranchAddress("event",  &eventField);
		// clustTreeChain -> SetBranchAddress("mod",    &(clusterField.mod));
		clustTreeChain -> SetBranchAddress("mod_on", &(clusterField.mod_on));
		clustTreeChain -> SetBranchAddress("clust",  &clusterField);
		// check if data is present
		Long64_t clustTreeNumEntries  = clustTreeChain  -> GetEntries();
		std::cout << debug_prompt << "total entries in the clustTree chain: " << clustTreeNumEntries << std::endl;
		if(clustTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: clustTree.");
		ProgressBar progressBar;
		const int    progressBarUpdateInterval = 10000;
		const double progressBarUpdateBy       = progressBarUpdateInterval / static_cast<double>(clustTreeNumEntries) * 100;
		auto  updateAndPrintProgress =  [&] (const int& entryIndex)
		{
			if(entryIndex % progressBarUpdateInterval == 0)
			{
				progressBar.update(progressBarUpdateBy);
				progressBar.print();
				std::cout << " -- Estimated time left: " << std::setw(6) << std::fixed << std::setprecision(1) << timer.getSecondsElapsed() * (1 / progressBar.getProgressionRate() - 1) << " second(s).";
			}
		};
		timer.restart("Measuring the time required for looping on the clusters...");
		for(Long64_t entryIndex = 0; entryIndex < clustTreeNumEntries; ++entryIndex)
		{
			clustTreeChain -> GetEntry(entryIndex);
			if(filterForRunNumberPresent) if(eventField.run <  runNumberLowerBound || runNumberUpperBound <= eventField.run)
			{
				updateAndPrintProgress(entryIndex);
				continue;
			}
			clusterOccupancyModule.fillHistograms();
			updateAndPrintProgress(entryIndex);
		}
		std::cout << std::endl;
		timer.printSeconds("Loop done. Took about: ", " second(s).");
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "In the clusters loop: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	////////////////////////////////
	// Trajector measurement loop //
	////////////////////////////////
	if(TRAJ_LOOP_REQUESTED) try
	{
		TChain* trajTreeChain  = new TChain("trajTree", "List of the trajectory measurements.");
		readInFilesAndAddToChain(config, "input_files_list", "input_files", trajTreeChain);
		// Trajectory measurement tree
		trajTreeChain -> SetBranchAddress("event",  &eventField);
		// trajTreeChain -> SetBranchAddress("mod",    &(trajField.mod));
		trajTreeChain -> SetBranchAddress("mod_on",    &(trajField.mod_on));
		trajTreeChain -> SetBranchAddress("clust",     &(trajField.clu));
		trajTreeChain -> SetBranchAddress("track",     &(trajField.trk));
		trajTreeChain -> SetBranchAddress("clust_pix", &(trajField.clu.pix));

		trajTreeChain -> SetBranchAddress("traj",   &trajField);
		// check if data is present
		Long64_t trajTreeNumEntries  = trajTreeChain  -> GetEntries();
		std::cout << debug_prompt << "total entries in the trajTree chain: " << trajTreeNumEntries << std::endl;
		if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
		ProgressBar progressBar;
		const int    progressBarUpdateInterval = 10000;
		const double progressBarUpdateBy       = progressBarUpdateInterval / static_cast<double>(trajTreeNumEntries) * 100;
		auto  updateAndPrintProgress =  [&] (const int& entryIndex)
		{
			if(entryIndex % progressBarUpdateInterval == 0)
			{
				progressBar.update(progressBarUpdateBy);
				progressBar.print();
				std::cout << " -- Estimated time left: " << std::setw(6) << std::fixed << std::setprecision(1) << timer.getSecondsElapsed() * (1 / progressBar.getProgressionRate() - 1) << " second(s).";
			}
		};
		timer.restart("Measuring the time required for looping on the trajectory measurements...");
		for(Long64_t entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
		{
			trajTreeChain -> GetEntry(entryIndex);
			if(filterForRunNumberPresent) if(eventField.run <  runNumberLowerBound || runNumberUpperBound <= eventField.run)
			{
				updateAndPrintProgress(entryIndex);
				continue;
			}
			// printTrajFieldInfoTrajOnly(trajField);
			cosmicTrajMeasHistosModule.fillHistograms();
			updateAndPrintProgress(entryIndex);
		}
		std::cout << std::endl;
		timer.printSeconds("Loop done. Took about: ", " second(s).");
		cosmicTrajMeasHistosModule.printCounters();
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "In the traj meas loop: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	///////////////////////////////////////
	// Print Info about empty histograms //
	///////////////////////////////////////
	for(const auto& histogramName: HISTOGRAMS_TO_SAVE_NAMES)
	{
		TH1* histogram = histograms.at(histogramName).get();
		if(histogram -> GetEntries() == 0)
		{
			std::cout << process_prompt << "Info: " << histogramName << " has no entries." << std::endl;
			// continue;
		}
	}
	////////////////////////////
	// Scale Efficiency plots //
	////////////////////////////
	{
		cosmicTrajMeasHistosModule.postLoopScaleAveragesSpecialEfficiencies();
		for(const auto& histogramPair: histograms)
		{
			std::string efficiencyHistoName = histogramPair.first;
			if(efficiencyHistoName.find(EFFICIENCY_PLOT_IDENTIFIER) == std::string::npos) continue;
			auto numhitsHisto               = getEfficiencyNumeratorHisto(histograms, efficiencyHistoName);
			auto efficiencyHisto2DPtrConversionResult = dynamic_cast<TH2D*>(histogramPair.second.get());
			if(efficiencyHisto2DPtrConversionResult)
			{
				// std::cout << "Downscaling histogram: " << efficiencyHistoName << std::endl;
				downscale2DHistogram(efficiencyHisto2DPtrConversionResult, dynamic_cast<TH2D*>(numhitsHisto));
				efficiencyHisto2DPtrConversionResult -> GetZaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_2D.first, EFFICIENCY_ZOOM_RANGE_2D.second);
				continue;
			}
			downscale1DHistogram(dynamic_cast<TH1D*>(histogramPair.second.get()), dynamic_cast<TH1D*>(numhitsHisto));
			dynamic_cast<TH1D*>(histogramPair.second.get()) -> GetYaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_1D.first, EFFICIENCY_ZOOM_RANGE_1D.second);
		}
	}
	///////////////////////
	// Load plot options //
	///////////////////////
	struct PlotOptions
	{
		int dressPlot = 0;
		int layer     = -1;
		int customCanvasDimensions = 0;
		int histoSizeX = 800;
		int histoSizeY = 800;
		int customTicks = 0;
		int xAxisDivisions = 510;
		int yAxisDivisions = 510;
		int logXAxis = 0;
		int logYAxis = 0;
		int customXAxisLabels = 0;
		std::vector<std::string> xAxisLabels;
	};
	std::map<std::string, PlotOptions> plotOptionsMap;
	try
	{
		std::string histogramDefinitionListPath = config["histogram_definition_list"];
		JSON inputListJSON = JSON::parse(fileToString(histogramDefinitionListPath));
		std::vector<JSON> histogramDefinitionList = inputListJSON["histogram_definitions"];
		for(const auto& definition: histogramDefinitionList)
		{
			std::string name = definition.at("name");
			PlotOptions plotExtras;
			// std::cout << debug_prompt << name << " has a key for dress_plot: " << (definition.count("dress_plot") ? "true" : "false") << std::endl;
			if(definition.count("dress_plot"              ) != 0) plotExtras.dressPlot              = definition.at("dress_plot");
			if(definition.count("layer"                   ) != 0) plotExtras.layer                  = definition.at("layer");
			if(definition.count("custom_canvas_dimensions") != 0) plotExtras.customCanvasDimensions = definition.at("custom_canvas_dimensions");
			if(definition.count("histo_size_x"            ) != 0) plotExtras.histoSizeX             = definition.at("histo_size_x");
			if(definition.count("histo_size_y"            ) != 0) plotExtras.histoSizeY             = definition.at("histo_size_y");
			if(definition.count("custom_ticks"            ) != 0) plotExtras.customTicks            = definition.at("custom_ticks");
			if(definition.count("x_axis_divisions"        ) != 0) plotExtras.xAxisDivisions         = definition.at("x_axis_divisions");
			if(definition.count("y_axis_divisions"        ) != 0) plotExtras.yAxisDivisions         = definition.at("y_axis_divisions");
			if(definition.count("log_x_axis"              ) != 0) plotExtras.logXAxis               = definition.at("log_x_axis");
			if(definition.count("log_y_axis"              ) != 0) plotExtras.logYAxis               = definition.at("log_y_axis");
			if(definition.count("custom_x_axis_labels"    ) != 0) plotExtras.customXAxisLabels      = definition.at("custom_x_axis_labels");
			if(definition.count("x_axis_labels"           ) != 0) plotExtras.xAxisLabels            = definition.at("x_axis_labels").get<std::vector<std::string>>();
			plotOptionsMap.emplace(name, std::move(plotExtras));
		}
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "While loading plot options: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	////////////////
	// Save plots //
	////////////////
	try
	{
		gROOT  -> SetBatch(kTRUE); // Uncomment to check the plots on-the fly
		constexpr int PHASE_SCENARIO = 1;
		gStyle -> SetPalette(1);
		gStyle -> SetNumberContours(999);
		// gStyle -> SetOptStat(1111);
		gStyle -> SetOptStat(1112211); // Integral overflow Underflow RMS (value 2: +error) Mean (value 2: +error) Entries Title
		// gStyle -> SetOptStat(0);
		gErrorIgnoreLevel = kError;
		// histogram.SetTitleSize(22);
		for(const auto& histogramName: HISTOGRAMS_TO_SAVE_NAMES)
		{
			TH1* histogram                 = histograms.at(histogramName).get();
			std::string originalHistoTitle = histogram -> GetTitle();
			std::string xAxisTitle         = histogram -> GetXaxis() -> GetTitle();
			std::string yAxisTitle         = histogram -> GetYaxis() -> GetTitle();
			const PlotOptions& plotOptions = plotOptionsMap.at(histogramName);
			if(filterForRunNumberPresent)
			{
				if(singleRun) histogram -> SetTitle(("RUN: " + std::to_string(runNumberLowerBound) + "_" + WBCSetting + ": " + originalHistoTitle).c_str());
				else          histogram -> SetTitle(("RUNS: [ " + std::to_string(runNumberLowerBound) + "-" + std::to_string(runNumberUpperBound - 1) + "]_" + WBCSetting + ": " + originalHistoTitle).c_str());
			}
			TCanvas* canvas;
			int histoSizeX = 800;
			int histoSizeY = 800;
			if(plotOptions.customCanvasDimensions)
			{
				histoSizeX = plotOptions.histoSizeX;
				histoSizeY = plotOptions.histoSizeY;
			}
			canvas = custom_can_(histogram, histogramName + "_canvas", 0, 0, histoSizeX, histoSizeY, 80, 140);
			if(plotOptions.logXAxis) canvas -> SetLogx(1);
			if(plotOptions.logYAxis) canvas -> SetLogy(1);
			canvas -> cd();
			// std::cout << "histogramName: " << histogramName << std::endl;
			// std::cout << "plotOptionsMap.at(\"" << histogramName << "\").customTicks: " << plotOptionsMap.at(histogramName).customTicks << std::endl;
			// std::cout << "plotOptions.customTicks: " << plotOptions.customTicks << std::endl;
			if(plotOptions.customTicks)
			{
				histogram -> GetXaxis() -> SetNdivisions(plotOptions.xAxisDivisions);
				histogram -> GetYaxis() -> SetNdivisions(plotOptions.yAxisDivisions);
			}
			if(plotOptions.customXAxisLabels)
			{
				for(unsigned int numBin = 0; numBin < plotOptions.xAxisLabels.size(); ++numBin)
				{
					histogram -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, plotOptions.xAxisLabels[numBin].c_str());
				}
			}
			TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
			if(histo2D != nullptr)
			{
				histo2D -> GetXaxis() -> SetTitle(xAxisTitle.c_str());
				histo2D -> GetYaxis() -> SetTitle(yAxisTitle.c_str());
				histo2D -> Draw("COLZ");
				if(plotOptions.dressPlot)
				{
					dress_occup_plot(histo2D, plotOptions.layer, PHASE_SCENARIO);
					canvas -> Update();
				}
				// addLegend(histo2D);
			}
			else 
			{
				histogram -> SetFillColor(38);
				if(dynamic_cast<TH1D*>(histogram) == nullptr)
				{
					std::cout << error_prompt << " error while typecasting for drawing." << std::endl;
				}
				if(histogramName.find(EFFICIENCY_PLOT_IDENTIFIER) != std::string::npos)
				{
					TGraphAsymmErrors* graph = getGraphForEfficiencyWithAsymmetricErrors(*dynamic_cast<TH1D*>(histogram), *dynamic_cast<TH1D*>(getEfficiencyNumeratorHisto(histograms, histogramName)));
					graph -> SetName((histogramName + "AsGraph").c_str());
					if(plotOptions.customTicks)
					{
						graph -> GetXaxis() -> SetNdivisions(plotOptions.xAxisDivisions);
						graph -> GetYaxis() -> SetNdivisions(plotOptions.yAxisDivisions);
					}
					if(plotOptions.customXAxisLabels)
					{
						for(unsigned int numBin = 0; numBin < plotOptions.xAxisLabels.size(); ++numBin)
						{
							graph -> GetXaxis() -> ChangeLabel(numBin + 1, -1, 0.025, -1, -1, -1, plotOptions.xAxisLabels[numBin].c_str());
						}
						if(std::string(graph -> GetName()).find("Efficiency") != std::string::npos)
						{
							graph -> GetYaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE_1D.first, EFFICIENCY_ZOOM_RANGE_1D.second);
						}
					}
					graph -> Draw("AP");
					// addLegend(histogram, graph);
					if(config["save_histograms_to_ntuple"] == true)
					{
						graph -> Write();
					}
				}
				else
				{
					auto histo1D = dynamic_cast<TH1D*>(histogram);
					histo1D -> GetXaxis() -> SetTitle(xAxisTitle.c_str());
					histo1D -> GetYaxis() -> SetTitle(yAxisTitle.c_str());
					histo1D -> Draw("HIST");
					// addLegend(histogram);
				}
			}
			gPad -> Update();
			canvas -> Update();
			if(config["save_histograms_to_ntuple"] == true)
			{
				histogram -> Write();
			}
			// static int print = 0;
			// if(print++ == 0) canvas -> SaveAs("CanvasTest.C");
			{
				std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
				std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
				std::string savePath = config["plots_save_directory"].get<std::string>() + "/" + epsFilename;
				canvas -> SaveAs(savePath.c_str());
				if(config["save_histograms_to_ntuple"] == true)
				{
					canvas -> Write();
				}
			}
		}
		gErrorIgnoreLevel = kPrint;
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "While saving histograms: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	if(histogramsNtuple)
	{
		std::cout << process_prompt << "Saving histograms in: " << config["ntuple_save_directory"] << "/" << config["ntuple_name"] << "... ";
		histogramsNtuple -> Write();
		histogramsNtuple -> Close();
		std::cout << "Done." << std::endl;
	}
	std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
	return 0;
}
catch(const std::exception& e)
{
	std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
	return -1;
}

void testSaveFolders(const JSON& config)
{
	auto checkDirectory = [] (const std::string& directoryName)
	{
		try 
		{
			boost::filesystem::path        filePath   = directoryName; 
			boost::filesystem::file_status fileStatus = boost::filesystem::status(filePath);
			if(!boost::filesystem::is_directory(fileStatus))
			{
				std::cerr << "Error: Missing directory: " << directoryName << std::endl;
				std::cout << "Do you want to automatically create this missing directory: " << directoryName << "? (y/n): ";
				char answer = std::cin.get();
				if(answer == 'y')
				{
					boost::filesystem::create_directory(filePath);
					std::cout << "Directory created succesfully." << std::endl;
				}
				else
				{
					exit(-1);
				}
			}
		}
		catch(boost::filesystem::filesystem_error &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}; 
	// Directories to check
	try { checkDirectory(config.at("plots_save_directory" )); } catch(...) { std::cout << error_prompt << "Configuration might require the definition of plots_save_directory..."  << std::endl; }
	try { checkDirectory(config.at("ntuple_save_directory")); } catch(...) { std::cout << error_prompt << "Configuration might require the definition of ntuple_save_directory..." << std::endl; }
}

TFile* generateOutputNtuple(const JSON& config)
{
	if(config.find("save_histograms_to_ntuple") == config.end())
	{
		std::cout << error_prompt << "In the configurations: \"save_histograms_to_ntuple\" is undefined." << std::endl;
		exit(-1);
	}
	if(config["save_histograms_to_ntuple"] == false) return nullptr;
	std::string ntupleSavePath;
	try
	{
		std::string saveDirname  = config.at("ntuple_save_directory");
		std::string saveFilename = config.at("ntuple_name");
		ntupleSavePath           = saveDirname + "/" + saveFilename;
		std::cout << process_prompt << "Will be saving the histograms as: " << ntupleSavePath << std::endl;
	}
	catch(...) { ntupleSavePath = "./Ntuple.root"; }
	std::time_t timeCreated = std::time(nullptr);
	std::string fileTitle = std::string("Efficiency filter calibrations output histograms, created at: ") + std::asctime(std::localtime(&timeCreated));
	return new TFile(ntupleSavePath.c_str(), "RECREATE", fileTitle.c_str());
}

std::vector<std::string> getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey)
{
	std::string inputFilesListPath = config[configKey];
	JSON inputListJSON = JSON::parse(fileToString(inputFilesListPath));
	return inputListJSON[innerKey];	
}

void readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain)
{
	std::vector<std::string> inputFiles = getFilesFromConfig(config, configKey, innerKey);
	std::for_each(inputFiles.begin(), inputFiles.end(), [&] (const std::string& fileName) { chain  -> Add(fileName.c_str()); });
}

template <typename T>
T checkGetElement(const JSON& definition, const std::string& propertyName)
{
	auto propertyIt = definition.find(propertyName);
	if(propertyIt == definition.end())
	{
		if(definition.count("name") == 0) 
		{
			std::cout << error_prompt << "Element of unknown name has one or more missing properties..." << std::endl;
			exit(-1);
		}
		std::string name = definition["name"];
		std::cout << error_prompt << "Cant find property: " << propertyName << " for " << name << "." << std::endl;
		exit(-1);
	}
	return propertyIt.value();
}

std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey)
{
	auto noDeletionPolicyTH1D = [] (TH1D*) -> void {};
	auto noDeletionPolicyTH2D = [] (TH2D*) -> void {};
	std::map<std::string, std::shared_ptr<TH1>> histograms;
	std::string histogramDefinitionListPath = config[configKey];
	JSON inputListJSON = JSON::parse(fileToString(histogramDefinitionListPath));
	std::vector<JSON> histogramDefinitionList = inputListJSON[innerKey];
	for(const auto& definition: histogramDefinitionList)
	{
		std::string name  = checkGetElement<std::string>(definition, "name");
		std::string title = checkGetElement<std::string>(definition, "title");
		std::string type  = checkGetElement<std::string>(definition, "type");
		int nbinsx        = checkGetElement<int>(definition, "nbinsx");
		float xMin        = checkGetElement<float>(definition, "xMin");
		float xMax        = checkGetElement<float>(definition, "xMax");
		if(type == "TH1D") histograms.insert({name, std::shared_ptr<TH1D>(new TH1D(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax), noDeletionPolicyTH1D)});
		if(type == "TH2D")
		{
			int nbinsy        = checkGetElement<int>(definition, "nbinsy");
			float yMin        = checkGetElement<float>(definition, "yMin");
			float yMax        = checkGetElement<float>(definition, "yMax");
			histograms.insert({name, std::shared_ptr<TH2D>(new TH2D(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax, nbinsy, yMin, yMax), noDeletionPolicyTH2D)});
		}
	}
	return histograms;
}

TH1* getEfficiencyNumeratorHisto(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& efficiencyHistoName)
{
	if(histograms.find(efficiencyHistoName) == histograms.end())
	{
		std::cout << error_prompt << "Looking up efficiency histogram called " << efficiencyHistoName << " failed." << std::endl;
		return nullptr;
	}
	size_t effPos = efficiencyHistoName.find(EFFICIENCY_PLOT_IDENTIFIER);
	if(effPos != std::string::npos)
	{
		std::string numeratorHistoName = std::string(efficiencyHistoName).replace(effPos, EFFICIENCY_PLOT_IDENTIFIER.size(), EFFICIENCY_NUMERATOR_IDENTIFIER);
		auto numHitsHisto              = histograms.find(numeratorHistoName);
		if(numHitsHisto == histograms.end())
		{
			std::cout << error_prompt << "Efficiency histo: " << efficiencyHistoName << " exist, but there is no histogram called: " << numeratorHistoName << std::endl;
			return nullptr;
		}
		return numHitsHisto -> second.get();
	}
	return nullptr;
}

TGraphAsymmErrors* getGraphForEfficiencyWithAsymmetricErrors(const TH1D& efficiencyHistogram, const TH1D& numHitsHistogram)
{
	const TAxis* xAxis = efficiencyHistogram.GetXaxis();
	const TAxis* yAxis = efficiencyHistogram.GetYaxis();
	const int numBins = xAxis -> GetNbins();
	std::vector<Double_t> valuesX;
	std::vector<Double_t> valuesY;
	std::vector<Double_t> errorsXLow (numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
	std::vector<Double_t> errorsXHigh(numBins, 0.5 * xAxis -> GetBinWidth(xAxis -> GetFirst()));
	std::vector<Double_t> errorsYLow;
	std::vector<Double_t> errorsYHigh;
	for(int bin = 0; bin < numBins; ++bin)
	{
		valuesX.push_back(xAxis -> GetBinCenter(bin + 1));
		if(std::string(efficiencyHistogram.GetName()) == std::string("layersDisksEfficiency"))
		{
			std::cout << efficiencyHistogram.GetBinContent(bin + 1) << std::endl;
		}
		valuesY.push_back(efficiencyHistogram.GetBinContent(bin + 1));
		double lowerBound, upperBound;
		std::tie(lowerBound, upperBound) = WilsonScoreIntervalErrorCalculator(numHitsHistogram.GetBinContent(bin + 1), valuesY[bin], 1.0).getError();
		errorsYLow .emplace_back(std::move(valuesY[bin] - lowerBound  ));
		errorsYHigh.emplace_back(std::move(upperBound   - valuesY[bin]));
	}
	TGraphAsymmErrors* graph = new TGraphAsymmErrors(numBins, valuesX.data(), valuesY.data(), errorsXLow.data(), errorsXHigh.data(), errorsYLow.data(), errorsYHigh.data());
	graph -> SetTitle(efficiencyHistogram.GetTitle());
	graph -> GetXaxis() -> SetTitle (xAxis -> GetTitle());
	graph -> GetYaxis() -> SetTitle (yAxis -> GetTitle());
	graph -> GetXaxis() -> SetRangeUser (xAxis -> GetXmin(), xAxis -> GetXmax());
	// graph -> GetYaxis() -> SetRangeUser (yAxis -> GetXmin(), yAxis -> GetXmax());
	graph -> GetXaxis() -> SetNdivisions(xAxis -> GetNdivisions());
	graph -> GetYaxis() -> SetNdivisions(yAxis -> GetNdivisions());
	graph -> GetXaxis() -> SetLabelOffset(xAxis -> GetLabelOffset());
	graph -> GetYaxis() -> SetLabelOffset(yAxis -> GetLabelOffset());
	graph -> SetMarkerColor(4);
	graph -> SetMarkerStyle(24);
	graph -> SetLineWidth(1);
	graph -> SetLineStyle(1);
	return graph;
	// const_cast<TH1D*>(&efficiencyHistogram) -> Draw("HIST");
}

void addLegend(TH1* histogram)
{
	histogram -> SetTitleSize(0);
	TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
	TLegend* legend = new TLegend(0.45, 0.6, 0.8625, 0.9551);
	if(histo2D != nullptr)
	{
		legend -> AddEntry(histogram -> GetName(), histogram -> GetTitle(), "");
		legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), "");
		legend -> AddEntry("", ("X Mean: "    + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
		legend -> AddEntry("", ("X Std Dev: " + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
		legend -> AddEntry("", ("Y Mean: "    + std::to_string(histogram -> GetMean(2)  )).c_str(), "");
		legend -> AddEntry("", ("Y Std Dev: " + std::to_string(histogram -> GetStdDev(2))).c_str(), "");
	}
	else
	{
		legend -> AddEntry(histogram -> GetName(), histogram -> GetTitle(), "cf");
		legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), "");
		legend -> AddEntry("", ("Mean: "    + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
		legend -> AddEntry("", ("Std Dev: " + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
	}
	legend -> SetBorderSize(1);
	legend -> SetFillColor(10);
	legend -> SetFillStyle(3001);
	// legend -> SetFillStyle(1);
	// legend -> SetFillColorAlpha(10, 0.5);
	legend -> SetTextFont(42);
	legend -> SetTextSize(0.02);
	legend -> Draw();
}

void addLegend(TH1* histogram, TGraphAsymmErrors* graph)
{
	histogram -> SetTitleSize(0);
	TLegend* legend = new TLegend(0.45, 0.6, 0.8625, 0.9551);
	legend -> AddEntry(graph -> GetName(), graph -> GetTitle(), "pe");
	legend -> AddEntry("", ("Entries: " + std::to_string(histogram -> GetEntries())).c_str(), ""); 
	legend -> AddEntry("", ("Mean: "         + std::to_string(histogram -> GetMean(1)  )).c_str(), "");
	legend -> AddEntry("", ("Std Dev: "      + std::to_string(histogram -> GetStdDev(1))).c_str(), "");
	legend -> SetBorderSize(1);
	// legend -> SetFillStyle(1);
	// legend -> SetFillColorAlpha(10, 0.5);
	legend -> SetFillColor(10);
	legend -> SetFillStyle(3001);
	legend -> SetTextFont(42);
	legend -> SetTextSize(0.02);
	legend -> Draw();
}