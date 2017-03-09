// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../../interface/DataStructures_v3.h"

// Hitt efficiency calculation
#include "../../../interface/PhaseITrackingEfficiencyFilters.h"

// Utility
#include "../interface/TestsCommon.h"
#include "../interface/TTreeTools.h"
#include "../interface/HelperFunctionsCommon.h"
#include "../interface/CanvasExtras.h"
#include "../interface/TimerColored.h"
#include "../interface/common_functions_jkarancs.h"
#include "../interface/FilterCalibrationModule.h"
#include "../interface/ClusterOccupancyModule.h"

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

constexpr auto                    PLOTS_SAVE_DIRECTORY               = "./results";
constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE              = {0.97, 1.01};

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                     CONFIG_FILE_PATH            = "./config_main.json"; 

const bool CLUST_LOOP_REQUESTED = false;
const bool TRAJ_LOOP_REQUESTED  = true;

void                                        testSaveFolders();
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                                        fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill,                     const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts = {});
void                                        fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts = {});

int main(int argc, char** argv) try
{
	std::cout << process_prompt << argv[0] << " started..." << std::endl;
	testSaveFolders();
	std::cout << process_prompt << "Loading config file... ";
	JSON config = JSON::parse(fileToString(CONFIG_FILE_PATH));
	std::cout << "Done." << std::endl;
	TimerColored timer(timer_prompt);
	TApplication* theApp = new TApplication("App", &argc, argv);
	gROOT->SetBatch(kFALSE);
	EventData       eventField;
	Cluster         clusterField;
	TrajMeasurement trajField;
	// trajTreeChain -> Draw("d_cl >> hsqrt(1000, 0.0, 500.0)");
	// std::cout << "Running the app." << std::endl;
	// theApp -> Run();
	// Histogram definitions
	std::cout << process_prompt << "Loading histogram definitions... ";
	std::map<std::string, std::shared_ptr<TH1>> histograms(processHistogramDefinitions(config, "histogram_definition_list", "histogram_definitions"));
	std::cout << process_prompt << "Done." << std::endl;
	// Modules
	ClusterOccupancyModule  clusterOccupancyModule (histograms, clusterField);
	FilterCalibrationModule filterCalibrationModule(histograms, eventField, trajField);
	// Checking if every histogram definition required is present
	clusterOccupancyModule .checkHistogramDependencies();
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
		if(clustTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
		timer.restart("Measuring the time required for looping on the clusters...");
		for(Long64_t entryIndex = 0; entryIndex < clustTreeNumEntries; ++entryIndex)
		{
			clustTreeChain -> GetEntry(entryIndex);
			clusterOccupancyModule.fillHistograms();
		}
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
		trajTreeChain -> SetBranchAddress("mod_on", &(trajField.mod_on));
		trajTreeChain -> SetBranchAddress("clust",  &(trajField.clu));
		trajTreeChain -> SetBranchAddress("track",  &(trajField.trk));
		trajTreeChain -> SetBranchAddress("traj",   &trajField);
		// check if data is present
		Long64_t trajTreeNumEntries  = trajTreeChain  -> GetEntries();
		std::cout << debug_prompt << "total entries in the trajTree chain: " << trajTreeNumEntries << std::endl;
		if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
		timer.restart("Measuring the time required for looping on the trajectory measurements...");
		for(Long64_t entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
		{
			trajTreeChain -> GetEntry(entryIndex);
			// printTrajFieldInfoTrajOnly(trajField);
			filterCalibrationModule.fillHistograms();
		}
		filterCalibrationModule.printCounters();
		timer.printSeconds("Loop done. Took about: ", " second(s).");
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "In the traj meas loop: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	////////////////////////////
	// Scale Efficiency plots //
	////////////////////////////
	{
		static const std::string efficiencyPlotIdentifier      = "Efficiency";
		static const std::string efficiencyNumeratorIdentifier = "Numhits";
		for(const auto& histogramPair: histograms)
		{
			std::string efficiencyHistoName = histogramPair.first;
			size_t      effPos              = efficiencyHistoName.find(efficiencyPlotIdentifier);
			if(effPos == std::string::npos)      continue;
			std::string numeratorHistoName  = std::string(efficiencyHistoName).replace(effPos, efficiencyPlotIdentifier.size(), efficiencyNumeratorIdentifier);
			auto numHitsHisto               = histograms.find(numeratorHistoName);
			if(numHitsHisto == histograms.end())
			{
				std::cout << error_prompt << "Efficiency histo: " << efficiencyHistoName << " exist, but there is no histogram called: " << numeratorHistoName << std::endl;
				continue;
			}
			auto efficiencyHisto2DPtrConversionResult = dynamic_cast<TH2D*>(histogramPair.second.get());
			if(efficiencyHisto2DPtrConversionResult)
			{
				// std::cout << "Downscaling histogram: " << efficiencyHistoName << std::endl;
				downscale2DHistogram(efficiencyHisto2DPtrConversionResult, dynamic_cast<TH2D*>(numHitsHisto -> second.get()));
				efficiencyHisto2DPtrConversionResult -> GetZaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE.first, EFFICIENCY_ZOOM_RANGE.second);
				continue;
			}
			downscale1DHistogram(dynamic_cast<TH1D*>(histogramPair.second.get()), dynamic_cast<TH1D*>(numHitsHisto -> second.get()));
			dynamic_cast<TH1D*>(histogramPair.second.get()) -> GetYaxis() -> SetRangeUser(EFFICIENCY_ZOOM_RANGE.first, EFFICIENCY_ZOOM_RANGE.second);
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
		gStyle -> SetOptStat(1111);
		gErrorIgnoreLevel = kError;
		// histogram.SetTitleSize(22);
		for(const auto& histogramPair: histograms)
		{
			const std::string& histogramName = histogramPair.first;
			TH1*               histogram     = histogramPair.second.get();
			if(histogram -> GetEntries() == 0)
			{
				std::cout << process_prompt << histogramName << " has no entries. It will not be saved." << std::endl;
				continue;
			}
			const PlotOptions& plotOptions   = plotOptionsMap.at(histogramName);
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
			// std::cout << "Histogram name: "    << histogramName << std::endl;
			// std::cout << "Number of entries: " << histogram -> GetEntries() << std::endl; 
			TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
			if(histo2D != nullptr)
			{
				// std::cout << debug_prompt << "2D plot found, name: " << histogramName << " dressPlot value: " << plotOptions.dressPlot << std::endl;
				histo2D -> Draw("COLZ");
				if(plotOptions.dressPlot)
				{
					dress_occup_plot(histo2D, plotOptions.layer, PHASE_SCENARIO);
					canvas -> Update();
					TPaveStats* stats = (TPaveStats*) histo2D -> GetListOfFunctions() -> FindObject("stats");
					if(stats) stats -> Draw("SAME");
					else std::cout << error_prompt << "Cannot redraw stats box for " << histo2D -> GetName() << ": no stats found. " << std::endl;
					// std::cout << debug_prompt << "Dress occup plot called with layer: " << plotOptions.layer << " and phase scenario: " << PHASE_SCENARIO << "." << std::endl;
				}
			}
			else 
			{
				histogram -> SetFillColor(38);
				if(dynamic_cast<TH1D*>(histogram) == nullptr)
				{
					std::cout << error_prompt << " error while typecasting for drawing." << std::endl;
				}
				// else
				// {
					// std::cout << "Drawn with entries: " << dynamic_cast<TH1D*>(histogram) -> GetEntries() << std::endl;
				// }
				dynamic_cast<TH1D*>(histogram) -> Draw("HIST");
			}
			// TText label;
			// CanvasExtras::setLabelStyleNote(label);
			canvas -> Update();
			{
				std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
				std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
				canvas -> SaveAs((std::string(PLOTS_SAVE_DIRECTORY) + "/" + epsFilename).c_str());
			}
		}
		gErrorIgnoreLevel = kPrint;
	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << "While saving histograms: " << e.what() << " exception occured." << std::endl;
		exit(-1);
	}
	std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
	theApp->Run();
	return 0;
}
catch(const std::exception& e)
{
	std::cout << error_prompt << "Exception thrown: " << e.what() << std::endl;
	return -1;
}

void testSaveFolders()
{
	auto checkDirectory = [] (const char* directoryName)
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
	checkDirectory(PLOTS_SAVE_DIRECTORY);
}

void readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain)
{
	std::string inputFilesListPath = config[configKey];
	JSON inputListJSON = JSON::parse(fileToString(inputFilesListPath));
	std::vector<std::string> inputFiles = inputListJSON[innerKey];
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
		if(type == "TH1D") histograms.insert({name, std::make_shared<TH1D>(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax)});
		if(type == "TH2D")
		{
			int nbinsy        = checkGetElement<int>(definition, "nbinsy");
			float yMin        = checkGetElement<float>(definition, "yMin");
			float yMax        = checkGetElement<float>(definition, "yMax");
			histograms.insert({name, std::make_shared<TH2D>(name.c_str(),  title.c_str(),  nbinsx, xMin, xMax, nbinsy, yMin, yMax)});
		}
	}
	return histograms;
}
