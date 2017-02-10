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

constexpr int                  ZEROBIAS_TRIGGER_BIT               = 0;
constexpr int                  ZEROBIAS_BITMASK                   = 1 << ZEROBIAS_TRIGGER_BIT;
constexpr int                  VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL  = 10;
constexpr int                  TRACK_QUALITY_HIGH_PURITY_BIT      = 2;
constexpr int                  TRACK_QUALITY_HIGH_PURITY_MASK     = 1 << TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr float                TRACK_PT_CUT_N_MINUS_1_VAL         = 1.0f;
constexpr int                  TRACK_NSTRIP_CUT_N_MINUS_1_VAL     = 10;
constexpr std::array<float, 4> TRACK_D0_CUT_BARREL_N_MINUS_1_VAL  = {0.01f, 0.02f, 0.02f, 0.02f};
constexpr float                TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL = 0.05f;
constexpr float                TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  = 0.0f;
constexpr float                TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL = 0.5f;
constexpr float                MEAS_HITSEP_CUT_N_MINUS_1_VAL     = 0.5f;
constexpr float                HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL = 0.05f;

const bool                                 TRAJ_LOOP_REQUESTED    = true;

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
	TTree* trajTree  = (TTree*)(inputFile -> Get("trajTree" ));
	TTreeTools::treeCheck(trajTree, "Tree missing: trajTree.", true);
	TrajMeasurement trajField;
	EventData       eventField;
	// Trajectory measurement tree
	TTreeTools::checkGetBranch(trajTree, "event")   -> SetAddress(&eventField);
	TTreeTools::checkGetBranch(trajTree, "mod")     -> SetAddress(&(trajField.mod));
	TTreeTools::checkGetBranch(trajTree, "mod_on")  -> SetAddress(&(trajField.mod_on));
	TTreeTools::checkGetBranch(trajTree, "clust")   -> SetAddress(&(trajField.clu));
	TTreeTools::checkGetBranch(trajTree, "track")   -> SetAddress(&(trajField.trk));
	TTreeTools::checkGetBranch(trajTree, "traj")    -> SetAddress(&trajField);
	Int_t trajTreeNumEntries  = trajTree  -> GetEntries();
	// Check if data is present
	std::cout << debug_prompt << "Total entries in the trajTree tree: " << trajTreeNumEntries << std::endl;
	if(trajTreeNumEntries == 0 ) throw std::runtime_error("No entries found in tree: trajTree.");
	// Histogram definitions
	std::vector<std::shared_ptr<TH1>> histograms;
	// N-1 cut tuning
	using Efficiency1DHistoPair = std::pair<std::shared_ptr<TH1D>, std::shared_ptr<TH1D>>;
	using Efficiency2DHistoPair = std::pair<std::shared_ptr<TH2D>, std::shared_ptr<TH2D>>;
	Efficiency1DHistoPair nMinus1Pt =  
	{
		std::make_shared<TH1D>("nMinus1PtNumhits",       "pt [with other effcuts]",                   100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1PtEfficiency",    "eff. vs pt [with other effcuts]",           100, 0.0, 10.0)
	};
	Efficiency1DHistoPair nMinus1Striphits = 
	{
		std::make_shared<TH1D>("nMinus1stripNumhits",    "striphits [with other effcuts]",            100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1stripEfficiency", "eff. vs n. striphits [with other effcuts]", 100, 0.0, 10.0)
	};
	std::array<Efficiency1DHistoPair, 2> nMinus1D0 = 
	{{
		{
			std::make_shared<TH1D>("nMinus1D0BarrelNumhits",    "d0 of barrel hits [with other effcuts]",  200, 0.0, 3.0),
			std::make_shared<TH1D>("nMinus1D0BarrelEfficiency",  "eff. vs d0 of barrel hits [with other effcuts]" , 200, 0.0, 3.0)
		},
		{
			std::make_shared<TH1D>("nMinus1D0ForwardNumhits",    "d0 of forward hits [with other effcuts]", 200, 0.0, 3.0),
			std::make_shared<TH1D>("nMinus1D0ForwardEfficiency", "eff. vs d0 of forward hits [with other effcuts]", 200, 0.0, 3.0)
		}
	}};
	std::array<Efficiency1DHistoPair, 2> nMinus1DZ = 
	{{
		{
			std::make_shared<TH1D>("nMinus1DZBarrelNumhits",     "dz of barrel hits [with other effcuts]",  200, 0.0, 1.0),
			std::make_shared<TH1D>("nMinus1DZBarrelEfficiency",  "eff. vs dz of barrel hits [with other effcuts]",  200, 0.0, 1.0)
		},
		{
			std::make_shared<TH1D>("nMinus1DZForwardNumhits",    "dz of forward hits [with other effcuts]", 200, 0.0, 50.0),
			std::make_shared<TH1D>("nMinus1DZForwardEfficiency", "eff. vs dz of forward hits [with other effcuts]", 200, 0.0, 50.0)
		}
	}};
	Efficiency1DHistoPair nMinus1LX =
	{
		std::make_shared<TH1D>("nMinus1LXNumhits",         "lx [with other effcuts]",                                              100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1LXEfficiency",      "eff. vs lx [with other effcuts]",                                      100, 0.0, 10.0)
	};
	Efficiency1DHistoPair nMinus1LY =
	{
		std::make_shared<TH1D>("nMinus1LyNumhits",         "ly [with other effcuts]",                                              100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1LyEfficiency",      "eff. vs ly [with other effcuts]",                                      100, 0.0, 10.0)
	};
	std::array<Efficiency2DHistoPair, 8> nMinus1LocalPosDist = 
	{{
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsLay1",         "Num. of hits vs hit position lay1 [with other effcuts]",     200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyLay1",      "Eff. vs hit position lay1 [with other effcuts]",             200, -1.0, 1.0, 200, -4.0, 4.0) },
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsLay2",         "Num. of hits vs hit position lay2 [with other effcuts]",     200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyLay2",      "Eff. vs hit position lay2 [with other effcuts]",             200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsLay3",         "Num. of hits vs hit position lay3 [with other effcuts]",     200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyLay3",      "Eff. vs hit position lay3 [with other effcuts]",             200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsLay4",         "Num. of hits vs hit position lay4 [with other effcuts]",     200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyLay4",      "Eff. vs hit position lay4 [with other effcuts]",             200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsForward1",     "Num. of hits vs hit position endcap 1 [with other effcuts]", 200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyForward1",  "Eff. vs hit position endcap 1 [with other effcuts]",         200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsForward2",     "Num. of hits vs hit position endcap 2 [with other effcuts]", 200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyForward2",  "Eff. vs hit position endcap 2 [with other effcuts]",         200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsForward3",     "Num. of hits vs hit position endcap 3 [with other effcuts]", 200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyForward3",  "Eff. vs hit position endcap 3 [with other effcuts]",         200, -1.0, 1.0, 200, -4.0, 4.0)
		},
		{
			std::make_shared<TH2D>("nMinus1LocalPosNumhitsForward4",     "Num. of hits vs hit position endcap 4 [with other effcuts]", 200, -1.0, 1.0, 200, -4.0, 4.0),
			std::make_shared<TH2D>("nMinus1LocalPosEfficiencyForward4",  "Eff. vs hit position endcap 4 [with other effcuts]",         200, -1.0, 1.0, 200, -4.0, 4.0)
		}
	}};
	Efficiency1DHistoPair nMinus1HitDist = 
	{
		std::make_shared<TH1D>("nMinus1HitDistNumhits",    "Closest hit distance to track [with other effcuts]",                   100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1HitDistEfficiency", "eff. vs Closest hit distance to track [with other effcuts]",           100, 0.0, 10.0)
	};
	Efficiency1DHistoPair nMinus1CluDist = 
	{
		std::make_shared<TH1D>("nMinus1CluDistNumhits",    "Closest cluster distance to traj. meas. [with other effcuts]",         100, 0.0, 10.0),
		std::make_shared<TH1D>("nMinus1CluDistEfficiency", "eff. vs Closest cluster distance to traj. meas. [with other effcuts]", 100, 0.0, 10.0)
	};
	histograms.push_back(nMinus1Pt.first);
	histograms.push_back(nMinus1Pt.second);
	histograms.push_back(nMinus1Striphits.first);
	histograms.push_back(nMinus1Striphits.second);
	histograms.push_back(nMinus1D0[0].first);
	histograms.push_back(nMinus1D0[1].second);
	histograms.push_back(nMinus1D0[0].first);
	histograms.push_back(nMinus1D0[1].second);
	histograms.push_back(nMinus1DZ[0].first);
	histograms.push_back(nMinus1DZ[1].second);
	histograms.push_back(nMinus1DZ[0].first);
	histograms.push_back(nMinus1DZ[1].second);
	histograms.push_back(nMinus1LX.first);
	histograms.push_back(nMinus1LX.second);
	histograms.push_back(nMinus1LY.first);
	histograms.push_back(nMinus1LY.second);
	std::for_each(nMinus1LocalPosDist.begin(), nMinus1LocalPosDist.end(), [&] (auto histoPair) { histograms.push_back(histoPair.first ); });
	std::for_each(nMinus1LocalPosDist.begin(), nMinus1LocalPosDist.end(), [&] (auto histoPair) { histograms.push_back(histoPair.second); });
	histograms.push_back(nMinus1HitDist.first);
	histograms.push_back(nMinus1HitDist.second);
	histograms.push_back(nMinus1CluDist.first);
	histograms.push_back(nMinus1CluDist.second);
	// Trajector measurement loop, scoping local variables
	if(TRAJ_LOOP_REQUESTED)
	{
		timer.restart("Measuring the time required for looping on the trajectory measurements...");
		const int& det       = trajField.mod_on.det;
		const int& layer     = trajField.mod_on.layer;
		const int& flipped   = trajField.mod_on.flipped;
		const int& disk      = trajField.mod_on.disk;
		const int& blade     = trajField.mod_on.blade;
		const int& panel     = trajField.mod_on.panel;
		const int& ring      = trajField.mod_on.ring;
		const int& missing   = trajField.missing;
		const TrackData& trk = trajField.trk;
		int nvtxCut_counter = 0, zerobiasCut_counter = 0, federrCut_counter = 0, hpCut_counter = 0, ptCut_counter = 0, nstripCut_counter = 0, d0Cut_counter = 0, dzCut_counter = 0, pixhitCut_counter = 0, lxFidCut_counter = 0, lyFidCut_counter = 0, valmisCut_counter = 0, hitsepCut_counter = 0;
		for(int entryIndex = 0; entryIndex < trajTreeNumEntries; ++entryIndex)
		{
			trajTree -> GetEntry(entryIndex);
			int nvtxCut = 0, zerobiasCut = 0, federrCut = 0, hpCut = 0, ptCut = 0, nstripCut = 0, d0Cut = 0, dzCut = 0, pixhitCut = 0, lxFidCut = 0, lyFidCut = 0, valmisCut = 0, hitsepCut = 0;
			// Nvtx cut
			// nvtxCut = 0 <= eventField.nvtx;
			nvtxCut = trk.fromVtxNtrk <= VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL;
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
			if(det == 0) d0Cut = std::abs(trk.d0) < TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer - 1];
			if(det == 1) d0Cut = std::abs(trk.d0) < TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
			// Dz cut
			if(det == 0) dzCut = std::abs(trk.dz) < TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
			if(det == 1) dzCut = std::abs(trk.dz) < TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
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
			// std::cout << "d0: " << std::abs(trk.d0);
			// std::cout << "dz: " << std::abs(trk.dz);
			// std::cin.get();
			if(nvtxCut && zerobiasCut && federrCut && hpCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				nMinus1Pt.first -> Fill(trk.pt);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1Pt.second -> Fill(trk.pt);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				nMinus1Striphits.first -> Fill(trk.strip);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1Striphits.second -> Fill(trk.strip);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				nMinus1D0[det].first -> Fill(trk.d0);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1D0[det].second -> Fill(trk.d0);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				nMinus1DZ[det].first -> Fill(trk.dz);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1DZ[det].second -> Fill(trk.dz);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && valmisCut && hitsepCut)
			{
				nMinus1LX.first -> Fill(trajField.lx);
				nMinus1LY.first -> Fill(trajField.ly);
				if(det == 0 && !flipped)
				{
					nMinus1LocalPosDist[layer - 1].first -> Fill(trajField.lx, trajField.ly);
				}
				if(det == 1)
				{
					if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 0) nMinus1LocalPosDist[4].first -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 1) nMinus1LocalPosDist[5].first -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 0) nMinus1LocalPosDist[6].first -> Fill(trajField.lx, trajField.ly);
					if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 1) nMinus1LocalPosDist[7].first -> Fill(trajField.lx, trajField.ly);
				}
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1LX.second -> Fill(trajField.lx);
					nMinus1LY.second -> Fill(trajField.ly);
					if(det == 0 && !flipped)
					{
						nMinus1LocalPosDist[layer - 1].second -> Fill(trajField.lx, trajField.ly);
					}
					if(det == 1)
					{
						if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 0) nMinus1LocalPosDist[4].first -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 0 && panel % 2 == 1) nMinus1LocalPosDist[5].first -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 0) nMinus1LocalPosDist[6].first -> Fill(trajField.lx, trajField.ly);
						if(ring == 1 && std::abs(blade % 2) == 1 && panel % 2 == 1) nMinus1LocalPosDist[7].first -> Fill(trajField.lx, trajField.ly);
					}
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut)
			{
				nMinus1HitDist.first -> Fill(trajField.d_tr);
				if(!(missing && HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL < trajField.d_cl))
				{
					nMinus1HitDist.second -> Fill(trajField.d_tr);
				}
			}
			if(nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut)
			{
				nMinus1CluDist.first -> Fill(trajField.d_cl);
				if(!missing) 
				{
					nMinus1CluDist.second -> Fill(trajField.d_cl);
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
		std::cout << "nvtxCut:     " << nvtxCut_counter     << std::endl;
		std::cout << "zerobiasCut: " << zerobiasCut_counter << std::endl;
		std::cout << "federrCut:   " << federrCut_counter   << std::endl;
		std::cout << "hpCut:       " << hpCut_counter       << std::endl;
		std::cout << "ptCut:       " << ptCut_counter       << std::endl;
		std::cout << "nstripCut:   " << nstripCut_counter   << std::endl;
		std::cout << "d0Cut:       " << d0Cut_counter       << std::endl;
		std::cout << "dzCut:       " << dzCut_counter       << std::endl;
		std::cout << "pixhitCut:   " << pixhitCut_counter   << std::endl;
		std::cout << "lxFidCut:    " << lxFidCut_counter    << std::endl;
		std::cout << "lyFidCut:    " << lyFidCut_counter    << std::endl;
		std::cout << "valmisCut:   " << valmisCut_counter   << std::endl;
		std::cout << "hitsepCut:   " << hitsepCut_counter   << std::endl;
		std::cout << " --- End cut counter values --- " << std::endl;
		timer.printSeconds("Loop done. Took about: ", " second(s).");
	}
	// Downscaling efficiency histograms
	auto downscale1DHistogram = [] (const std::shared_ptr<TH1D>& toDownscale, const std::shared_ptr<TH1D>& downscaleFactors)
	{
		const auto& nBins = toDownscale -> GetNbinsX();
		if(nBins != downscaleFactors -> GetNbinsX()) throw std::runtime_error("Error downscaling histograms: bin numbers do not match.");
		for(int binNum = 0; binNum < nBins; ++binNum)
		{
			const auto& binDownscaleFactor = downscaleFactors -> GetBinContent(binNum); 
			const auto& originalBinContent = toDownscale      -> GetBinContent(binNum);
			if(binDownscaleFactor == 0) continue;
			std::cout << "Efficiency bin content before scaling: " << originalBinContent << std::endl;
			toDownscale -> SetBinContent(binNum, originalBinContent / binDownscaleFactor); 
			std::cout << "Efficiency bin content after scaling : " << toDownscale -> GetBinContent(binNum) << std::endl;
		}
	};
	auto downscale2DHistogram = [] (std::shared_ptr<TH2D>& toDownscale, std::shared_ptr<TH2D>& downscaleFactors)
	{
		const auto& nBinsX = toDownscale -> GetNbinsX();
		const auto& nBinsY = toDownscale -> GetNbinsY();
		if(nBinsX != downscaleFactors -> GetNbinsX() || nBinsY != downscaleFactors -> GetNbinsX()) throw std::runtime_error("Error downscaling histograms: bin numbers do not match.");
		for(int binXNum = 0; binXNum < nBinsX; ++binXNum)
		{
			for(int binYNum = 0; binYNum < nBinsY; ++binYNum)
			{
				const auto& binDownscaleFactor = downscaleFactors -> GetBinContent(binXNum, binYNum); 
				const auto& originalBinContent = toDownscale      -> GetBinContent(binXNum, binYNum);
				if(binDownscaleFactor == 0) continue;
				toDownscale -> SetBinContent(binXNum, binYNum, originalBinContent / binDownscaleFactor);
			}
		}
	};
	auto downscale1DEfficiencyPair = [&] (Efficiency1DHistoPair& efficiencyPair) { downscale1DHistogram(efficiencyPair.second, efficiencyPair.first); };
	auto downscale2DEfficiencyPair = [&] (Efficiency2DHistoPair& efficiencyPair) { downscale2DHistogram(efficiencyPair.second, efficiencyPair.first); };
	downscale1DEfficiencyPair(nMinus1Pt);
	downscale1DEfficiencyPair(nMinus1Striphits);
	downscale1DEfficiencyPair(nMinus1D0[0]);
	downscale1DEfficiencyPair(nMinus1D0[1]);
	downscale1DEfficiencyPair(nMinus1DZ[0]);
	downscale1DEfficiencyPair(nMinus1DZ[1]);
	std::for_each(nMinus1LocalPosDist.begin(), nMinus1LocalPosDist.end(), [&] (auto histoPair) { downscale2DEfficiencyPair(histoPair); });
	downscale1DEfficiencyPair(nMinus1LX);
	downscale1DEfficiencyPair(nMinus1LY);
	downscale1DEfficiencyPair(nMinus1HitDist);
	downscale1DEfficiencyPair(nMinus1CluDist);
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
