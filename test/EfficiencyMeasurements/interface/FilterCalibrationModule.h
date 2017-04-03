#pragma once

// Data structure
#include "../../../interface/DataStructures_v4.h"
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

		const EventData&       eventField_;
		const TrajMeasurement& trajField_;

		int before = 0;

		// Shortcuts
		const int&       det_;
		const int&       layer_;
		const int&       flipped_;
		const int&       side_;
		const int&       disk_;
		const int&       blade_;
		const int&       panel_;
		const int&       ring_;
		const int&       missing_;
		const float&     ladderCoord_;
		const float&     moduleCoord_;
		const float&     bladePanelCoord_;
		const float&     diskRingCoord_;
		const float&     lx_;
		const float&     ly_;
		const float&     lz_;
		const float&     glx_;
		const float&     gly_;
		const float&     glz_;
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
		// Histograms
		TH1* layersDisksNumhits;                TH1* layersDisksEfficiency;
		TH1* rechitOccupancy_l1;                TH1* rechitOccupancy_l2;                TH1* rechitOccupancy_l3;                TH1* rechitOccupancy_l4;                TH1* rechitOccupancy_fwd;
		TH1* rocNumhitsWithCuts_l1;             TH1* rocNumhitsWithCuts_l2;             TH1* rocNumhitsWithCuts_l3;             TH1* rocNumhitsWithCuts_l4;             TH1* rocNumhitsWithCuts_fwd;
		TH1* rocEfficiencyWithCuts_l1;          TH1* rocEfficiencyWithCuts_l2;          TH1* rocEfficiencyWithCuts_l3;          TH1* rocEfficiencyWithCuts_l4;          TH1* rocEfficiencyWithCuts_fwd;
		TH1* rechitPhiVsZ_l1;                   TH1* rechitPhiVsZ_l2;                   TH1* rechitPhiVsZ_l3;                   TH1* rechitPhiVsZ_l4;                   TH1* rechitPhiVsZ_fwd;
		TH1* sensorNumhitsWithCutsPhiVsZ_l1;    TH1* sensorNumhitsWithCutsPhiVsZ_l2;    TH1* sensorNumhitsWithCutsPhiVsZ_l3;    TH1* sensorNumhitsWithCutsPhiVsZ_l4;    TH1* sensorNumhitsWithCutsPhiVsZ_fwd;
		TH1* sensorEfficiencyWithCutsPhiVsZ_l1; TH1* sensorEfficiencyWithCutsPhiVsZ_l2; TH1* sensorEfficiencyWithCutsPhiVsZ_l3; TH1* sensorEfficiencyWithCutsPhiVsZ_l4; TH1* sensorEfficiencyWithCutsPhiVsZ_fwd;
		TH1* lxNumhitsPreCuts_fwd;              TH1* lxNumhitsPreCuts_l1;               TH1* lxNumhitsPreCuts_l2;               TH1* lxNumhitsPreCuts_l3;               TH1* lxNumhitsPreCuts_l4;
		TH1* lxNumhitsInnerPreCuts_l1;          TH1* lxNumhitsOuterPreCuts_l1;          TH1* lxNumhitsInnerPreCuts_l2;          TH1* lxNumhitsOuterPreCuts_l2;          TH1* lxNumhitsInnerPreCuts_l3;           TH1* lxNumhitsOuterPreCuts_l3;          TH1* lxNumhitsInnerPreCuts_l4;          TH1* lxNumhitsOuterPreCuts_l4;
		TH1* lxEfficiencyPreCuts_fwd;           TH1* lxEfficiencyPreCuts_l1;            TH1* lxEfficiencyPreCuts_l2;            TH1* lxEfficiencyPreCuts_l3;            TH1* lxEfficiencyPreCuts_l4;
		TH1* lxEfficiencyInnerPreCuts_l1;       TH1* lxEfficiencyOuterPreCuts_l1;       TH1* lxEfficiencyInnerPreCuts_l2;       TH1* lxEfficiencyOuterPreCuts_l2;       TH1* lxEfficiencyInnerPreCuts_l3;        TH1* lxEfficiencyOuterPreCuts_l3;       TH1* lxEfficiencyInnerPreCuts_l4;       TH1* lxEfficiencyOuterPreCuts_l4;
		TH1* lyNumhitsPreCuts_fwd;              TH1* lyNumhitsPreCuts_l1;               TH1* lyNumhitsPreCuts_l2;               TH1* lyNumhitsPreCuts_l3;               TH1* lyNumhitsPreCuts_l4;
		TH1* lyNumhitsInnerPreCuts_l1;          TH1* lyNumhitsOuterPreCuts_l1;          TH1* lyNumhitsInnerPreCuts_l2;          TH1* lyNumhitsOuterPreCuts_l2;          TH1* lyNumhitsInnerPreCuts_l3;           TH1* lyNumhitsOuterPreCuts_l3;          TH1* lyNumhitsInnerPreCuts_l4;          TH1* lyNumhitsOuterPreCuts_l4;
		TH1* lyEfficiencyPreCuts_fwd;           TH1* lyEfficiencyPreCuts_l1;            TH1* lyEfficiencyPreCuts_l2;            TH1* lyEfficiencyPreCuts_l3;            TH1* lyEfficiencyPreCuts_l4;
		TH1* lyEfficiencyInnerPreCuts_l1;       TH1* lyEfficiencyOuterPreCuts_l1;       TH1* lyEfficiencyInnerPreCuts_l2;       TH1* lyEfficiencyOuterPreCuts_l2;       TH1* lyEfficiencyInnerPreCuts_l3;        TH1* lyEfficiencyOuterPreCuts_l3;       TH1* lyEfficiencyInnerPreCuts_l4;       TH1* lyEfficiencyOuterPreCuts_l4;
		TH1* nMinus1LxNumhits_fwd;              TH1* nMinus1LxNumhits_l1;               TH1* nMinus1LxNumhits_l2;               TH1* nMinus1LxNumhits_l3;               TH1* nMinus1LxNumhits_l4;
		TH1* nMinus1LxNumhitsInner_l1;          TH1* nMinus1LxNumhitsOuter_l1;          TH1* nMinus1LxNumhitsInner_l2;          TH1* nMinus1LxNumhitsOuter_l2;          TH1* nMinus1LxNumhitsInner_l3;           TH1* nMinus1LxNumhitsOuter_l3;          TH1* nMinus1LxNumhitsInner_l4;          TH1* nMinus1LxNumhitsOuter_l4;
		TH1* nMinus1LxEfficiency_fwd;           TH1* nMinus1LxEfficiency_l1;            TH1* nMinus1LxEfficiency_l2;            TH1* nMinus1LxEfficiency_l3;            TH1* nMinus1LxEfficiency_l4;
		TH1* nMinus1LxEfficiencyInner_l1;       TH1* nMinus1LxEfficiencyOuter_l1;       TH1* nMinus1LxEfficiencyInner_l2;       TH1* nMinus1LxEfficiencyOuter_l2;       TH1* nMinus1LxEfficiencyInner_l3;        TH1* nMinus1LxEfficiencyOuter_l3;       TH1* nMinus1LxEfficiencyInner_l4;       TH1* nMinus1LxEfficiencyOuter_l4;
		TH1* nMinus1LyNumhits_fwd;              TH1* nMinus1LyNumhits_l1;               TH1* nMinus1LyNumhits_l2;               TH1* nMinus1LyNumhits_l3;               TH1* nMinus1LyNumhits_l4;
		TH1* nMinus1LyNumhitsInner_l1;          TH1* nMinus1LyNumhitsOuter_l1;          TH1* nMinus1LyNumhitsInner_l2;          TH1* nMinus1LyNumhitsOuter_l2;          TH1* nMinus1LyNumhitsInner_l3;           TH1* nMinus1LyNumhitsOuter_l3;          TH1* nMinus1LyNumhitsInner_l4;          TH1* nMinus1LyNumhitsOuter_l4;
		TH1* nMinus1LyEfficiency_fwd;           TH1* nMinus1LyEfficiency_l1;            TH1* nMinus1LyEfficiency_l2;            TH1* nMinus1LyEfficiency_l3;            TH1* nMinus1LyEfficiency_l4;
		TH1* nMinus1LyEfficiencyInner_l1;       TH1* nMinus1LyEfficiencyOuter_l1;       TH1* nMinus1LyEfficiencyInner_l2;       TH1* nMinus1LyEfficiencyOuter_l2;       TH1* nMinus1LyEfficiencyInner_l3;        TH1* nMinus1LyEfficiencyOuter_l3;       TH1* nMinus1LyEfficiencyInner_l4;       TH1* nMinus1LyEfficiencyOuter_l4;
		TH1* vtxNtrkNumhitsPreCuts;             TH1* vtxNtrkEfficiencyPreCuts;
		TH1* nMinus1VtxNtrkNumhits;             TH1* nMinus1VtxNtrkEfficiency;
		TH1* nMinus1PtNumhits;                  TH1* nMinus1PtEfficiency;
		TH1* ptNumhitsPreCuts;                  TH1* ptEfficiencyPreCuts;
		TH1* stripNumhitsPreCuts;               TH1* stripEfficiencyPreCuts;
		TH1* nMinus1stripNumhits;               TH1* nMinus1stripEfficiency;
		TH1* lxNumhitsPreCuts;                  TH1* lxEfficiencyPreCuts;
		TH1* nMinus1LxNumhits;                  TH1* nMinus1LxEfficiency;
		TH1* lyNumhitsPreCuts;                  TH1* lyEfficiencyPreCuts;
		TH1* nMinus1LyNumhits;                  TH1* nMinus1LyEfficiency;
		TH1* hitDistNumhitsPreCuts;             TH1* hitDistEfficiencyPreCuts;
		TH1* nMinus1HitDistNumhits;             TH1* nMinus1HitDistEfficiency;
		TH1* cluDistNumhitsPreCuts;             TH1* cluDistEfficiencyPreCuts;
		TH1* nMinus1CluDistNumhits;             TH1* nMinus1CluDistEfficiency;
		TH1* d0BarrelNumhitsPreCuts;            TH1* d0BarrelEfficiencyPreCuts;
		TH1* nMinus1D0BarrelNumhits;            TH1* nMinus1D0BarrelEfficiency;
		TH1* d0ForwardNumhitsPreCuts;           TH1* d0ForwardEfficiencyPreCuts;
		TH1* nMinus1D0ForwardNumhits;           TH1* nMinus1D0ForwardEfficiency;
		TH1* dZBarrelNumhitsPreCuts;            TH1* dZBarrelEfficiencyPreCuts;         TH1* dZForwardNumhitsPreCuts;           TH1* dZForwardEfficiencyPreCuts;
		TH1* nMinus1DZBarrelNumhits;            TH1* nMinus1DZBarrelEfficiency;
		TH1* localPosNumhitsLay1PreCuts;        TH1* localPosEfficiencyLay1PreCuts;     TH1* localPosNumhitsLay2PreCuts;        TH1* localPosEfficiencyLay2PreCuts;     TH1* localPosNumhitsLay3PreCuts;         TH1* localPosEfficiencyLay3PreCuts;     TH1* localPosNumhitsLay4PreCuts;        TH1* localPosEfficiencyLay4PreCuts;
		TH1* localPosNumhitsForward1PreCuts;    TH1* localPosNumhitsForward2PreCuts;    TH1* localPosNumhitsForward3PreCuts;    TH1* localPosNumhitsForward4PreCuts;    TH1* localPosNumhitsForward5PreCuts;     TH1* localPosNumhitsForward6PreCuts;    TH1* localPosNumhitsForward7PreCuts;    TH1* localPosNumhitsForward8PreCuts;
		TH1* localPosEfficiencyForward1PreCuts; TH1* localPosEfficiencyForward2PreCuts; TH1* localPosEfficiencyForward3PreCuts; TH1* localPosEfficiencyForward4PreCuts; TH1* localPosEfficiencyForward5PreCuts;  TH1* localPosEfficiencyForward6PreCuts; TH1* localPosEfficiencyForward7PreCuts; TH1* localPosEfficiencyForward8PreCuts;
		TH1* nMinus1DZForwardNumhits;           TH1* nMinus1DZForwardEfficiency;
		TH1* nMinus1LocalPosNumhitsLay1;        TH1* nMinus1LocalPosNumhitsLay2;        TH1* nMinus1LocalPosNumhitsLay3;        TH1* nMinus1LocalPosNumhitsLay4;
		TH1* nMinus1LocalPosEfficiencyLay1;     TH1* nMinus1LocalPosEfficiencyLay2;     TH1* nMinus1LocalPosEfficiencyLay3;     TH1* nMinus1LocalPosEfficiencyLay4;
		TH1* nMinus1LocalPosNumhitsForward1;    TH1* nMinus1LocalPosNumhitsForward2;    TH1* nMinus1LocalPosNumhitsForward3;    TH1* nMinus1LocalPosNumhitsForward4;    TH1* nMinus1LocalPosNumhitsForward5;     TH1* nMinus1LocalPosNumhitsForward6;    TH1* nMinus1LocalPosNumhitsForward7;    TH1* nMinus1LocalPosNumhitsForward8;
		TH1* nMinus1LocalPosEfficiencyForward1; TH1* nMinus1LocalPosEfficiencyForward2; TH1* nMinus1LocalPosEfficiencyForward3; TH1* nMinus1LocalPosEfficiencyForward4; TH1* nMinus1LocalPosEfficiencyForward5;  TH1* nMinus1LocalPosEfficiencyForward6; TH1* nMinus1LocalPosEfficiencyForward7; TH1* nMinus1LocalPosEfficiencyForward8;
	public:
		FilterCalibrationModule(HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
		~FilterCalibrationModule() = default;
		void fillHistograms();
		void printCounters();
		void printCutValues();
	private:
		void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts = 1);
		void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
		void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts = 1);
		void fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
		void calculateCuts();
		void incrementCounters();
};