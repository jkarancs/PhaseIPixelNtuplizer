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

class TrajMeasHistogramFillerModule
{
	using HistoMapType       = std::map<std::string, std::shared_ptr<TH1>>;
	protected:
		const EventData&       eventField_;
		const TrajMeasurement& trajField_;
		// Shortcuts
		const int&       det_;
		const int&       layer_;
		const int&       flipped_;
		const int&       module_;
		const int&       ladder_;
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
		const float&     norm_charge_;
		const TrackData& trk_;
		// Histograms
		TH1* layersDisksNumhits;                      TH1* layersDisksEfficiency;
		TH1* rechitOccupancy_l1;                      TH1* rechitOccupancy_l2;                         TH1* rechitOccupancy_l3;                      TH1* rechitOccupancy_l4;                         TH1* rechitOccupancy_fwd;
		TH1* rocNumhitsWithCuts_l1;                   TH1* rocNumhitsWithCuts_l2;                      TH1* rocNumhitsWithCuts_l3;                   TH1* rocNumhitsWithCuts_l4;                      TH1* rocNumhitsWithCuts_fwd;
		TH1* rocEfficiencyWithCuts_l1;                TH1* rocEfficiencyWithCuts_l2;                   TH1* rocEfficiencyWithCuts_l3;                TH1* rocEfficiencyWithCuts_l4;                   TH1* rocEfficiencyWithCuts_fwd;
		TH1* rechitPhiVsZ_l1;                         TH1* rechitPhiVsZ_l2;                            TH1* rechitPhiVsZ_l3;                         TH1* rechitPhiVsZ_l4;                            TH1* rechitPhiVsZ_fwd;
		TH1* sensorNumhitsWithCutsPhiVsZ_l1;          TH1* sensorNumhitsWithCutsPhiVsZ_l2;             TH1* sensorNumhitsWithCutsPhiVsZ_l3;          TH1* sensorNumhitsWithCutsPhiVsZ_l4;             TH1* sensorNumhitsWithCutsPhiVsZ_fwd;
		TH1* sensorEfficiencyWithCutsPhiVsZ_l1;       TH1* sensorEfficiencyWithCutsPhiVsZ_l2;          TH1* sensorEfficiencyWithCutsPhiVsZ_l3;       TH1* sensorEfficiencyWithCutsPhiVsZ_l4;          TH1* sensorEfficiencyWithCutsPhiVsZ_fwd;
		TH1* lxNumhitsPreCuts_fwd;                    TH1* lxNumhitsPreCuts_l1;                        TH1* lxNumhitsPreCuts_l2;                     TH1* lxNumhitsPreCuts_l3;                        TH1* lxNumhitsPreCuts_l4;
		TH1* lxNumhitsInnerPreCuts_l1;                TH1* lxNumhitsOuterPreCuts_l1;                   TH1* lxNumhitsInnerPreCuts_l2;                TH1* lxNumhitsOuterPreCuts_l2;                   TH1* lxNumhitsInnerPreCuts_l3;                TH1* lxNumhitsOuterPreCuts_l3;                   TH1* lxNumhitsInnerPreCuts_l4;                TH1* lxNumhitsOuterPreCuts_l4;
		TH1* lxEfficiencyPreCuts_fwd;                 TH1* lxEfficiencyPreCuts_l1;                     TH1* lxEfficiencyPreCuts_l2;                  TH1* lxEfficiencyPreCuts_l3;                     TH1* lxEfficiencyPreCuts_l4;
		TH1* lxEfficiencyInnerPreCuts_l1;             TH1* lxEfficiencyOuterPreCuts_l1;                TH1* lxEfficiencyInnerPreCuts_l2;             TH1* lxEfficiencyOuterPreCuts_l2;                TH1* lxEfficiencyInnerPreCuts_l3;             TH1* lxEfficiencyOuterPreCuts_l3;                TH1* lxEfficiencyInnerPreCuts_l4;             TH1* lxEfficiencyOuterPreCuts_l4;
		TH1* lyNumhitsPreCuts_fwd;                    TH1* lyNumhitsPreCuts_l1;                        TH1* lyNumhitsPreCuts_l2;                     TH1* lyNumhitsPreCuts_l3;                        TH1* lyNumhitsPreCuts_l4;
		TH1* lyNumhitsInnerPreCuts_l1;                TH1* lyNumhitsOuterPreCuts_l1;                   TH1* lyNumhitsInnerPreCuts_l2;                TH1* lyNumhitsOuterPreCuts_l2;                   TH1* lyNumhitsInnerPreCuts_l3;                TH1* lyNumhitsOuterPreCuts_l3;                   TH1* lyNumhitsInnerPreCuts_l4;                TH1* lyNumhitsOuterPreCuts_l4;
		TH1* lyEfficiencyPreCuts_fwd;                 TH1* lyEfficiencyPreCuts_l1;                     TH1* lyEfficiencyPreCuts_l2;                  TH1* lyEfficiencyPreCuts_l3;                     TH1* lyEfficiencyPreCuts_l4;
		TH1* lyEfficiencyInnerPreCuts_l1;             TH1* lyEfficiencyOuterPreCuts_l1;                TH1* lyEfficiencyInnerPreCuts_l2;             TH1* lyEfficiencyOuterPreCuts_l2;                TH1* lyEfficiencyInnerPreCuts_l3;             TH1* lyEfficiencyOuterPreCuts_l3;                TH1* lyEfficiencyInnerPreCuts_l4;             TH1* lyEfficiencyOuterPreCuts_l4;
		TH1* nMinus1LxNumhits_fwd;                    TH1* nMinus1LxNumhits_l1;                        TH1* nMinus1LxNumhits_l2;                     TH1* nMinus1LxNumhits_l3;                        TH1* nMinus1LxNumhits_l4;
		TH1* nMinus1LxNumhitsInner_l1;                TH1* nMinus1LxNumhitsOuter_l1;                   TH1* nMinus1LxNumhitsInner_l2;                TH1* nMinus1LxNumhitsOuter_l2;                   TH1* nMinus1LxNumhitsInner_l3;                TH1* nMinus1LxNumhitsOuter_l3;                   TH1* nMinus1LxNumhitsInner_l4;                TH1* nMinus1LxNumhitsOuter_l4;
		TH1* nMinus1LxEfficiency_fwd;                 TH1* nMinus1LxEfficiency_l1;                     TH1* nMinus1LxEfficiency_l2;                  TH1* nMinus1LxEfficiency_l3;                     TH1* nMinus1LxEfficiency_l4;
		TH1* nMinus1LxEfficiencyInner_l1;             TH1* nMinus1LxEfficiencyOuter_l1;                TH1* nMinus1LxEfficiencyInner_l2;             TH1* nMinus1LxEfficiencyOuter_l2;                TH1* nMinus1LxEfficiencyInner_l3;             TH1* nMinus1LxEfficiencyOuter_l3;                TH1* nMinus1LxEfficiencyInner_l4;             TH1* nMinus1LxEfficiencyOuter_l4;
		TH1* nMinus1LyNumhits_fwd;                    TH1* nMinus1LyNumhits_l1;                        TH1* nMinus1LyNumhits_l2;                     TH1* nMinus1LyNumhits_l3;                        TH1* nMinus1LyNumhits_l4;
		TH1* nMinus1LyNumhitsInner_l1;                TH1* nMinus1LyNumhitsOuter_l1;                   TH1* nMinus1LyNumhitsInner_l2;                TH1* nMinus1LyNumhitsOuter_l2;                   TH1* nMinus1LyNumhitsInner_l3;                TH1* nMinus1LyNumhitsOuter_l3;                   TH1* nMinus1LyNumhitsInner_l4;                TH1* nMinus1LyNumhitsOuter_l4;
		TH1* nMinus1LyEfficiency_fwd;                 TH1* nMinus1LyEfficiency_l1;                     TH1* nMinus1LyEfficiency_l2;                  TH1* nMinus1LyEfficiency_l3;                     TH1* nMinus1LyEfficiency_l4;
		TH1* nMinus1LyEfficiencyInner_l1;             TH1* nMinus1LyEfficiencyOuter_l1;                TH1* nMinus1LyEfficiencyInner_l2;             TH1* nMinus1LyEfficiencyOuter_l2;                TH1* nMinus1LyEfficiencyInner_l3;             TH1* nMinus1LyEfficiencyOuter_l3;                TH1* nMinus1LyEfficiencyInner_l4;             TH1* nMinus1LyEfficiencyOuter_l4;
		TH1* localPosNumhitsLay1InnerPreCuts;         TH1* localPosEfficiencyLay1InnerPreCuts;         TH1* localPosNumhitsLay2InnerPreCuts;         TH1* localPosEfficiencyLay2InnerPreCuts;         TH1* localPosNumhitsLay3InnerPreCuts;         TH1* localPosEfficiencyLay3InnerPreCuts;         TH1* localPosNumhitsLay4InnerPreCuts;         TH1* localPosEfficiencyLay4InnerPreCuts;
		TH1* localPosNumhitsLay1OuterPreCuts;         TH1* localPosEfficiencyLay1OuterPreCuts;         TH1* localPosNumhitsLay2OuterPreCuts;         TH1* localPosEfficiencyLay2OuterPreCuts;         TH1* localPosNumhitsLay3OuterPreCuts;         TH1* localPosEfficiencyLay3OuterPreCuts;         TH1* localPosNumhitsLay4OuterPreCuts;         TH1* localPosEfficiencyLay4OuterPreCuts;
		TH1* nMinus1LocalPosNumhitsLay1Inner;         TH1* nMinus1LocalPosEfficiencyLay1Inner;         TH1* nMinus1LocalPosNumhitsLay2Inner;         TH1* nMinus1LocalPosEfficiencyLay2Inner;         TH1* nMinus1LocalPosNumhitsLay3Inner;         TH1* nMinus1LocalPosEfficiencyLay3Inner;         TH1* nMinus1LocalPosNumhitsLay4Inner;         TH1* nMinus1LocalPosEfficiencyLay4Inner;
		TH1* nMinus1LocalPosNumhitsLay1Outer;         TH1* nMinus1LocalPosEfficiencyLay1Outer;         TH1* nMinus1LocalPosNumhitsLay2Outer;         TH1* nMinus1LocalPosEfficiencyLay2Outer;         TH1* nMinus1LocalPosNumhitsLay3Outer;         TH1* nMinus1LocalPosEfficiencyLay3Outer;         TH1* nMinus1LocalPosNumhitsLay4Outer;         TH1* nMinus1LocalPosEfficiencyLay4Outer;
		TH1* vtxNtrkNumhitsPreCuts;                   TH1* vtxNtrkEfficiencyPreCuts;
		TH1* nMinus1VtxNtrkNumhits;                   TH1* nMinus1VtxNtrkEfficiency;
		TH1* nMinus1PtNumhits;                        TH1* nMinus1PtEfficiency;
		TH1* ptNumhitsPreCuts;                        TH1* ptEfficiencyPreCuts;
		TH1* stripNumhitsPreCuts;                     TH1* stripEfficiencyPreCuts;
		TH1* nMinus1stripNumhits;                     TH1* nMinus1stripEfficiency;
		TH1* lxNumhitsPreCuts;                        TH1* lxEfficiencyPreCuts;
		TH1* nMinus1LxNumhits;                        TH1* nMinus1LxEfficiency;
		TH1* lyNumhitsPreCuts;                        TH1* lyEfficiencyPreCuts;
		TH1* nMinus1LyNumhits;                        TH1* nMinus1LyEfficiency;
		TH1* hitDistNumhitsPreCuts;                   TH1* hitDistEfficiencyPreCuts;
		TH1* nMinus1HitDistNumhits;                   TH1* nMinus1HitDistEfficiency;
		TH1* cluDistNumhitsPreCuts;                   TH1* cluDistEfficiencyPreCuts;
		TH1* nMinus1CluDistNumhits;                   TH1* nMinus1CluDistEfficiency;
		TH1* d0BarrelNumhitsPreCuts;                  TH1* d0BarrelEfficiencyPreCuts;
		TH1* nMinus1D0BarrelNumhits;                  TH1* nMinus1D0BarrelEfficiency;
		TH1* d0ForwardNumhitsPreCuts;                 TH1* d0ForwardEfficiencyPreCuts;
		TH1* nMinus1D0ForwardNumhits;                 TH1* nMinus1D0ForwardEfficiency;
		TH1* dZBarrelNumhitsPreCuts;                  TH1* dZBarrelEfficiencyPreCuts;                  TH1* dZForwardNumhitsPreCuts;                 TH1* dZForwardEfficiencyPreCuts;
		TH1* nMinus1DZBarrelNumhits;                  TH1* nMinus1DZBarrelEfficiency;
		TH1* localPosNumhitsLay1PreCuts;              TH1* localPosEfficiencyLay1PreCuts;              TH1* localPosNumhitsLay2PreCuts;              TH1* localPosEfficiencyLay2PreCuts;              TH1* localPosNumhitsLay3PreCuts;              TH1* localPosEfficiencyLay3PreCuts;              TH1* localPosNumhitsLay4PreCuts;              TH1* localPosEfficiencyLay4PreCuts;
		TH1* localPosNumhitsForward1PreCuts;          TH1* localPosNumhitsForward2PreCuts;             TH1* localPosNumhitsForward3PreCuts;          TH1* localPosNumhitsForward4PreCuts;             TH1* localPosNumhitsForward5PreCuts;          TH1* localPosNumhitsForward6PreCuts;             TH1* localPosNumhitsForward7PreCuts;          TH1* localPosNumhitsForward8PreCuts;
		TH1* localPosEfficiencyForward1PreCuts;       TH1* localPosEfficiencyForward2PreCuts;          TH1* localPosEfficiencyForward3PreCuts;       TH1* localPosEfficiencyForward4PreCuts;          TH1* localPosEfficiencyForward5PreCuts;       TH1* localPosEfficiencyForward6PreCuts;          TH1* localPosEfficiencyForward7PreCuts;       TH1* localPosEfficiencyForward8PreCuts;
		TH1* nMinus1DZForwardNumhits;                 TH1* nMinus1DZForwardEfficiency;
		TH1* nMinus1LocalPosNumhitsLay1;              TH1* nMinus1LocalPosNumhitsLay2;                 TH1* nMinus1LocalPosNumhitsLay3;              TH1* nMinus1LocalPosNumhitsLay4;
		TH1* nMinus1LocalPosEfficiencyLay1;           TH1* nMinus1LocalPosEfficiencyLay2;              TH1* nMinus1LocalPosEfficiencyLay3;           TH1* nMinus1LocalPosEfficiencyLay4;
		TH1* nMinus1LocalPosNumhitsForward1;          TH1* nMinus1LocalPosNumhitsForward2;             TH1* nMinus1LocalPosNumhitsForward3;          TH1* nMinus1LocalPosNumhitsForward4;             TH1* nMinus1LocalPosNumhitsForward5;          TH1* nMinus1LocalPosNumhitsForward6;             TH1* nMinus1LocalPosNumhitsForward7;          TH1* nMinus1LocalPosNumhitsForward8;
		TH1* nMinus1LocalPosEfficiencyForward1;       TH1* nMinus1LocalPosEfficiencyForward2;          TH1* nMinus1LocalPosEfficiencyForward3;       TH1* nMinus1LocalPosEfficiencyForward4;          TH1* nMinus1LocalPosEfficiencyForward5;       TH1* nMinus1LocalPosEfficiencyForward6;          TH1* nMinus1LocalPosEfficiencyForward7;       TH1* nMinus1LocalPosEfficiencyForward8;
		TH1* nvtxCutOnlyLxNumhits;                    TH1* nvtxCutOnlyLxEfficiency;                    TH1* hpCutOnlyLxNumhits;                      TH1* hpCutOnlyLxEfficiency;
		TH1* ptCutOnlyLxNumhits;                      TH1* ptCutOnlyLxEfficiency;                      TH1* d0CutOnlyLxNumhits;                      TH1* d0CutOnlyLxEfficiency;
		TH1* dzCutOnlyLxNumhits;                      TH1* dzCutOnlyLxEfficiency;                      TH1* pixhitCutOnlyLxNumhits;                  TH1* pixhitCutOnlyLxEfficiency;
		TH1* hitsepCutOnlyLxNumhits;                  TH1* hitsepCutOnlyLxEfficiency;
		TH1* localPosNumhitsOrientation1Disk1PreCuts; TH1* localPosEfficiencyOrientation1Disk1PreCuts; TH1* localPosNumhitsOrientation2Disk1PreCuts; TH1* localPosEfficiencyOrientation2Disk1PreCuts; TH1* localPosNumhitsOrientation3Disk1PreCuts; TH1* localPosEfficiencyOrientation3Disk1PreCuts; TH1* localPosNumhitsOrientation4Disk1PreCuts; TH1* localPosEfficiencyOrientation4Disk1PreCuts; TH1* localPosNumhitsOrientation5Disk1PreCuts; TH1* localPosEfficiencyOrientation5Disk1PreCuts; TH1* localPosNumhitsOrientation6Disk1PreCuts; TH1* localPosEfficiencyOrientation6Disk1PreCuts; TH1* localPosNumhitsOrientation7Disk1PreCuts; TH1* localPosEfficiencyOrientation7Disk1PreCuts; TH1* localPosNumhitsOrientation8Disk1PreCuts; TH1* localPosEfficiencyOrientation8Disk1PreCuts;
		TH1* nMinus1LocalPosNumhitsOrientation1Disk1; TH1* nMinus1LocalPosEfficiencyOrientation1Disk1; TH1* nMinus1LocalPosNumhitsOrientation2Disk1; TH1* nMinus1LocalPosEfficiencyOrientation2Disk1; TH1* nMinus1LocalPosNumhitsOrientation3Disk1; TH1* nMinus1LocalPosEfficiencyOrientation3Disk1; TH1* nMinus1LocalPosNumhitsOrientation4Disk1; TH1* nMinus1LocalPosEfficiencyOrientation4Disk1; TH1* nMinus1LocalPosNumhitsOrientation5Disk1; TH1* nMinus1LocalPosEfficiencyOrientation5Disk1; TH1* nMinus1LocalPosNumhitsOrientation6Disk1; TH1* nMinus1LocalPosEfficiencyOrientation6Disk1; TH1* nMinus1LocalPosNumhitsOrientation7Disk1; TH1* nMinus1LocalPosEfficiencyOrientation7Disk1; TH1* nMinus1LocalPosNumhitsOrientation8Disk1; TH1* nMinus1LocalPosEfficiencyOrientation8Disk1;
		TH1* localPosNumhitsOrientation1Disk2PreCuts; TH1* localPosEfficiencyOrientation1Disk2PreCuts; TH1* localPosNumhitsOrientation2Disk2PreCuts; TH1* localPosEfficiencyOrientation2Disk2PreCuts; TH1* localPosNumhitsOrientation3Disk2PreCuts; TH1* localPosEfficiencyOrientation3Disk2PreCuts; TH1* localPosNumhitsOrientation4Disk2PreCuts; TH1* localPosEfficiencyOrientation4Disk2PreCuts; TH1* localPosNumhitsOrientation5Disk2PreCuts; TH1* localPosEfficiencyOrientation5Disk2PreCuts; TH1* localPosNumhitsOrientation6Disk2PreCuts; TH1* localPosEfficiencyOrientation6Disk2PreCuts; TH1* localPosNumhitsOrientation7Disk2PreCuts; TH1* localPosEfficiencyOrientation7Disk2PreCuts; TH1* localPosNumhitsOrientation8Disk2PreCuts; TH1* localPosEfficiencyOrientation8Disk2PreCuts;
		TH1* nMinus1LocalPosNumhitsOrientation1Disk2; TH1* nMinus1LocalPosEfficiencyOrientation1Disk2; TH1* nMinus1LocalPosNumhitsOrientation2Disk2; TH1* nMinus1LocalPosEfficiencyOrientation2Disk2; TH1* nMinus1LocalPosNumhitsOrientation3Disk2; TH1* nMinus1LocalPosEfficiencyOrientation3Disk2; TH1* nMinus1LocalPosNumhitsOrientation4Disk2; TH1* nMinus1LocalPosEfficiencyOrientation4Disk2; TH1* nMinus1LocalPosNumhitsOrientation5Disk2; TH1* nMinus1LocalPosEfficiencyOrientation5Disk2; TH1* nMinus1LocalPosNumhitsOrientation6Disk2; TH1* nMinus1LocalPosEfficiencyOrientation6Disk2; TH1* nMinus1LocalPosNumhitsOrientation7Disk2; TH1* nMinus1LocalPosEfficiencyOrientation7Disk2; TH1* nMinus1LocalPosNumhitsOrientation8Disk2; TH1* nMinus1LocalPosEfficiencyOrientation8Disk2;
		TH1* localPosNumhitsOrientation1Disk3PreCuts; TH1* localPosEfficiencyOrientation1Disk3PreCuts; TH1* localPosNumhitsOrientation2Disk3PreCuts; TH1* localPosEfficiencyOrientation2Disk3PreCuts; TH1* localPosNumhitsOrientation3Disk3PreCuts; TH1* localPosEfficiencyOrientation3Disk3PreCuts; TH1* localPosNumhitsOrientation4Disk3PreCuts; TH1* localPosEfficiencyOrientation4Disk3PreCuts; TH1* localPosNumhitsOrientation5Disk3PreCuts; TH1* localPosEfficiencyOrientation5Disk3PreCuts; TH1* localPosNumhitsOrientation6Disk3PreCuts; TH1* localPosEfficiencyOrientation6Disk3PreCuts; TH1* localPosNumhitsOrientation7Disk3PreCuts; TH1* localPosEfficiencyOrientation7Disk3PreCuts; TH1* localPosNumhitsOrientation8Disk3PreCuts; TH1* localPosEfficiencyOrientation8Disk3PreCuts;
		TH1* nMinus1LocalPosNumhitsOrientation1Disk3; TH1* nMinus1LocalPosEfficiencyOrientation1Disk3; TH1* nMinus1LocalPosNumhitsOrientation2Disk3; TH1* nMinus1LocalPosEfficiencyOrientation2Disk3; TH1* nMinus1LocalPosNumhitsOrientation3Disk3; TH1* nMinus1LocalPosEfficiencyOrientation3Disk3; TH1* nMinus1LocalPosNumhitsOrientation4Disk3; TH1* nMinus1LocalPosEfficiencyOrientation4Disk3; TH1* nMinus1LocalPosNumhitsOrientation5Disk3; TH1* nMinus1LocalPosEfficiencyOrientation5Disk3; TH1* nMinus1LocalPosNumhitsOrientation6Disk3; TH1* nMinus1LocalPosEfficiencyOrientation6Disk3; TH1* nMinus1LocalPosNumhitsOrientation7Disk3; TH1* nMinus1LocalPosEfficiencyOrientation7Disk3; TH1* nMinus1LocalPosNumhitsOrientation8Disk3; TH1* nMinus1LocalPosEfficiencyOrientation8Disk3;
		TH1* cluDistNumhitsBarrelPreCuts;             TH1* cluDistNumhitsForwardPreCuts;
		TH1* hitDistNumhitsBarrelPreCuts;             TH1* hitDistNumhitsForwardPreCuts;
		TH1* cosmicsRingNumhits;                      TH1* cosmicsRingNumhitsWithAssociatedCluster;    TH1* cosmicsRingNumhitsDxyClLessThan1_0;
		TH1* cosmicsRingEffDxyClLessThan0_5;
		TH1* cosmicsRingsAverageDx;                   TH1* cosmicsRingsAverageDy;
		TH1* cosmicsRowVsColDxyClLessThan0_5_total;   TH1* cosmicsRowVsColDxyClLessThan0_5_barrel;     TH1* cosmicsRowVsColDxyClLessThan0_5_forward;
		TH1* rechitGlyVsGlx_barrel;
		TH1* rechitGlyVsGlx_fwd_disk1;                TH1* rechitGlyVsGlx_positiveZ_fwd_disk1;         TH1* rechitGlyVsGlx_negativeZ_fwd_disk1;
		TH1* rechitGlyVsGlx_fwd_disk2;                TH1* rechitGlyVsGlx_positiveZ_fwd_disk2;         TH1* rechitGlyVsGlx_negativeZ_fwd_disk2;
		TH1* rechitGlyVsGlx_fwd_disk3;                TH1* rechitGlyVsGlx_positiveZ_fwd_disk3;         TH1* rechitGlyVsGlx_negativeZ_fwd_disk3;
		TH1* associatedClusterXDistanceTotal;         TH1* associatedClusterYDistanceTotal;
		TH1* associatedClusterXDistanceBarrel;        TH1* associatedClusterYDistanceBarrel;
		TH1* associatedClusterXDistanceForward;       TH1* associatedClusterYDistanceForward;
		TH1* rechitZ_total;                           TH1* rechitZ_barrel;                             TH1* rechitZ_forward;
		TH1* rechitNormCharge_total;                  TH1* rechitNormCharge_barrel;                    TH1* rechitNormCharge_forward;
	public:
		TrajMeasHistogramFillerModule(const HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg);
		~TrajMeasHistogramFillerModule() = default;
		virtual void getHistogramsFromHistoMap(const HistoMapType& histogramsArg);
		virtual void fillHistograms() = 0;
	protected:
		TH1*         checkGetHistoFromMap(const HistoMapType& histogramsArg, const std::string& name);
		void         fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts = 1);
		void         fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
		void         fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts = 1);
		void         fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts);
		virtual void calculateCuts() = 0;
};