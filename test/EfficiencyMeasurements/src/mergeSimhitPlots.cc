// Descriptions:
// Reads in an Ntuple created by running the PhaseIPixelNtuplizer and creates
// the main plots required for efficiency measurements.
// List of the plots generated:
//  - 
//  -
//  - (nothing else yet...)

// Data structure
#include "../../../interface/DataStructures_v4.h"

// Hitt efficiency calculation
#include "../../../interface/PhaseITrackingEfficiencyFilters.h"

// Utility
#include "../interface/TestsCommon.h"
#include "../interface/TTreeTools.h"
#include "../interface/HelperFunctionsCommon.h"
#include "../interface/CanvasExtras.h"
#include "../interface/TimerColored.h"
#include "../interface/ProgressBar.h"
#include "../interface/common_functions_jkarancs.h"

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

constexpr std::pair<float, float> EFFICIENCY_ZOOM_RANGE              = {0.97, 1.01};

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                    CONFIG_FILE_PATH            = "./config_main.json"; 
const     std::string             EFFICIENCY_PLOT_IDENTIFIER      = "Efficiency";
const     std::string             EFFICIENCY_NUMERATOR_IDENTIFIER = "Numhits";

const bool CLUST_LOOP_REQUESTED = true;
const bool TRAJ_LOOP_REQUESTED  = true;

void                                        testSaveFolders(const JSON& config);
std::vector<std::string>                    getFilesFromConfig(const JSON& config, const std::string& configKey, const std::string& innerKey);
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);

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
	// TApplication* theApp = new TApplication("App", &argc, argv);
	gROOT->SetBatch(kFALSE);
	EventData       eventField;
	Cluster         clusterField;
	TrajMeasurement trajField;
	// Histogram definitions
	TH2D simhitsPhiVsZ_fwd("simhitsPhiVsZ_fwd", "simhit occupancy, phi vs z - forward", 112, -52.15, 52.15,  50,   0.0, 3.14159);
	TH2D simhitsPhiVsZ_l1 ("simhitsPhiVsZ_l1",  "simhit occupancy, phi vs z - layer 1", 72,  -26.7,  26.7,   50,   0.0, 3.14159);
	TH2D simhitsPhiVsZ_l2 ("simhitsPhiVsZ_l2",  "simhit occupancy, phi vs z - layer 2", 72,  -26.7,  26.7,   50,   0.0, 3.14159);
	TH2D simhitsPhiVsZ_l3 ("simhitsPhiVsZ_l3",  "simhit occupancy, phi vs z - layer 3", 72,  -26.7,  26.7,   50,   0.0, 3.14159);
	TH2D simhitsPhiVsZ_l4 ("simhitsPhiVsZ_l4",  "simhit occupancy, phi vs z - layer 4", 72,  -26.7,  26.7,   50,   0.0, 3.14159);
	// Merge the histograms
	std::vector<std::string> filenames = getFilesFromConfig(config, "input_files_list", "input_files");
	for(const auto& filename: filenames)
	{
		TFile* source = TFile::Open(filename.c_str(), "READ");
		simhitsPhiVsZ_fwd.Add(dynamic_cast<TH2D*>(source -> Get("simhitOccupancy_fwd")));
		simhitsPhiVsZ_l1 .Add(dynamic_cast<TH2D*>(source -> Get("simhitOccupancy_l1")));
		simhitsPhiVsZ_l2 .Add(dynamic_cast<TH2D*>(source -> Get("simhitOccupancy_l2")));
		simhitsPhiVsZ_l3 .Add(dynamic_cast<TH2D*>(source -> Get("simhitOccupancy_l3")));
		simhitsPhiVsZ_l4 .Add(dynamic_cast<TH2D*>(source -> Get("simhitOccupancy_l4")));
	}
	////////////////
	// Save plots //
	////////////////
	gROOT  -> SetBatch(kTRUE); // Uncomment to check the plots on-the fly
	// constexpr int PHASE_SCENARIO = 1;
	gStyle -> SetPalette(1);
	gStyle -> SetNumberContours(999);
	gStyle -> SetOptStat(1111);
	gErrorIgnoreLevel = kError;
	// histogram.SetTitleSize(22);
	std::vector<TH2D*> histograms = { &simhitsPhiVsZ_fwd, &simhitsPhiVsZ_l1, &simhitsPhiVsZ_l2, &simhitsPhiVsZ_l3, &simhitsPhiVsZ_l4 };
	for(const auto& histogram: histograms)
	{
		const std::string& histogramName = histogram -> GetName();
		TCanvas* canvas;
		int histoSizeX = 800;
		int histoSizeY = 800;
		canvas = custom_can_(histogram, histogramName + "_canvas", 0, 0, histoSizeX, histoSizeY, 80, 140);
		canvas -> cd();
		TH2D* histo2D = dynamic_cast<TH2D*>(histogram);
		if(histo2D != nullptr)
		{
			histo2D -> Draw("COLZ");
			canvas -> Update();
			TPaveStats* stats = (TPaveStats*) histo2D -> GetListOfFunctions() -> FindObject("stats");
			if(stats) stats -> Draw("SAME");
			else std::cout << error_prompt << "Cannot redraw stats box for " << histo2D -> GetName() << ": no stats found. " << std::endl;
		}
		canvas -> Update();
		{
			std::string epsFilename = (std::string(canvas -> GetTitle()) + ".eps");
			std::transform(epsFilename.begin(), epsFilename.end(), epsFilename.begin(), [] (char ch) { return ch == ' ' ? '_' : ch; });
			std::string savePath = config["plots_save_directory"].get<std::string>() + "/" + epsFilename;
			canvas -> SaveAs(savePath.c_str());
		}
	}
	gErrorIgnoreLevel = kPrint;
	std::cout << process_prompt << argv[0] << " terminated succesfully." << std::endl;
	std::cin.get();	
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
