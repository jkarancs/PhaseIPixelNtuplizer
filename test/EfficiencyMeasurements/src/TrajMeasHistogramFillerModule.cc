#include "../interface/TrajMeasHistogramFillerModule.h"

TrajMeasHistogramFillerModule::TrajMeasHistogramFillerModule(const HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg): 
	eventField_      (eventFieldArg),
	trajField_       (trajFieldArg),
	det_             (trajField_.mod_on.det),
	layer_           (trajField_.mod_on.layer),
	flipped_         (trajField_.mod_on.flipped),
	module_          (trajField_.mod_on.module),
	ladder_          (trajField_.mod_on.ladder),
	side_            (trajField_.mod_on.side),
	disk_            (trajField_.mod_on.disk),
	blade_           (trajField_.mod_on.blade),
	panel_           (trajField_.mod_on.panel),
	ring_            (trajField_.mod_on.ring),
	missing_         (trajField_.missing),
	ladderCoord_     (trajField_.mod_on.ladder_coord),
	moduleCoord_     (trajField_.mod_on.module_coord),
	bladePanelCoord_ (trajField_.mod_on.blade_panel_coord),
	diskRingCoord_   (trajField_.mod_on.disk_ring_coord),
	lx_              (trajField_.lx),
	ly_              (trajField_.ly),
	lz_              (trajField_.lz),
	glx_             (trajField_.glx),
	gly_             (trajField_.gly),
	glz_             (trajField_.glz),
	norm_charge_     (trajField_.norm_charge),
	trk_             (trajField_.trk)
{
	getHistogramsFromHistoMap(histogramsArg);
}

void TrajMeasHistogramFillerModule::getHistogramsFromHistoMap(const HistoMapType& histogramsArg)
{
	auto checkGetHistoFromMap                  = [&] (const std::string& name) -> TH1* {return this -> checkGetHistoFromMap(histogramsArg, name);};
	layersDisksNumhits                         = checkGetHistoFromMap("layersDisksNumhits");
	layersDisksEfficiency                      = checkGetHistoFromMap("layersDisksEfficiency");
	rechitOccupancy_l1                         = checkGetHistoFromMap("rechitOccupancy_l1");
	rechitOccupancy_l2                         = checkGetHistoFromMap("rechitOccupancy_l2");
	rechitOccupancy_l3                         = checkGetHistoFromMap("rechitOccupancy_l3");
	rechitOccupancy_l4                         = checkGetHistoFromMap("rechitOccupancy_l4");
	rechitOccupancy_fwd                        = checkGetHistoFromMap("rechitOccupancy_fwd");
	rocNumhitsWithCuts_l1                      = checkGetHistoFromMap("rocNumhitsWithCuts_l1");
	rocNumhitsWithCuts_l2                      = checkGetHistoFromMap("rocNumhitsWithCuts_l2");
	rocNumhitsWithCuts_l3                      = checkGetHistoFromMap("rocNumhitsWithCuts_l3");
	rocNumhitsWithCuts_l4                      = checkGetHistoFromMap("rocNumhitsWithCuts_l4");
	rocNumhitsWithCuts_fwd                     = checkGetHistoFromMap("rocNumhitsWithCuts_fwd");
	rocEfficiencyWithCuts_l1                   = checkGetHistoFromMap("rocEfficiencyWithCuts_l1");
	rocEfficiencyWithCuts_l2                   = checkGetHistoFromMap("rocEfficiencyWithCuts_l2");
	rocEfficiencyWithCuts_l3                   = checkGetHistoFromMap("rocEfficiencyWithCuts_l3");
	rocEfficiencyWithCuts_l4                   = checkGetHistoFromMap("rocEfficiencyWithCuts_l4");
	rocEfficiencyWithCuts_fwd                  = checkGetHistoFromMap("rocEfficiencyWithCuts_fwd");
	rechitPhiVsZ_l1                            = checkGetHistoFromMap("rechitPhiVsZ_l1");
	rechitPhiVsZ_l2                            = checkGetHistoFromMap("rechitPhiVsZ_l2");
	rechitPhiVsZ_l3                            = checkGetHistoFromMap("rechitPhiVsZ_l3");
	rechitPhiVsZ_l4                            = checkGetHistoFromMap("rechitPhiVsZ_l4");
	rechitPhiVsZ_fwd                           = checkGetHistoFromMap("rechitPhiVsZ_fwd");
	sensorNumhitsWithCutsPhiVsZ_l1             = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l1");
	sensorNumhitsWithCutsPhiVsZ_l2             = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l2");
	sensorNumhitsWithCutsPhiVsZ_l3             = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l3");
	sensorNumhitsWithCutsPhiVsZ_l4             = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_l4");
	sensorNumhitsWithCutsPhiVsZ_fwd            = checkGetHistoFromMap("sensorNumhitsWithCutsPhiVsZ_fwd");
	sensorEfficiencyWithCutsPhiVsZ_l1          = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l1");
	sensorEfficiencyWithCutsPhiVsZ_l2          = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l2");
	sensorEfficiencyWithCutsPhiVsZ_l3          = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l3");
	sensorEfficiencyWithCutsPhiVsZ_l4          = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_l4");
	sensorEfficiencyWithCutsPhiVsZ_fwd         = checkGetHistoFromMap("sensorEfficiencyWithCutsPhiVsZ_fwd");
	rechitPhiVsZ_l1                            = checkGetHistoFromMap("rechitPhiVsZ_l1");
	rechitPhiVsZ_l2                            = checkGetHistoFromMap("rechitPhiVsZ_l2");
	rechitPhiVsZ_l3                            = checkGetHistoFromMap("rechitPhiVsZ_l3");
	rechitPhiVsZ_l4                            = checkGetHistoFromMap("rechitPhiVsZ_l4");
	rechitPhiVsZ_fwd                           = checkGetHistoFromMap("rechitPhiVsZ_fwd");
	vtxNtrkNumhitsPreCuts                      = checkGetHistoFromMap("vtxNtrkNumhitsPreCuts");
	vtxNtrkEfficiencyPreCuts                   = checkGetHistoFromMap("vtxNtrkEfficiencyPreCuts");
	nMinus1VtxNtrkNumhits                      = checkGetHistoFromMap("nMinus1VtxNtrkNumhits");
	nMinus1VtxNtrkEfficiency                   = checkGetHistoFromMap("nMinus1VtxNtrkEfficiency");
	nMinus1PtNumhits                           = checkGetHistoFromMap("nMinus1PtNumhits");
	nMinus1PtEfficiency                        = checkGetHistoFromMap("nMinus1PtEfficiency");
	ptNumhitsPreCuts                           = checkGetHistoFromMap("ptNumhitsPreCuts");
	ptEfficiencyPreCuts                        = checkGetHistoFromMap("ptEfficiencyPreCuts");
	stripNumhitsPreCuts                        = checkGetHistoFromMap("stripNumhitsPreCuts");
	stripEfficiencyPreCuts                     = checkGetHistoFromMap("stripEfficiencyPreCuts");
	nMinus1stripNumhits                        = checkGetHistoFromMap("nMinus1stripNumhits");
	nMinus1stripEfficiency                     = checkGetHistoFromMap("nMinus1stripEfficiency");
	lxNumhitsPreCuts                           = checkGetHistoFromMap("lxNumhitsPreCuts");
	lxEfficiencyPreCuts                        = checkGetHistoFromMap("lxEfficiencyPreCuts");
	nMinus1LxNumhits                           = checkGetHistoFromMap("nMinus1LxNumhits");
	nMinus1LxEfficiency                        = checkGetHistoFromMap("nMinus1LxEfficiency");
	lyNumhitsPreCuts                           = checkGetHistoFromMap("lyNumhitsPreCuts");
	lyEfficiencyPreCuts                        = checkGetHistoFromMap("lyEfficiencyPreCuts");
	nMinus1LyNumhits                           = checkGetHistoFromMap("nMinus1LyNumhits");
	nMinus1LyEfficiency                        = checkGetHistoFromMap("nMinus1LyEfficiency");
	hitDistNumhitsPreCuts                      = checkGetHistoFromMap("hitDistNumhitsPreCuts");
	hitDistEfficiencyPreCuts                   = checkGetHistoFromMap("hitDistEfficiencyPreCuts");
	nMinus1HitDistNumhits                      = checkGetHistoFromMap("nMinus1HitDistNumhits");
	nMinus1HitDistEfficiency                   = checkGetHistoFromMap("nMinus1HitDistEfficiency");
	cluDistNumhitsPreCuts                      = checkGetHistoFromMap("cluDistNumhitsPreCuts");
	cluDistEfficiencyPreCuts                   = checkGetHistoFromMap("cluDistEfficiencyPreCuts");
	nMinus1CluDistNumhits                      = checkGetHistoFromMap("nMinus1CluDistNumhits");
	nMinus1CluDistEfficiency                   = checkGetHistoFromMap("nMinus1CluDistEfficiency");
	d0BarrelNumhitsPreCuts                     = checkGetHistoFromMap("d0BarrelNumhitsPreCuts");
	d0BarrelEfficiencyPreCuts                  = checkGetHistoFromMap("d0BarrelEfficiencyPreCuts");
	nMinus1D0BarrelNumhits                     = checkGetHistoFromMap("nMinus1D0BarrelNumhits");
	nMinus1D0BarrelEfficiency                  = checkGetHistoFromMap("nMinus1D0BarrelEfficiency");
	d0ForwardNumhitsPreCuts                    = checkGetHistoFromMap("d0ForwardNumhitsPreCuts");
	d0ForwardEfficiencyPreCuts                 = checkGetHistoFromMap("d0ForwardEfficiencyPreCuts");
	nMinus1D0ForwardNumhits                    = checkGetHistoFromMap("nMinus1D0ForwardNumhits");
	nMinus1D0ForwardEfficiency                 = checkGetHistoFromMap("nMinus1D0ForwardEfficiency");
	dZBarrelNumhitsPreCuts                     = checkGetHistoFromMap("dZBarrelNumhitsPreCuts");
	dZBarrelEfficiencyPreCuts                  = checkGetHistoFromMap("dZBarrelEfficiencyPreCuts");
	dZForwardNumhitsPreCuts                    = checkGetHistoFromMap("dZForwardNumhitsPreCuts");
	dZForwardEfficiencyPreCuts                 = checkGetHistoFromMap("dZForwardEfficiencyPreCuts");
	nMinus1DZBarrelNumhits                     = checkGetHistoFromMap("nMinus1DZBarrelNumhits");
	nMinus1DZBarrelEfficiency                  = checkGetHistoFromMap("nMinus1DZBarrelEfficiency");
	localPosNumhitsLay1PreCuts                 = checkGetHistoFromMap("localPosNumhitsLay1PreCuts");
	localPosEfficiencyLay1PreCuts              = checkGetHistoFromMap("localPosEfficiencyLay1PreCuts");
	localPosNumhitsLay2PreCuts                 = checkGetHistoFromMap("localPosNumhitsLay2PreCuts");
	localPosEfficiencyLay2PreCuts              = checkGetHistoFromMap("localPosEfficiencyLay2PreCuts");
	localPosNumhitsLay3PreCuts                 = checkGetHistoFromMap("localPosNumhitsLay3PreCuts");
	localPosEfficiencyLay3PreCuts              = checkGetHistoFromMap("localPosEfficiencyLay3PreCuts");
	localPosNumhitsLay4PreCuts                 = checkGetHistoFromMap("localPosNumhitsLay4PreCuts");
	localPosEfficiencyLay4PreCuts              = checkGetHistoFromMap("localPosEfficiencyLay4PreCuts");
	localPosNumhitsForward1PreCuts             = checkGetHistoFromMap("localPosNumhitsForward1PreCuts");
	localPosNumhitsForward2PreCuts             = checkGetHistoFromMap("localPosNumhitsForward2PreCuts");
	localPosNumhitsForward3PreCuts             = checkGetHistoFromMap("localPosNumhitsForward3PreCuts");
	localPosNumhitsForward4PreCuts             = checkGetHistoFromMap("localPosNumhitsForward4PreCuts");
	localPosNumhitsForward5PreCuts             = checkGetHistoFromMap("localPosNumhitsForward5PreCuts");
	localPosNumhitsForward6PreCuts             = checkGetHistoFromMap("localPosNumhitsForward6PreCuts");
	localPosNumhitsForward7PreCuts             = checkGetHistoFromMap("localPosNumhitsForward7PreCuts");
	localPosNumhitsForward8PreCuts             = checkGetHistoFromMap("localPosNumhitsForward8PreCuts");
	localPosEfficiencyForward1PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward1PreCuts");
	localPosEfficiencyForward2PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward2PreCuts");
	localPosEfficiencyForward3PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward3PreCuts");
	localPosEfficiencyForward4PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward4PreCuts");
	localPosEfficiencyForward5PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward5PreCuts");
	localPosEfficiencyForward6PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward6PreCuts");
	localPosEfficiencyForward7PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward7PreCuts");
	localPosEfficiencyForward8PreCuts          = checkGetHistoFromMap("localPosEfficiencyForward8PreCuts");
	nMinus1DZForwardNumhits                    = checkGetHistoFromMap("nMinus1DZForwardNumhits");
	nMinus1DZForwardEfficiency                 = checkGetHistoFromMap("nMinus1DZForwardEfficiency");
	nMinus1LocalPosNumhitsLay1                 = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay1");
	nMinus1LocalPosNumhitsLay2                 = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay2");
	nMinus1LocalPosNumhitsLay3                 = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay3");
	nMinus1LocalPosNumhitsLay4                 = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay4");
	nMinus1LocalPosEfficiencyLay1              = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay1");
	nMinus1LocalPosEfficiencyLay2              = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay2");
	nMinus1LocalPosEfficiencyLay3              = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay3");
	nMinus1LocalPosEfficiencyLay4              = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay4");
	nMinus1LocalPosNumhitsForward1             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward1");
	nMinus1LocalPosNumhitsForward2             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward2");
	nMinus1LocalPosNumhitsForward3             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward3");
	nMinus1LocalPosNumhitsForward4             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward4");
	nMinus1LocalPosNumhitsForward5             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward5");
	nMinus1LocalPosNumhitsForward6             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward6");
	nMinus1LocalPosNumhitsForward7             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward7");
	nMinus1LocalPosNumhitsForward8             = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward8");
	nMinus1LocalPosEfficiencyForward1          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward1");
	nMinus1LocalPosEfficiencyForward2          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward2");
	nMinus1LocalPosEfficiencyForward3          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward3");
	nMinus1LocalPosEfficiencyForward4          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward4");
	nMinus1LocalPosEfficiencyForward5          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward5");
	nMinus1LocalPosEfficiencyForward6          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward6");
	nMinus1LocalPosEfficiencyForward7          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward7");
	nMinus1LocalPosEfficiencyForward8          = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward8");
	lxNumhitsPreCuts_fwd                       = checkGetHistoFromMap("lxNumhitsPreCuts_fwd");
	lxNumhitsPreCuts_l1                        = checkGetHistoFromMap("lxNumhitsPreCuts_l1");
	lxNumhitsPreCuts_l2                        = checkGetHistoFromMap("lxNumhitsPreCuts_l2");
	lxNumhitsPreCuts_l3                        = checkGetHistoFromMap("lxNumhitsPreCuts_l3");
	lxNumhitsPreCuts_l4                        = checkGetHistoFromMap("lxNumhitsPreCuts_l4");
	lxNumhitsInnerPreCuts_l1                   = checkGetHistoFromMap("lxNumhitsInnerPreCuts_l1");
	lxNumhitsOuterPreCuts_l1                   = checkGetHistoFromMap("lxNumhitsOuterPreCuts_l1");
	lxNumhitsInnerPreCuts_l2                   = checkGetHistoFromMap("lxNumhitsInnerPreCuts_l2");
	lxNumhitsOuterPreCuts_l2                   = checkGetHistoFromMap("lxNumhitsOuterPreCuts_l2");
	lxNumhitsInnerPreCuts_l3                   = checkGetHistoFromMap("lxNumhitsInnerPreCuts_l3");
	lxNumhitsOuterPreCuts_l3                   = checkGetHistoFromMap("lxNumhitsOuterPreCuts_l3");
	lxNumhitsInnerPreCuts_l4                   = checkGetHistoFromMap("lxNumhitsInnerPreCuts_l4");
	lxNumhitsOuterPreCuts_l4                   = checkGetHistoFromMap("lxNumhitsOuterPreCuts_l4");
	lxEfficiencyPreCuts_fwd                    = checkGetHistoFromMap("lxEfficiencyPreCuts_fwd");
	lxEfficiencyPreCuts_l1                     = checkGetHistoFromMap("lxEfficiencyPreCuts_l1");
	lxEfficiencyPreCuts_l2                     = checkGetHistoFromMap("lxEfficiencyPreCuts_l2");
	lxEfficiencyPreCuts_l3                     = checkGetHistoFromMap("lxEfficiencyPreCuts_l3");
	lxEfficiencyPreCuts_l4                     = checkGetHistoFromMap("lxEfficiencyPreCuts_l4");
	lxEfficiencyInnerPreCuts_l1                = checkGetHistoFromMap("lxEfficiencyInnerPreCuts_l1");
	lxEfficiencyOuterPreCuts_l1                = checkGetHistoFromMap("lxEfficiencyOuterPreCuts_l1");
	lxEfficiencyInnerPreCuts_l2                = checkGetHistoFromMap("lxEfficiencyInnerPreCuts_l2");
	lxEfficiencyOuterPreCuts_l2                = checkGetHistoFromMap("lxEfficiencyOuterPreCuts_l2");
	lxEfficiencyInnerPreCuts_l3                = checkGetHistoFromMap("lxEfficiencyInnerPreCuts_l3");
	lxEfficiencyOuterPreCuts_l3                = checkGetHistoFromMap("lxEfficiencyOuterPreCuts_l3");
	lxEfficiencyInnerPreCuts_l4                = checkGetHistoFromMap("lxEfficiencyInnerPreCuts_l4");
	lxEfficiencyOuterPreCuts_l4                = checkGetHistoFromMap("lxEfficiencyOuterPreCuts_l4");
	lyNumhitsPreCuts_fwd                       = checkGetHistoFromMap("lyNumhitsPreCuts_fwd");
	lyNumhitsPreCuts_l1                        = checkGetHistoFromMap("lyNumhitsPreCuts_l1");
	lyNumhitsPreCuts_l2                        = checkGetHistoFromMap("lyNumhitsPreCuts_l2");
	lyNumhitsPreCuts_l3                        = checkGetHistoFromMap("lyNumhitsPreCuts_l3");
	lyNumhitsPreCuts_l4                        = checkGetHistoFromMap("lyNumhitsPreCuts_l4");
	lyNumhitsInnerPreCuts_l1                   = checkGetHistoFromMap("lyNumhitsInnerPreCuts_l1");
	lyNumhitsOuterPreCuts_l1                   = checkGetHistoFromMap("lyNumhitsOuterPreCuts_l1");
	lyNumhitsInnerPreCuts_l2                   = checkGetHistoFromMap("lyNumhitsInnerPreCuts_l2");
	lyNumhitsOuterPreCuts_l2                   = checkGetHistoFromMap("lyNumhitsOuterPreCuts_l2");
	lyNumhitsInnerPreCuts_l3                   = checkGetHistoFromMap("lyNumhitsInnerPreCuts_l3");
	lyNumhitsOuterPreCuts_l3                   = checkGetHistoFromMap("lyNumhitsOuterPreCuts_l3");
	lyNumhitsInnerPreCuts_l4                   = checkGetHistoFromMap("lyNumhitsInnerPreCuts_l4");
	lyNumhitsOuterPreCuts_l4                   = checkGetHistoFromMap("lyNumhitsOuterPreCuts_l4");
	lyEfficiencyPreCuts_fwd                    = checkGetHistoFromMap("lyEfficiencyPreCuts_fwd");
	lyEfficiencyPreCuts_l1                     = checkGetHistoFromMap("lyEfficiencyPreCuts_l1");
	lyEfficiencyPreCuts_l2                     = checkGetHistoFromMap("lyEfficiencyPreCuts_l2");
	lyEfficiencyPreCuts_l3                     = checkGetHistoFromMap("lyEfficiencyPreCuts_l3");
	lyEfficiencyPreCuts_l4                     = checkGetHistoFromMap("lyEfficiencyPreCuts_l4");
	lyEfficiencyInnerPreCuts_l1                = checkGetHistoFromMap("lyEfficiencyInnerPreCuts_l1");
	lyEfficiencyOuterPreCuts_l1                = checkGetHistoFromMap("lyEfficiencyOuterPreCuts_l1");
	lyEfficiencyInnerPreCuts_l2                = checkGetHistoFromMap("lyEfficiencyInnerPreCuts_l2");
	lyEfficiencyOuterPreCuts_l2                = checkGetHistoFromMap("lyEfficiencyOuterPreCuts_l2");
	lyEfficiencyInnerPreCuts_l3                = checkGetHistoFromMap("lyEfficiencyInnerPreCuts_l3");
	lyEfficiencyOuterPreCuts_l3                = checkGetHistoFromMap("lyEfficiencyOuterPreCuts_l3");
	lyEfficiencyInnerPreCuts_l4                = checkGetHistoFromMap("lyEfficiencyInnerPreCuts_l4");
	lyEfficiencyOuterPreCuts_l4                = checkGetHistoFromMap("lyEfficiencyOuterPreCuts_l4");
	nMinus1LxNumhits_fwd                       = checkGetHistoFromMap("nMinus1LxNumhits_fwd");
	nMinus1LxNumhits_l1                        = checkGetHistoFromMap("nMinus1LxNumhits_l1");
	nMinus1LxNumhits_l2                        = checkGetHistoFromMap("nMinus1LxNumhits_l2");
	nMinus1LxNumhits_l3                        = checkGetHistoFromMap("nMinus1LxNumhits_l3");
	nMinus1LxNumhits_l4                        = checkGetHistoFromMap("nMinus1LxNumhits_l4");
	nMinus1LxNumhitsInner_l1                   = checkGetHistoFromMap("nMinus1LxNumhitsInner_l1");
	nMinus1LxNumhitsOuter_l1                   = checkGetHistoFromMap("nMinus1LxNumhitsOuter_l1");
	nMinus1LxNumhitsInner_l2                   = checkGetHistoFromMap("nMinus1LxNumhitsInner_l2");
	nMinus1LxNumhitsOuter_l2                   = checkGetHistoFromMap("nMinus1LxNumhitsOuter_l2");
	nMinus1LxNumhitsInner_l3                   = checkGetHistoFromMap("nMinus1LxNumhitsInner_l3");
	nMinus1LxNumhitsOuter_l3                   = checkGetHistoFromMap("nMinus1LxNumhitsOuter_l3");
	nMinus1LxNumhitsInner_l4                   = checkGetHistoFromMap("nMinus1LxNumhitsInner_l4");
	nMinus1LxNumhitsOuter_l4                   = checkGetHistoFromMap("nMinus1LxNumhitsOuter_l4");
	nMinus1LxEfficiency_fwd                    = checkGetHistoFromMap("nMinus1LxEfficiency_fwd");
	nMinus1LxEfficiency_l1                     = checkGetHistoFromMap("nMinus1LxEfficiency_l1");
	nMinus1LxEfficiency_l2                     = checkGetHistoFromMap("nMinus1LxEfficiency_l2");
	nMinus1LxEfficiency_l3                     = checkGetHistoFromMap("nMinus1LxEfficiency_l3");
	nMinus1LxEfficiency_l4                     = checkGetHistoFromMap("nMinus1LxEfficiency_l4");
	nMinus1LxEfficiencyInner_l1                = checkGetHistoFromMap("nMinus1LxEfficiencyInner_l1");
	nMinus1LxEfficiencyOuter_l1                = checkGetHistoFromMap("nMinus1LxEfficiencyOuter_l1");
	nMinus1LxEfficiencyInner_l2                = checkGetHistoFromMap("nMinus1LxEfficiencyInner_l2");
	nMinus1LxEfficiencyOuter_l2                = checkGetHistoFromMap("nMinus1LxEfficiencyOuter_l2");
	nMinus1LxEfficiencyInner_l3                = checkGetHistoFromMap("nMinus1LxEfficiencyInner_l3");
	nMinus1LxEfficiencyOuter_l3                = checkGetHistoFromMap("nMinus1LxEfficiencyOuter_l3");
	nMinus1LxEfficiencyInner_l4                = checkGetHistoFromMap("nMinus1LxEfficiencyInner_l4");
	nMinus1LxEfficiencyOuter_l4                = checkGetHistoFromMap("nMinus1LxEfficiencyOuter_l4");
	nMinus1LyNumhits_fwd                       = checkGetHistoFromMap("nMinus1LyNumhits_fwd");
	nMinus1LyNumhits_l1                        = checkGetHistoFromMap("nMinus1LyNumhits_l1");
	nMinus1LyNumhits_l2                        = checkGetHistoFromMap("nMinus1LyNumhits_l2");
	nMinus1LyNumhits_l3                        = checkGetHistoFromMap("nMinus1LyNumhits_l3");
	nMinus1LyNumhits_l4                        = checkGetHistoFromMap("nMinus1LyNumhits_l4");
	nMinus1LyNumhitsInner_l1                   = checkGetHistoFromMap("nMinus1LyNumhitsInner_l1");
	nMinus1LyNumhitsOuter_l1                   = checkGetHistoFromMap("nMinus1LyNumhitsOuter_l1");
	nMinus1LyNumhitsInner_l2                   = checkGetHistoFromMap("nMinus1LyNumhitsInner_l2");
	nMinus1LyNumhitsOuter_l2                   = checkGetHistoFromMap("nMinus1LyNumhitsOuter_l2");
	nMinus1LyNumhitsInner_l3                   = checkGetHistoFromMap("nMinus1LyNumhitsInner_l3");
	nMinus1LyNumhitsOuter_l3                   = checkGetHistoFromMap("nMinus1LyNumhitsOuter_l3");
	nMinus1LyNumhitsInner_l4                   = checkGetHistoFromMap("nMinus1LyNumhitsInner_l4");
	nMinus1LyNumhitsOuter_l4                   = checkGetHistoFromMap("nMinus1LyNumhitsOuter_l4");
	nMinus1LyEfficiency_fwd                    = checkGetHistoFromMap("nMinus1LyEfficiency_fwd");
	nMinus1LyEfficiency_l1                     = checkGetHistoFromMap("nMinus1LyEfficiency_l1");
	nMinus1LyEfficiency_l2                     = checkGetHistoFromMap("nMinus1LyEfficiency_l2");
	nMinus1LyEfficiency_l3                     = checkGetHistoFromMap("nMinus1LyEfficiency_l3");
	nMinus1LyEfficiency_l4                     = checkGetHistoFromMap("nMinus1LyEfficiency_l4");
	nMinus1LyEfficiencyInner_l1                = checkGetHistoFromMap("nMinus1LyEfficiencyInner_l1");
	nMinus1LyEfficiencyOuter_l1                = checkGetHistoFromMap("nMinus1LyEfficiencyOuter_l1");
	nMinus1LyEfficiencyInner_l2                = checkGetHistoFromMap("nMinus1LyEfficiencyInner_l2");
	nMinus1LyEfficiencyOuter_l2                = checkGetHistoFromMap("nMinus1LyEfficiencyOuter_l2");
	nMinus1LyEfficiencyInner_l3                = checkGetHistoFromMap("nMinus1LyEfficiencyInner_l3");
	nMinus1LyEfficiencyOuter_l3                = checkGetHistoFromMap("nMinus1LyEfficiencyOuter_l3");
	nMinus1LyEfficiencyInner_l4                = checkGetHistoFromMap("nMinus1LyEfficiencyInner_l4");
	nMinus1LyEfficiencyOuter_l4                = checkGetHistoFromMap("nMinus1LyEfficiencyOuter_l4");
	localPosNumhitsLay1InnerPreCuts            = checkGetHistoFromMap("localPosNumhitsLay1InnerPreCuts");
	localPosEfficiencyLay1InnerPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay1InnerPreCuts");
	localPosNumhitsLay2InnerPreCuts            = checkGetHistoFromMap("localPosNumhitsLay2InnerPreCuts");
	localPosEfficiencyLay2InnerPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay2InnerPreCuts");
	localPosNumhitsLay3InnerPreCuts            = checkGetHistoFromMap("localPosNumhitsLay3InnerPreCuts");
	localPosEfficiencyLay3InnerPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay3InnerPreCuts");
	localPosNumhitsLay4InnerPreCuts            = checkGetHistoFromMap("localPosNumhitsLay4InnerPreCuts");
	localPosEfficiencyLay4InnerPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay4InnerPreCuts");
	localPosNumhitsLay1OuterPreCuts            = checkGetHistoFromMap("localPosNumhitsLay1OuterPreCuts");
	localPosEfficiencyLay1OuterPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay1OuterPreCuts");
	localPosNumhitsLay2OuterPreCuts            = checkGetHistoFromMap("localPosNumhitsLay2OuterPreCuts");
	localPosEfficiencyLay2OuterPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay2OuterPreCuts");
	localPosNumhitsLay3OuterPreCuts            = checkGetHistoFromMap("localPosNumhitsLay3OuterPreCuts");
	localPosEfficiencyLay3OuterPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay3OuterPreCuts");
	localPosNumhitsLay4OuterPreCuts            = checkGetHistoFromMap("localPosNumhitsLay4OuterPreCuts");
	localPosEfficiencyLay4OuterPreCuts         = checkGetHistoFromMap("localPosEfficiencyLay4OuterPreCuts");
	nMinus1LocalPosNumhitsLay1Inner            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay1Inner");
	nMinus1LocalPosEfficiencyLay1Inner         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay1Inner");
	nMinus1LocalPosNumhitsLay2Inner            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay2Inner");
	nMinus1LocalPosEfficiencyLay2Inner         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay2Inner");
	nMinus1LocalPosNumhitsLay3Inner            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay3Inner");
	nMinus1LocalPosEfficiencyLay3Inner         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay3Inner");
	nMinus1LocalPosNumhitsLay4Inner            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay4Inner");
	nMinus1LocalPosEfficiencyLay4Inner         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay4Inner");
	nMinus1LocalPosNumhitsLay1Outer            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay1Outer");
	nMinus1LocalPosEfficiencyLay1Outer         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay1Outer");
	nMinus1LocalPosNumhitsLay2Outer            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay2Outer");
	nMinus1LocalPosEfficiencyLay2Outer         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay2Outer");
	nMinus1LocalPosNumhitsLay3Outer            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay3Outer");
	nMinus1LocalPosEfficiencyLay3Outer         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay3Outer");
	nMinus1LocalPosNumhitsLay4Outer            = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay4Outer");
	nMinus1LocalPosEfficiencyLay4Outer         = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay4Outer");
	nvtxCutOnlyLxNumhits                       = checkGetHistoFromMap("nvtxCutOnlyLxNumhits");
	nvtxCutOnlyLxEfficiency                    = checkGetHistoFromMap("nvtxCutOnlyLxEfficiency");
	hpCutOnlyLxNumhits                         = checkGetHistoFromMap("hpCutOnlyLxNumhits");
	hpCutOnlyLxEfficiency                      = checkGetHistoFromMap("hpCutOnlyLxEfficiency");
	ptCutOnlyLxNumhits                         = checkGetHistoFromMap("ptCutOnlyLxNumhits");
	ptCutOnlyLxEfficiency                      = checkGetHistoFromMap("ptCutOnlyLxEfficiency");
	d0CutOnlyLxNumhits                         = checkGetHistoFromMap("d0CutOnlyLxNumhits");
	d0CutOnlyLxEfficiency                      = checkGetHistoFromMap("d0CutOnlyLxEfficiency");
	dzCutOnlyLxNumhits                         = checkGetHistoFromMap("dzCutOnlyLxNumhits");
	dzCutOnlyLxEfficiency                      = checkGetHistoFromMap("dzCutOnlyLxEfficiency");
	pixhitCutOnlyLxNumhits                     = checkGetHistoFromMap("pixhitCutOnlyLxNumhits");
	pixhitCutOnlyLxEfficiency                  = checkGetHistoFromMap("pixhitCutOnlyLxEfficiency");
	hitsepCutOnlyLxNumhits                     = checkGetHistoFromMap("hitsepCutOnlyLxNumhits");
	hitsepCutOnlyLxEfficiency                  = checkGetHistoFromMap("hitsepCutOnlyLxEfficiency");
	localPosNumhitsOrientation1Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation1Disk1PreCuts");
	localPosEfficiencyOrientation1Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation1Disk1PreCuts");
	localPosNumhitsOrientation2Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation2Disk1PreCuts");
	localPosEfficiencyOrientation2Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation2Disk1PreCuts");
	localPosNumhitsOrientation3Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation3Disk1PreCuts");
	localPosEfficiencyOrientation3Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation3Disk1PreCuts");
	localPosNumhitsOrientation4Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation4Disk1PreCuts");
	localPosEfficiencyOrientation4Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation4Disk1PreCuts");
	localPosNumhitsOrientation5Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation5Disk1PreCuts");
	localPosEfficiencyOrientation5Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation5Disk1PreCuts");
	localPosNumhitsOrientation6Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation6Disk1PreCuts");
	localPosEfficiencyOrientation6Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation6Disk1PreCuts");
	localPosNumhitsOrientation7Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation7Disk1PreCuts");
	localPosEfficiencyOrientation7Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation7Disk1PreCuts");
	localPosNumhitsOrientation8Disk1PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation8Disk1PreCuts");
	localPosEfficiencyOrientation8Disk1PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation8Disk1PreCuts");
	nMinus1LocalPosNumhitsOrientation1Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation1Disk1");
	nMinus1LocalPosEfficiencyOrientation1Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation1Disk1");
	nMinus1LocalPosNumhitsOrientation2Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation2Disk1");
	nMinus1LocalPosEfficiencyOrientation2Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation2Disk1");
	nMinus1LocalPosNumhitsOrientation3Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation3Disk1");
	nMinus1LocalPosEfficiencyOrientation3Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation3Disk1");
	nMinus1LocalPosNumhitsOrientation4Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation4Disk1");
	nMinus1LocalPosEfficiencyOrientation4Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation4Disk1");
	nMinus1LocalPosNumhitsOrientation5Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation5Disk1");
	nMinus1LocalPosEfficiencyOrientation5Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation5Disk1");
	nMinus1LocalPosNumhitsOrientation6Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation6Disk1");
	nMinus1LocalPosEfficiencyOrientation6Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation6Disk1");
	nMinus1LocalPosNumhitsOrientation7Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation7Disk1");
	nMinus1LocalPosEfficiencyOrientation7Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation7Disk1");
	nMinus1LocalPosNumhitsOrientation8Disk1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation8Disk1");
	nMinus1LocalPosEfficiencyOrientation8Disk1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation8Disk1");
	localPosNumhitsOrientation1Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation1Disk2PreCuts");
	localPosEfficiencyOrientation1Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation1Disk2PreCuts");
	localPosNumhitsOrientation2Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation2Disk2PreCuts");
	localPosEfficiencyOrientation2Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation2Disk2PreCuts");
	localPosNumhitsOrientation3Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation3Disk2PreCuts");
	localPosEfficiencyOrientation3Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation3Disk2PreCuts");
	localPosNumhitsOrientation4Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation4Disk2PreCuts");
	localPosEfficiencyOrientation4Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation4Disk2PreCuts");
	localPosNumhitsOrientation5Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation5Disk2PreCuts");
	localPosEfficiencyOrientation5Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation5Disk2PreCuts");
	localPosNumhitsOrientation6Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation6Disk2PreCuts");
	localPosEfficiencyOrientation6Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation6Disk2PreCuts");
	localPosNumhitsOrientation7Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation7Disk2PreCuts");
	localPosEfficiencyOrientation7Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation7Disk2PreCuts");
	localPosNumhitsOrientation8Disk2PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation8Disk2PreCuts");
	localPosEfficiencyOrientation8Disk2PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation8Disk2PreCuts");
	nMinus1LocalPosNumhitsOrientation1Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation1Disk2");
	nMinus1LocalPosEfficiencyOrientation1Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation1Disk2");
	nMinus1LocalPosNumhitsOrientation2Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation2Disk2");
	nMinus1LocalPosEfficiencyOrientation2Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation2Disk2");
	nMinus1LocalPosNumhitsOrientation3Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation3Disk2");
	nMinus1LocalPosEfficiencyOrientation3Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation3Disk2");
	nMinus1LocalPosNumhitsOrientation4Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation4Disk2");
	nMinus1LocalPosEfficiencyOrientation4Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation4Disk2");
	nMinus1LocalPosNumhitsOrientation5Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation5Disk2");
	nMinus1LocalPosEfficiencyOrientation5Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation5Disk2");
	nMinus1LocalPosNumhitsOrientation6Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation6Disk2");
	nMinus1LocalPosEfficiencyOrientation6Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation6Disk2");
	nMinus1LocalPosNumhitsOrientation7Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation7Disk2");
	nMinus1LocalPosEfficiencyOrientation7Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation7Disk2");
	nMinus1LocalPosNumhitsOrientation8Disk2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation8Disk2");
	nMinus1LocalPosEfficiencyOrientation8Disk2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation8Disk2");
	localPosNumhitsOrientation1Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation1Disk3PreCuts");
	localPosEfficiencyOrientation1Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation1Disk3PreCuts");
	localPosNumhitsOrientation2Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation2Disk3PreCuts");
	localPosEfficiencyOrientation2Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation2Disk3PreCuts");
	localPosNumhitsOrientation3Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation3Disk3PreCuts");
	localPosEfficiencyOrientation3Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation3Disk3PreCuts");
	localPosNumhitsOrientation4Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation4Disk3PreCuts");
	localPosEfficiencyOrientation4Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation4Disk3PreCuts");
	localPosNumhitsOrientation5Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation5Disk3PreCuts");
	localPosEfficiencyOrientation5Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation5Disk3PreCuts");
	localPosNumhitsOrientation6Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation6Disk3PreCuts");
	localPosEfficiencyOrientation6Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation6Disk3PreCuts");
	localPosNumhitsOrientation7Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation7Disk3PreCuts");
	localPosEfficiencyOrientation7Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation7Disk3PreCuts");
	localPosNumhitsOrientation8Disk3PreCuts    = checkGetHistoFromMap("localPosNumhitsOrientation8Disk3PreCuts");
	localPosEfficiencyOrientation8Disk3PreCuts = checkGetHistoFromMap("localPosEfficiencyOrientation8Disk3PreCuts");
	nMinus1LocalPosNumhitsOrientation1Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation1Disk3");
	nMinus1LocalPosEfficiencyOrientation1Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation1Disk3");
	nMinus1LocalPosNumhitsOrientation2Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation2Disk3");
	nMinus1LocalPosEfficiencyOrientation2Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation2Disk3");
	nMinus1LocalPosNumhitsOrientation3Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation3Disk3");
	nMinus1LocalPosEfficiencyOrientation3Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation3Disk3");
	nMinus1LocalPosNumhitsOrientation4Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation4Disk3");
	nMinus1LocalPosEfficiencyOrientation4Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation4Disk3");
	nMinus1LocalPosNumhitsOrientation5Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation5Disk3");
	nMinus1LocalPosEfficiencyOrientation5Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation5Disk3");
	nMinus1LocalPosNumhitsOrientation6Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation6Disk3");
	nMinus1LocalPosEfficiencyOrientation6Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation6Disk3");
	nMinus1LocalPosNumhitsOrientation7Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation7Disk3");
	nMinus1LocalPosEfficiencyOrientation7Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation7Disk3");
	nMinus1LocalPosNumhitsOrientation8Disk3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsOrientation8Disk3");
	nMinus1LocalPosEfficiencyOrientation8Disk3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyOrientation8Disk3");
	cluDistNumhitsBarrelPreCuts                = checkGetHistoFromMap("cluDistNumhitsBarrelPreCuts");
	cluDistNumhitsForwardPreCuts               = checkGetHistoFromMap("cluDistNumhitsForwardPreCuts");
	hitDistNumhitsBarrelPreCuts                = checkGetHistoFromMap("hitDistNumhitsBarrelPreCuts");
	hitDistNumhitsForwardPreCuts               = checkGetHistoFromMap("hitDistNumhitsForwardPreCuts");
	cosmicsRingNumhits                         = checkGetHistoFromMap("cosmicsRingNumhits");
	cosmicsRingNumhitsWithAssociatedCluster    = checkGetHistoFromMap("cosmicsRingNumhitsWithAssociatedCluster");
	cosmicsRingEffDxyClLessThan0_5             = checkGetHistoFromMap("cosmicsRingEffDxyClLessThan0_5");
	cosmicsRingNumhitsDxyClLessThan1_0         = checkGetHistoFromMap("cosmicsRingNumhitsDxyClLessThan1_0");
	cosmicsRingsAverageDx                      = checkGetHistoFromMap("cosmicsRingsAverageDx");
	cosmicsRingsAverageDy                      = checkGetHistoFromMap("cosmicsRingsAverageDy");
	cosmicsRowVsColDxyClLessThan0_5_total      = checkGetHistoFromMap("cosmicsRowVsColDxyClLessThan0_5_total");
	cosmicsRowVsColDxyClLessThan0_5_barrel     = checkGetHistoFromMap("cosmicsRowVsColDxyClLessThan0_5_barrel");
	cosmicsRowVsColDxyClLessThan0_5_forward    = checkGetHistoFromMap("cosmicsRowVsColDxyClLessThan0_5_forward");
	rechitGlyVsGlx_barrel                      = checkGetHistoFromMap("rechitGlyVsGlx_barrel");
	rechitGlyVsGlx_fwd_disk1                   = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk1");
	rechitGlyVsGlx_positiveZ_fwd_disk1         = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk1");
	rechitGlyVsGlx_negativeZ_fwd_disk1         = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk1");
	rechitGlyVsGlx_fwd_disk2                   = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk2");
	rechitGlyVsGlx_positiveZ_fwd_disk2         = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk2");
	rechitGlyVsGlx_negativeZ_fwd_disk2         = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk2");
	rechitGlyVsGlx_fwd_disk3                   = checkGetHistoFromMap("rechitGlyVsGlx_fwd_disk3");
	rechitGlyVsGlx_positiveZ_fwd_disk3         = checkGetHistoFromMap("rechitGlyVsGlx_positiveZ_fwd_disk3");
	rechitGlyVsGlx_negativeZ_fwd_disk3         = checkGetHistoFromMap("rechitGlyVsGlx_negativeZ_fwd_disk3");
	associatedClusterXDistanceTotal            = checkGetHistoFromMap("associatedClusterXDistanceTotal");
	associatedClusterYDistanceTotal            = checkGetHistoFromMap("associatedClusterYDistanceTotal");
	associatedClusterXDistanceBarrel           = checkGetHistoFromMap("associatedClusterXDistanceBarrel");
	associatedClusterYDistanceBarrel           = checkGetHistoFromMap("associatedClusterYDistanceBarrel");
	associatedClusterXDistanceForward          = checkGetHistoFromMap("associatedClusterXDistanceForward");
	associatedClusterYDistanceForward          = checkGetHistoFromMap("associatedClusterYDistanceForward");
	rechitZ_total                              = checkGetHistoFromMap("rechitZ_total");
	rechitZ_barrel                             = checkGetHistoFromMap("rechitZ_barrel");
	rechitZ_forward                            = checkGetHistoFromMap("rechitZ_forward");
	rechitNormCharge_total                     = checkGetHistoFromMap("rechitNormCharge_total");
	rechitNormCharge_barrel                    = checkGetHistoFromMap("rechitNormCharge_barrel");
	rechitNormCharge_forward                   = checkGetHistoFromMap("rechitNormCharge_forward");
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

TH1* TrajMeasHistogramFillerModule::checkGetHistoFromMap(const HistoMapType& histogramsArg, const std::string& name) try
{
	return histogramsArg.at(name).get();
}
catch(const std::out_of_range& e)
{
	std::cout << error_prompt << e.what() << " in " << __PRETTY_FUNCTION__ << " while looking for: " << name << "." << std::endl;
	exit(-1);
	return nullptr;
}

void TrajMeasHistogramFillerModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts)
{
	if(!cuts) return;
	numHitsHisto -> Fill(xFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void TrajMeasHistogramFillerModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	numHitsHisto -> Fill(xFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void TrajMeasHistogramFillerModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts)
{
	if(!cuts) return;
	numHitsHisto -> Fill(xFill, yFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}
void TrajMeasHistogramFillerModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	numHitsHisto -> Fill(xFill, yFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}
