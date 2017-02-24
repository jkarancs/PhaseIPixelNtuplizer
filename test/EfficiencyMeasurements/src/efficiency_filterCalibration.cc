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

#include "../interface/json.hpp"
using JSON = nlohmann::json;

constexpr float HALF_PI = 0.5 * 3.141592653589793238462;

constexpr int                  ZEROBIAS_TRIGGER_BIT               = 0;
constexpr int                  ZEROBIAS_BITMASK                   = 1 << ZEROBIAS_TRIGGER_BIT;
constexpr int                  VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL  = 10;
constexpr int                  TRACK_QUALITY_HIGH_PURITY_BIT      = 2;
constexpr int                  TRACK_QUALITY_HIGH_PURITY_MASK     = 1 << TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr float                TRACK_PT_CUT_N_MINUS_1_VAL         = 1.0f;
constexpr int                  TRACK_NSTRIP_CUT_N_MINUS_1_VAL     = 10;
constexpr std::array<float, 4> TRACK_D0_CUT_BARREL_N_MINUS_1_VAL  = {0.01f, 0.02f, 0.02f, 0.02f};
constexpr float                TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL = 0.05f;
constexpr float                TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  = 0.01f;
constexpr float                TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL = 0.5f;
constexpr float                MEAS_HITSEP_CUT_N_MINUS_1_VAL      = 0.5f;
constexpr float                HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL   = 0.05f;
constexpr auto                 PLOTS_SAVE_DIRECTORY               = "./results";

constexpr std::array<int, 4>   LAYER_NUM_LADDERS{ 6, 14, 22, 32 };

// const std::pair<float, float>  LAYER_MODULE_LABEL_POS      = std::make_pair(0.79f, 0.88f);
constexpr auto                     CONFIG_FILE_PATH            = "./config_main.json"; 

const bool TRAJ_LOOP_REQUESTED    = true;

void                                        testSaveFolders();
void                                        readInFilesAndAddToChain(const JSON& config, const std::string& configKey, const std::string& innerKey, TChain* chain);
std::map<std::string, std::shared_ptr<TH1>> processHistogramDefinitions(const JSON& config, const std::string& configKey, const std::string& innerKey);

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
	TChain* trajTreeChain = new TChain("trajTree", "List of the trajectory measurements.");
	readInFilesAndAddToChain(config, "input_files_list", "input_files", trajTreeChain);
	TrajMeasurement trajField;
	EventData       eventField;
	// Trajectory measurement tree
	trajTreeChain -> SetBranchAddress("event",  &eventField);
	trajTreeChain -> SetBranchAddress("mod",    &(trajField.mod));
	trajTreeChain -> SetBranchAddress("mod_on", &(trajField.mod_on));
	trajTreeChain -> SetBranchAddress("clust",  &(trajField.clu));
	trajTreeChain -> SetBranchAddress("track",  &(trajField.trk));
	trajTreeChain -> SetBranchAddress("traj",   &trajField);
	// trajTreeChain -> Draw("d_cl >> hsqrt(1000, 0.0, 500.0)");
	// std::cout << "Running the app." << std::endl;
	// theApp -> Run();
	Long64_t trajTreeNumEntries  = trajTreeChain  -> GetEntries();
	// Check if data is present
	std::cout << debug_prompt << "Total entries in the trajTree tree: " << trajTreeNumEntries << std::endl;
	if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
	// Histogram definitions
	std::map<std::string, std::shared_ptr<TH1>> histograms(processHistogramDefinitions(config, "histogram_definition_list", "histogram_definitions"));
	////////////////////////////////
	// Trajector measurement loop //
	////////////////////////////////
	try
	{
		timer.restart("Measuring the time required for looping on the trajectory measurements...");
		const int&   det             = trajField.mod_on.det;
		const int&   layer           = trajField.mod_on.layer;
		const int&   flipped         = trajField.mod_on.flipped;
		const int&   disk            = trajField.mod_on.disk;
		const int&   blade           = trajField.mod_on.blade;
		const int&   panel           = trajField.mod_on.panel;
		const int&   ring            = trajField.mod_on.ring;
		const int&   missing         = trajField.missing;
		const float& ladderCoord     = trajField.mod_on.ladder_coord;
		const float& moduleCoord     = trajField.mod_on.module_coord;
		const float& bladePanelCoord = trajField.mod_on.blade_panel_coord;
		const float& diskRingCoord   = trajField.mod_on.disk_ring_coord;
		const TrackData& trk         = trajField.trk;
		int nvtxCut_counter = 0, zerobiasCut_counter = 0, federrCut_counter = 0, hpCut_counter = 0, ptCut_counter = 0, nstripCut_counter = 0, d0Cut_counter = 0, dzCut_counter = 0, pixhitCut_counter = 0, lxFidCut_counter = 0, lyFidCut_counter = 0, valmisCut_counter = 0, hitsepCut_counter = 0;
		for(Long64_t entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
		{
			trajTreeChain -> GetEntry(entryIndex);
			if(det == 0) switch(layer)
			{
				case 1:
					histograms.at("OnTrkCluOccupancy_l1")  -> Fill(moduleCoord, ladderCoord);
					break;
				case 2:
					histograms.at("OnTrkCluOccupancy_l2")  -> Fill(moduleCoord, ladderCoord);
					break;
				case 3:
					histograms.at("OnTrkCluOccupancy_l3")  -> Fill(moduleCoord, ladderCoord);
					break;
				case 4:
					histograms.at("OnTrkCluOccupancy_l4")  -> Fill(moduleCoord, ladderCoord);
					break;
				default:
					std::cout << error_prompt << "Wrong layer number on barrel. Check the code source!" << std::endl; 
			}
			if(det == 1) histograms.at("OnTrkCluOccupancy_fwd") -> Fill(diskRingCoord, bladePanelCoord);
			int nvtxCut = 0, zerobiasCut = 0, federrCut = 0, hpCut = 0, ptCut = 0, nstripCut = 0, d0Cut = 0, dzCut = 0, pixhitCut = 0, lxFidCut = 0, lyFidCut = 0, valmisCut = 0, hitsepCut = 0;
			// Nvtx cut
			// nvtxCut = 0 <= eventField.nvtx;
			// nvtxCut = trk.fromVtxNtrk <= VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL;
			nvtxCut = VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL < trk.fromVtxNtrk;
			// Zerobias cut
			zerobiasCut = eventField.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT;
			// Federr cut
			federrCut = eventField.federrs_size == 0;
			// Hp cut
			hpCut = (trk.quality & TRACK_QUALITY_HIGH_PURITY_MASK) >> TRACK_QUALITY_HIGH_PURITY_BIT;
			// Pt cut
			ptCut = TRACK_PT_CUT_N_MINUS_1_VAL < trk.pt;
			// Nstrip cut
			nstripCut = TRACK_NSTRIP_CUT_N_MINUS_1_VAL < trk.strip;
			// D0 cut
			if(det == 0) d0Cut = TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer - 1] < std::abs(trk.d0);
			if(det == 1) d0Cut = TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL           < std::abs(trk.d0);
			// Dz cut
			if(det == 0) dzCut = TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  < std::abs(trk.dz);
			if(det == 1) dzCut = TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL < std::abs(trk.dz);
			// Pixhit cut
			if(det == 0)
			{
				switch(layer)
				{
					case 1:
						pixhitCut =
							(trk.validbpix[1] > 0 && trk.validbpix[2] > 0 && trk.validbpix[3] > 0) ||
							(trk.validbpix[1] > 0 && trk.validbpix[2] > 0 && trk.validfpix[0] > 0) ||
							(trk.validbpix[1] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0) ||
							(trk.validfpix[0] > 0 && trk.validfpix[2] > 0 && trk.validfpix[2] > 0);
						break;
					case 2:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validbpix[2] > 0 && trk.validbpix[3] > 0) ||
							(trk.validbpix[0] > 0 && trk.validbpix[2] > 0 && trk.validfpix[0] > 0) ||
							(trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0);
						break;
					case 3:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[3] > 0) ||
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[0] > 0);
						break;
					case 4:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[2] > 0);
						break;
				}
			}
			if(det == 1)
			{
				switch(disk)
				{
					case 1:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validbpix[2] > 0) ||
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[1] > 0) ||
							(trk.validbpix[0] > 0 && trk.validfpix[1] > 0 && trk.validfpix[2] > 0);
						break;
					case 2:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validbpix[1] > 0 && trk.validfpix[0] > 0) ||
							(trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[2] > 0);
						break;
					case 3:
						pixhitCut =
							(trk.validbpix[0] > 0 && trk.validfpix[0] > 0 && trk.validfpix[1] > 0);
						break;
				}
			}
			// Fidicual cuts
			lxFidCut = 1;
			lyFidCut = 1;
			// Valmis cut
			valmisCut = trajField.validhit || trajField.missing;
			// Hitsep cut
			hitsepCut = trajField.d_tr < MEAS_HITSEP_CUT_N_MINUS_1_VAL;
			// std::cout << " --- Start cut values --- " << std::endl;
			// std::cout << "nvtxCut:     " << nvtxCut     << std::endl;
			// std::cout << "zerobiasCut: " << zerobiasCut << std::endl;
			// std::cout << "federrCut:   " << federrCut   << std::endl;
			// std::cout << "hpCut:       " << hpCut       << std::endl;
			// std::cout << "ptCut:       " << ptCut       << std::endl;
			// std::cout << "nstripCut:   " << nstripCut   << std::endl;
			// std::cout << "d0Cut:       " << d0Cut       << std::endl;
			// std::cout << "dzCut:       " << dzCut       << std::endl;
			// std::cout << "pixhitCut:   " << pixhitCut   << std::endl;
			// std::cout << "lxFidCut:    " << lxFidCut    << std::endl;
			// std::cout << "lyFidCut:    " << lyFidCut    << std::endl;
			// std::cout << "valmisCut:   " << valmisCut   << std::endl;
			// std::cout << "hitsepCut:   " << hitsepCut   << std::endl;
			// std::cout << " --- End cut values --- " << std::endl;
			// std::cout << "qulatity:    " << trk.quality       << std::endl;
			// std::cout << "fromVtxNtrk: " << trk.fromVtxNtrk   << std::endl;
			// std::cout << "d0:          "  << std::abs(trk.d0) << std::endl;
			// std::cout << "dz:          "  << std::abs(trk.dz) << std::endl;
			// std::cin.get();
			if(zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				histograms.at("nMinus1VtxNtrkNumhits") -> Fill(trk.fromVtxNtrk);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at("nMinus1VtxNtrkEfficiency") -> Fill(trk.fromVtxNtrk);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				histograms.at("nMinus1PtNumhits") -> Fill(trk.pt);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at("nMinus1PtEfficiency") -> Fill(trk.pt);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				histograms.at("nMinus1stripNumhits") -> Fill(trk.strip);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at("nMinus1stripEfficiency") -> Fill(trk.strip);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				static const std::array<std::string, 4> names = {"nMinus1D0BarrelNumhits", "nMinus1D0BarrelEfficiency", "nMinus1D0ForwardNumhits", "nMinus1D0ForwardEfficiency"};
				histograms.at(names[det * 2]) -> Fill(trk.d0);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at(names[det * 2 + 1]) -> Fill(trk.d0);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				static const std::array<std::string, 4> names = {"nMinus1DZBarrelNumhits", "nMinus1DZBarrelEfficiency", "nMinus1DZForwardNumhits", "nMinus1DZForwardEfficiency"};
				histograms.at(names[det * 2]) -> Fill(trk.dz);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at(names[det * 2]) -> Fill(trk.dz);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && valmisCut && hitsepCut)
			{
				histograms.at("nMinus1LXNumhits") -> Fill(trajField.lx);
				histograms.at("nMinus1LyNumhits") -> Fill(trajField.ly);
				if(det == 0 && !flipped)
				{
					static const std::array<std::string, 4> names = {"nMinus1LocalPosNumhitsLay1", "nMinus1LocalPosNumhitsLay2", "nMinus1LocalPosNumhitsLay3", "nMinus1LocalPosNumhitsLay4"};
					histograms.at(names[layer - 1]) -> Fill(trajField.lx, trajField.ly);
				}
				if(det == 1)
				{
					static const std::array<std::string, 4> names = {"nMinus1LocalPosNumhitsForward1", "nMinus1LocalPosNumhitsForward2", "nMinus1LocalPosNumhitsForward3", "nMinus1LocalPosNumhitsForward4"};
					if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 0) histograms.at(names[0]) -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 1) histograms.at(names[1]) -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 0) histograms.at(names[2]) -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 1) histograms.at(names[3]) -> Fill(trajField.lx, trajField.ly);
				}
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at("nMinus1LXEfficiency") -> Fill(trajField.lx);
					histograms.at("nMinus1LyEfficiency") -> Fill(trajField.ly);
					if(det == 0 && !flipped)
					{
						static const std::array<std::string, 4> names = {"nMinus1LocalPosEfficiencyLay1", "nMinus1LocalPosEfficiencyLay2", "nMinus1LocalPosEfficiencyLay3", "nMinus1LocalPosEfficiencyLay4"};
						histograms[names[layer - 1]] -> Fill(trajField.lx, trajField.ly);
					}
					if(det == 1)
					{
						static const std::array<std::string, 4> names = {"nMinus1LocalPosEfficiencyForward1", "nMinus1LocalPosEfficiencyForward2", "nMinus1LocalPosEfficiencyForward3", "nMinus1LocalPosEfficiencyForward4"};
						if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 0) histograms.at(names[0]) -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 1) histograms.at(names[1]) -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 0) histograms.at(names[2]) -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 1) histograms.at(names[3]) -> Fill(trajField.lx, trajField.ly);
					}
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut)
			{
				histograms.at("nMinus1HitDistNumhits") -> Fill(trajField.d_tr);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					histograms.at("nMinus1HitDistEfficiency") -> Fill(trajField.d_tr);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				histograms.at("nMinus1CluDistNumhits") -> Fill(trajField.d_cl);
				if(!missing)
				{
					histograms.at("nMinus1CluDistEfficiency") -> Fill(trajField.d_cl);
				}
			}
			nvtxCut_counter     += nvtxCut;
			zerobiasCut_counter += zerobiasCut;
			federrCut_counter   += federrCut;
			hpCut_counter       += hpCut;
			ptCut_counter       += ptCut;
			nstripCut_counter   += nstripCut;
			d0Cut_counter       += d0Cut;
			dzCut_counter       += dzCut;
			pixhitCut_counter   += pixhitCut;
			lxFidCut_counter    += lxFidCut;
			lyFidCut_counter    += lyFidCut;
			valmisCut_counter   += valmisCut;
			hitsepCut_counter   += hitsepCut;
			// Perform cuts
			// if(nvtxCut)     continue;
			// if(zerobiasCut) continue;
			// if(federrCut)   continue;
			// if(hpCut)       continue;
			// if(ptCut)       continue;
			// if(nstripCut)   continue;
			// if(d0Cut)       continue;
			// if(dzCut)       continue;
			// if(pixhitCut)   continue;
			// if(lxFidCut)    continue;
			// if(lyFidCut)    continue;
			// if(valmisCut)   continue;
			// if(hitsepCut)   continue;
		}
		std::cout << " --- Start counter counters values --- " << std::endl;
		std::cout << "nvtxCut:     " << std::setw(12) << nvtxCut_counter     << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << nvtxCut_counter     * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "zerobiasCut: " << std::setw(12) << zerobiasCut_counter << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << zerobiasCut_counter * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "federrCut:   " << std::setw(12) << federrCut_counter   << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << federrCut_counter   * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "hpCut:       " << std::setw(12) << hpCut_counter       << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << hpCut_counter       * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "ptCut:       " << std::setw(12) << ptCut_counter       << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << ptCut_counter       * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "nstripCut:   " << std::setw(12) << nstripCut_counter   << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << nstripCut_counter   * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "d0Cut:       " << std::setw(12) << d0Cut_counter       << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << d0Cut_counter       * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "dzCut:       " << std::setw(12) << dzCut_counter       << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << dzCut_counter       * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "pixhitCut:   " << std::setw(12) << pixhitCut_counter   << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << pixhitCut_counter   * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "lxFidCut:    " << std::setw(12) << lxFidCut_counter    << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << lxFidCut_counter    * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "lyFidCut:    " << std::setw(12) << lyFidCut_counter    << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << lyFidCut_counter    * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "valmisCut:   " << std::setw(12) << valmisCut_counter   << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << valmisCut_counter   * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << "hitsepCut:   " << std::setw(12) << hitsepCut_counter   << " / " << std::setw(12) << trajTreeNumEntries << " (" << std::setprecision(5) << hitsepCut_counter   * 100.0 / trajTreeNumEntries<< "%)" << std::endl;
		std::cout << " --- End cut counter values --- " << std::endl;
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
		static const std::string efficiencyNumeratorIdentifier = "NumHits";
		for(const auto& histogramPair: histograms)
		{
			std::string efficiencyHistoName = histogramPair.first;
			size_t      effPos              = efficiencyHistoName.find(efficiencyPlotIdentifier);
			if(effPos == std::string::npos)      continue;
			std::string numeratorHistoName  = efficiencyHistoName.replace(effPos, efficiencyPlotIdentifier.size(), efficiencyNumeratorIdentifier);
			auto numHitsHisto               = histograms.find(numeratorHistoName);
			if(numHitsHisto == histograms.end()) continue;
			auto efficiencyHisto2DPtrConversionResult = dynamic_cast<TH2D*>(histogramPair.second.get());
			if(efficiencyHisto2DPtrConversionResult)
			{
				downscale2DHistogram(efficiencyHisto2DPtrConversionResult, dynamic_cast<TH2D*>(numHitsHisto -> second.get()));
				continue;
			}
			downscale1DHistogram(dynamic_cast<TH1D*>(histogramPair.second.get()), dynamic_cast<TH1D*>(numHitsHisto -> second.get()));
		}
	}
	///////////////////////
	// Load plot options //
	///////////////////////
	struct PlotOptions
	{
		int dressPlot = 0;
		int layer     = -1;
	};
	std::map<std::string, PlotOptions> plotOptionsMap;
	try
	{
		std::string histogramDefinitionListPath = config["histogram_definition_list"];
		JSON inputListJSON = JSON::parse(fileToString(histogramDefinitionListPath));
		std::vector<JSON> histogramDefinitionList = inputListJSON["histogram_definitions"];
		for(const auto& definition: histogramDefinitionList)
		{
			PlotOptions plotExtras;
			std::string name = definition.at("name");
			if(definition.count("plot_extras") == 0) 
			{
				plotOptionsMap.emplace(name, std::move(plotExtras));
				continue; 
			}
			// std::cout << debug_prompt << name << " has a key for dress_plot: " << (definition.count("dress_plot") ? "true" : "false") << std::endl;
			if(definition.count("dress_plot") != 0)
			{
				// std::cout << debug_prompt << definition.at("dress_plot") << std::endl;
				plotExtras.dressPlot = definition.at("dress_plot");
			}
			if(definition.count("layer"     ) != 0)
			{
				// std::cout << debug_prompt << definition.at("layer") << std::endl;
				plotExtras.layer     = definition.at("layer");
			}
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
		constexpr int PHASE_SCENARIO = 1;
		gStyle -> SetPalette(1);
		gStyle -> SetNumberContours(999);
		gROOT->SetBatch(kTRUE); // Uncomment to check the plots on-the fly
		gStyle->SetOptStat(1111);
		gErrorIgnoreLevel = kError;
		// histogram.SetTitleSize(22);
		// histogram.GetYaxis() -> SetNdivisions(507);
		for(const auto& histogramPair: histograms)
		{
			const std::string& histogramName = histogramPair.first;
			TH1*               histogram     = histogramPair.second.get();
			const PlotOptions& plotOptions   = plotOptionsMap.at(histogramName);
			TCanvas* canvas = custom_can_(histogram, histogramName + "_canvas", 0, 0, 800, 800, 80, 140);
			canvas -> cd();
			// histogram.SetFillColor(38);
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
					// std::cout << debug_prompt << "Dress occup plot called with layer: " << plotOptions.layer << " and phase scenario: " << PHASE_SCENARIO << "." << std::endl;
				}
			}
			else dynamic_cast<TH1D*>(histogram) -> Draw("BHIST");
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