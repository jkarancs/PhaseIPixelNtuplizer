#pragma once

// Data structure
#include "../../../interface/DataStructures_v4.h"
#include "../interface/TimerColored.h"
#include "../interface/CommonActors.h"
#include "../interface/TrajMeasHistogramFillerModule.h"

#include "TH2D.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <memory>

class FilterCalibrationModule: public TrajMeasHistogramFillerModule
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
		static constexpr float                HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL   = 0.10f;
		static constexpr float                BARREL_MODULE_EDGE_X_CUT           = 0.6f;
		static constexpr float                BARREL_MODULE_EDGE_Y_CUT           = 3.0f;

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
	public:
		FilterCalibrationModule(const HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
		~FilterCalibrationModule() = default;
		virtual void getHistogramsFromHistoMap(const HistoMapType& histogramsArg);
		virtual void fillHistograms();
		void printCounters();
		void printCutValues();
	private:
		virtual void calculateCuts();
		void incrementCounters();
};