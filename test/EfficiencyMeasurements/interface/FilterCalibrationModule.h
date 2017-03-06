#pragma once

// Data structure
#include "../../../interface/DataStructures_v3.h"
#include "../interface/TimerColored.h"
#include "../interface/CommonActors.h"

#include "TH2D.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <memory>

class FilterCalibrationModule
{
	using HistoMapType       = std::map<std::string, std::shared_ptr<TH1>>;
	private:
		static constexpr int                  ZEROBIAS_TRIGGER_BIT               = 0;
		static constexpr int                  ZEROBIAS_BITMASK                   = 1 << ZEROBIAS_TRIGGER_BIT;
		static constexpr int                  VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL  = 10;
		static constexpr int                  TRACK_QUALITY_HIGH_PURITY_BIT      = 2;
		static constexpr int                  TRACK_QUALITY_HIGH_PURITY_MASK     = 1 << TRACK_QUALITY_HIGH_PURITY_BIT;
		static constexpr float                TRACK_PT_CUT_N_MINUS_1_VAL         = 1.0f;
		static constexpr int                  TRACK_NSTRIP_CUT_N_MINUS_1_VAL     = 10;
		static constexpr std::array<float, 4> TRACK_D0_CUT_BARREL_N_MINUS_1_VAL  = {0.01f, 0.02f, 0.02f, 0.02f};
		static constexpr float                TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL = 0.05f;
		static constexpr float                TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  = 0.01f;
		static constexpr float                TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL = 0.5f;
		static constexpr float                MEAS_HITSEP_CUT_N_MINUS_1_VAL      = 0.01f;
		static constexpr float                HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL   = 0.005f;

		HistoMapType&          histograms_;
		const EventData&       eventField_;
		const TrajMeasurement& trajField_;

		// Shortcuts
		const int&       det_;
		const int&       layer_;
		const int&       flipped_;
		const int&       disk_;
		const int&       blade_;
		const int&       panel_;
		const int&       ring_;
		const int&       missing_;
		const float&     ladderCoord_;
		const float&     moduleCoord_;
		const float&     bladePanelCoord_;
		const float&     diskRingCoord_;
		const TrackData& trk_;
		// Cut values
		int nvtxCut     = 0;
		int zerobiasCut = 0;
		int federrCut   = 0;
		int hpCut       = 0;
		int ptCut       = 0;
		int nstripCut   = 0;
		int d0Cut       = 0;
		int dzCut       = 0;
		int pixhitCut   = 0;
		int lxFidCut    = 0;
		int lyFidCut    = 0;
		int valmisCut   = 0;
		int hitsepCut   = 0;
		int effCutAll   = 0;
		// Counters
		int entry_counter_       = 0;
		int nvtxCut_counter_     = 0;
		int zerobiasCut_counter_ = 0;
		int federrCut_counter_   = 0;
		int hpCut_counter_       = 0;
		int ptCut_counter_       = 0;
		int nstripCut_counter_   = 0;
		int d0Cut_counter_       = 0;
		int dzCut_counter_       = 0;
		int pixhitCut_counter_   = 0;
		int lxFidCut_counter_    = 0;
		int lyFidCut_counter_    = 0;
		int valmisCut_counter_   = 0;
		int hitsepCut_counter_   = 0;
		int effCutAll_counter_   = 0;
		// Key list (for checks)
		const std::vector<std::string> histogramKeyList_ = 
		{
			"nMinus1VtxNtrkNumhits",             "nMinus1VtxNtrkEfficiency",         
			"nMinus1PtNumhits",                  "nMinus1PtEfficiency",              
			"nMinus1stripNumhits",               "nMinus1stripEfficiency",           
			"nMinus1LxNumhits",                  "nMinus1LxEfficiency",              
			"nMinus1LyNumhits",                  "nMinus1LyEfficiency",              
			"nMinus1HitDistNumhits",             "nMinus1HitDistEfficiency",         
			"nMinus1CluDistNumhits",             "nMinus1CluDistEfficiency",         
			"nMinus1D0BarrelNumhits",            "nMinus1D0BarrelEfficiency",         "nMinus1D0ForwardNumhits",           "nMinus1D0ForwardEfficiency",
			"nMinus1DZBarrelNumhits",            "nMinus1DZBarrelEfficiency",         "nMinus1DZForwardNumhits",           "nMinus1DZForwardEfficiency",
			"nMinus1LocalPosNumhitsLay1",        "nMinus1LocalPosNumhitsLay2",        "nMinus1LocalPosNumhitsLay3",        "nMinus1LocalPosNumhitsLay4",
			"nMinus1LocalPosEfficiencyLay1",     "nMinus1LocalPosEfficiencyLay2",     "nMinus1LocalPosEfficiencyLay3",     "nMinus1LocalPosEfficiencyLay4",
			"nMinus1LocalPosNumhitsForward1",    "nMinus1LocalPosNumhitsForward2",    "nMinus1LocalPosNumhitsForward3",    "nMinus1LocalPosNumhitsForward4",
			"nMinus1LocalPosEfficiencyForward1", "nMinus1LocalPosEfficiencyForward2", "nMinus1LocalPosEfficiencyForward3", "nMinus1LocalPosEfficiencyForward4"
		};
	public:
		FilterCalibrationModule(HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
		~FilterCalibrationModule() = default;
		void checkHistogramDependencies();
		void fillFilterHistograms();
		void printCounters();
		void printCutValues();
	private:
		void fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts = {});
		void fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts = {});
		void calculateCuts();
		void incrementCounters();
};