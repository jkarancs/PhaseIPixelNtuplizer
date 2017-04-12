#include "../interface/FilterCalibrationModule.h"

constexpr int                  FilterCalibrationModule::ZEROBIAS_TRIGGER_BIT;
constexpr int                  FilterCalibrationModule::ZEROBIAS_BITMASK;
constexpr int                  FilterCalibrationModule::VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL;
constexpr int                  FilterCalibrationModule::TRACK_QUALITY_HIGH_PURITY_BIT;
constexpr int                  FilterCalibrationModule::TRACK_QUALITY_HIGH_PURITY_MASK;
constexpr float                FilterCalibrationModule::TRACK_PT_CUT_N_MINUS_1_VAL;
constexpr int                  FilterCalibrationModule::TRACK_NSTRIP_CUT_N_MINUS_1_VAL;
constexpr std::array<float, 4> FilterCalibrationModule::TRACK_D0_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                FilterCalibrationModule::TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                FilterCalibrationModule::TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
constexpr float                FilterCalibrationModule::TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
constexpr float                FilterCalibrationModule::MEAS_HITSEP_CUT_N_MINUS_1_VAL;
constexpr float                FilterCalibrationModule::HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL;

FilterCalibrationModule::FilterCalibrationModule(HistoMapType& histogramsArg, const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg): 
	eventField_      (eventFieldArg),
	trajField_       (trajFieldArg),
	det_             (trajField_.mod_on.det),
	layer_           (trajField_.mod_on.layer),
	flipped_         (trajField_.mod_on.flipped),
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
	trk_             (trajField_.trk)
{
	getHistogramsFromHistoMap(histogramsArg);
}

void FilterCalibrationModule::getHistogramsFromHistoMap(HistoMapType& histogramsArg)
{
	auto checkGetHistoFromMap = [&] (const std::string& name) -> TH1*
	{
		try { return histogramsArg.at(name).get(); }
		catch(const std::out_of_range& e)
		{
			std::cout << error_prompt << e.what() << " in " << __PRETTY_FUNCTION__ << " while looking for: " << name << "." << std::endl;
			exit(-1);
			return nullptr;
		}
	};
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
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void FilterCalibrationModule::fillHistograms()
{
	// FIXME: this recalculation should be obsolete now
	const float d_cl = sqrt(trajField_.dx_cl * trajField_.dx_cl + trajField_.dy_cl * trajField_.dy_cl);
	const float d_tr = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
	const float phi  = atan2(gly_, glx_);
	const int        clust_near              = (0 < d_cl) && (d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
	const int        fillEfficiencyCondition = !missing_ || clust_near;
	calculateCuts();
	const int noVtxCut       =            zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	// const int noHpCut        = nvtxCut && zerobiasCut && federrCut          && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	const int noPtCut        = nvtxCut && zerobiasCut && federrCut && hpCut          && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	const int noNStripCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut              && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	const int noD0Cut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut          && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	const int noDZCut        = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut          && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	const int noFidicualsCut = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut                         && valmisCut && hitsepCut;
	const int noHitsepCut    = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut             ;
	fillPairs(vtxNtrkNumhitsPreCuts,  vtxNtrkEfficiencyPreCuts, trk_.fromVtxNtrk, fillEfficiencyCondition                );
	fillPairs(nMinus1VtxNtrkNumhits,  nMinus1VtxNtrkEfficiency, trk_.fromVtxNtrk, fillEfficiencyCondition, noVtxCut      );
	fillPairs(ptNumhitsPreCuts,       ptEfficiencyPreCuts,      trk_.pt,          fillEfficiencyCondition                );
	fillPairs(nMinus1PtNumhits,       nMinus1PtEfficiency,      trk_.pt,          fillEfficiencyCondition, noPtCut       );
	fillPairs(lxNumhitsPreCuts,       lxEfficiencyPreCuts,      lx_,              fillEfficiencyCondition                );
	fillPairs(nMinus1LxNumhits,       nMinus1LxEfficiency,      lx_,              fillEfficiencyCondition, noFidicualsCut);
	fillPairs(lyNumhitsPreCuts,       lyEfficiencyPreCuts,      ly_,              fillEfficiencyCondition                );
	fillPairs(nMinus1LyNumhits,       nMinus1LyEfficiency,      ly_,              fillEfficiencyCondition, noFidicualsCut);
	fillPairs(stripNumhitsPreCuts,    stripEfficiencyPreCuts,   trk_.strip,       fillEfficiencyCondition                );
	fillPairs(nMinus1stripNumhits,    nMinus1stripEfficiency,   trk_.strip,       fillEfficiencyCondition, noNStripCut   );
	fillPairs(hitDistNumhitsPreCuts,  hitDistEfficiencyPreCuts, d_tr,             fillEfficiencyCondition                );
	fillPairs(nMinus1HitDistNumhits,  nMinus1HitDistEfficiency, d_tr,             fillEfficiencyCondition, noHitsepCut   );
	fillPairs(cluDistNumhitsPreCuts,  cluDistEfficiencyPreCuts, d_cl,             !missing_                              );
	fillPairs(nMinus1CluDistNumhits,  nMinus1CluDistEfficiency, d_cl,             !missing_,               effCutAll     );
	if(det_ == 0)
	{
		cluDistNumhitsBarrelPreCuts -> Fill(d_cl);
		hitDistNumhitsBarrelPreCuts -> Fill(d_tr);
		fillPairs(layersDisksNumhits,      layersDisksEfficiency,     layer_,  fillEfficiencyCondition);
		fillPairs(d0BarrelNumhitsPreCuts,  d0BarrelEfficiencyPreCuts, trk_.d0, fillEfficiencyCondition);
		fillPairs(dZBarrelNumhitsPreCuts,  dZBarrelEfficiencyPreCuts, trk_.dz, fillEfficiencyCondition);
		fillPairs(nMinus1D0BarrelNumhits,  nMinus1D0BarrelEfficiency, trk_.d0, fillEfficiencyCondition, noD0Cut);
		fillPairs(nMinus1DZBarrelNumhits,  nMinus1DZBarrelEfficiency, trk_.dz, fillEfficiencyCondition, noDZCut);
		if(layer_ == 1)
		{
			rechitOccupancy_l1 -> Fill(moduleCoord_, ladderCoord_);
			rechitPhiVsZ_l1    -> Fill(glz_, phi);
			fillPairs(rocNumhitsWithCuts_l1,          rocEfficiencyWithCuts_l1,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll     );
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l1, sensorEfficiencyWithCutsPhiVsZ_l1, glz_,         phi,          fillEfficiencyCondition                );
			fillPairs(localPosNumhitsLay1PreCuts,     localPosEfficiencyLay1PreCuts,     lx_,          ly_,          fillEfficiencyCondition                );
			fillPairs(nMinus1LocalPosNumhitsLay1,     nMinus1LocalPosEfficiencyLay1,     lx_,          ly_,          fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lxNumhitsPreCuts_l1,            lxEfficiencyPreCuts_l1,            lx_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LxNumhits_l1,            nMinus1LxEfficiency_l1,            lx_,                        fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lyNumhitsPreCuts_l1,            lyEfficiencyPreCuts_l1,            ly_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LyNumhits_l1,            nMinus1LyEfficiency_l1,            ly_,                        fillEfficiencyCondition, noFidicualsCut);
			// Lx cut valitdations 
			fillPairs(nvtxCutOnlyLxNumhits,   nvtxCutOnlyLxEfficiency,   lx_, fillEfficiencyCondition, nvtxCut);
			fillPairs(hpCutOnlyLxNumhits,     hpCutOnlyLxEfficiency,     lx_, fillEfficiencyCondition, hpCut);
			fillPairs(ptCutOnlyLxNumhits,     ptCutOnlyLxEfficiency,     lx_, fillEfficiencyCondition, ptCut);
			fillPairs(d0CutOnlyLxNumhits,     d0CutOnlyLxEfficiency,     lx_, fillEfficiencyCondition, d0Cut);
			fillPairs(dzCutOnlyLxNumhits,     dzCutOnlyLxEfficiency,     lx_, fillEfficiencyCondition, dzCut);
			fillPairs(pixhitCutOnlyLxNumhits, pixhitCutOnlyLxEfficiency, lx_, fillEfficiencyCondition, pixhitCut);
			fillPairs(hitsepCutOnlyLxNumhits, hitsepCutOnlyLxEfficiency, lx_, fillEfficiencyCondition, hitsepCut);
			if(flipped_ == 0)
			{
				fillPairs(lxNumhitsOuterPreCuts_l1,        lxEfficiencyOuterPreCuts_l1,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsOuter_l1,        nMinus1LxEfficiencyOuter_l1,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsOuterPreCuts_l1,        lyEfficiencyOuterPreCuts_l1,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsOuter_l1,        nMinus1LyEfficiencyOuter_l1,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay1OuterPreCuts, localPosEfficiencyLay1OuterPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay1Outer, nMinus1LocalPosEfficiencyLay1Outer, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(flipped_ == 1)
			{
				fillPairs(lxNumhitsInnerPreCuts_l1,        lxEfficiencyInnerPreCuts_l1,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsInner_l1,        nMinus1LxEfficiencyInner_l1,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsInnerPreCuts_l1,        lyEfficiencyInnerPreCuts_l1,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsInner_l1,        nMinus1LyEfficiencyInner_l1,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay1InnerPreCuts, localPosEfficiencyLay1InnerPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay1Inner, nMinus1LocalPosEfficiencyLay1Inner, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
		if(layer_ == 2)
		{
			rechitOccupancy_l2 -> Fill(moduleCoord_, ladderCoord_);
			rechitPhiVsZ_l2    -> Fill(glz_, phi);
			fillPairs(rocNumhitsWithCuts_l2,          rocEfficiencyWithCuts_l2,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll     );
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l2, sensorEfficiencyWithCutsPhiVsZ_l2, glz_,         phi,          fillEfficiencyCondition                );
			fillPairs(localPosNumhitsLay2PreCuts,     localPosEfficiencyLay2PreCuts,     lx_,          ly_,          fillEfficiencyCondition                );
			fillPairs(nMinus1LocalPosNumhitsLay2,     nMinus1LocalPosEfficiencyLay2,     lx_,          ly_,          fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lxNumhitsPreCuts_l2,            lxEfficiencyPreCuts_l2,            lx_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LxNumhits_l2,            nMinus1LxEfficiency_l2,            lx_,                        fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lyNumhitsPreCuts_l2,            lyEfficiencyPreCuts_l2,            ly_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LyNumhits_l2,            nMinus1LyEfficiency_l2,            ly_,                        fillEfficiencyCondition, noFidicualsCut);
			if(flipped_ == 0)
			{
				fillPairs(lxNumhitsOuterPreCuts_l2,        lxEfficiencyOuterPreCuts_l2,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsOuter_l2,        nMinus1LxEfficiencyOuter_l2,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsOuterPreCuts_l2,        lyEfficiencyOuterPreCuts_l2,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsOuter_l2,        nMinus1LyEfficiencyOuter_l2,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay2OuterPreCuts, localPosEfficiencyLay2OuterPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay2Outer, nMinus1LocalPosEfficiencyLay2Outer, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(flipped_ == 1)
			{
				fillPairs(lxNumhitsInnerPreCuts_l2,        lxEfficiencyInnerPreCuts_l2,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsInner_l2,        nMinus1LxEfficiencyInner_l2,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsInnerPreCuts_l2,        lyEfficiencyInnerPreCuts_l2,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsInner_l2,        nMinus1LyEfficiencyInner_l2,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay2InnerPreCuts, localPosEfficiencyLay2InnerPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay2Inner, nMinus1LocalPosEfficiencyLay2Inner, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
		if(layer_ == 3)
		{
			rechitOccupancy_l3 -> Fill(moduleCoord_, ladderCoord_);
			rechitPhiVsZ_l3    -> Fill(glz_, phi);
			fillPairs(rocNumhitsWithCuts_l3,          rocEfficiencyWithCuts_l3,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll     );
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l3, sensorEfficiencyWithCutsPhiVsZ_l3, glz_,         phi,          fillEfficiencyCondition                );
			fillPairs(localPosNumhitsLay3PreCuts,     localPosEfficiencyLay3PreCuts,     lx_,          ly_,          fillEfficiencyCondition                );
			fillPairs(nMinus1LocalPosNumhitsLay3,     nMinus1LocalPosEfficiencyLay3,     lx_,          ly_,          fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lxNumhitsPreCuts_l3,            lxEfficiencyPreCuts_l3,            lx_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LxNumhits_l3,            nMinus1LxEfficiency_l3,            lx_,                        fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lyNumhitsPreCuts_l3,            lyEfficiencyPreCuts_l3,            ly_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LyNumhits_l3,            nMinus1LyEfficiency_l3,            ly_,                        fillEfficiencyCondition, noFidicualsCut);
			if(flipped_ == 0)
			{
				fillPairs(lxNumhitsOuterPreCuts_l3,        lxEfficiencyOuterPreCuts_l3,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsOuter_l3,        nMinus1LxEfficiencyOuter_l3,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsOuterPreCuts_l3,        lyEfficiencyOuterPreCuts_l3,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsOuter_l3,        nMinus1LyEfficiencyOuter_l3,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay3OuterPreCuts, localPosEfficiencyLay3OuterPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay3Outer, nMinus1LocalPosEfficiencyLay3Outer, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(flipped_ == 1)
			{
				fillPairs(lxNumhitsInnerPreCuts_l3,        lxEfficiencyInnerPreCuts_l3,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsInner_l3,        nMinus1LxEfficiencyInner_l3,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsInnerPreCuts_l3,        lyEfficiencyInnerPreCuts_l3,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsInner_l3,        nMinus1LyEfficiencyInner_l3,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay3InnerPreCuts, localPosEfficiencyLay3InnerPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay3Inner, nMinus1LocalPosEfficiencyLay3Inner, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
		if(layer_ == 4)
		{
			rechitOccupancy_l4 -> Fill(moduleCoord_, ladderCoord_);
			rechitPhiVsZ_l4    -> Fill(glz_, phi);
			fillPairs(rocNumhitsWithCuts_l4,          rocEfficiencyWithCuts_l4,          moduleCoord_, ladderCoord_, fillEfficiencyCondition, effCutAll     );
			fillPairs(sensorNumhitsWithCutsPhiVsZ_l4, sensorEfficiencyWithCutsPhiVsZ_l4, glz_,         phi,          fillEfficiencyCondition                );
			fillPairs(localPosNumhitsLay4PreCuts,     localPosEfficiencyLay4PreCuts,     lx_,          ly_,          fillEfficiencyCondition                );
			fillPairs(nMinus1LocalPosNumhitsLay4,     nMinus1LocalPosEfficiencyLay4,     lx_,          ly_,          fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lxNumhitsPreCuts_l4,            lxEfficiencyPreCuts_l4,            lx_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LxNumhits_l4,            nMinus1LxEfficiency_l4,            lx_,                        fillEfficiencyCondition, noFidicualsCut);
			fillPairs(lyNumhitsPreCuts_l4,            lyEfficiencyPreCuts_l4,            ly_,                        fillEfficiencyCondition                );
			fillPairs(nMinus1LyNumhits_l4,            nMinus1LyEfficiency_l4,            ly_,                        fillEfficiencyCondition, noFidicualsCut);
			if(flipped_ == 0)
			{
				fillPairs(lxNumhitsOuterPreCuts_l4,        lxEfficiencyOuterPreCuts_l4,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsOuter_l4,        nMinus1LxEfficiencyOuter_l4,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsOuterPreCuts_l4,        lyEfficiencyOuterPreCuts_l4,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsOuter_l4,        nMinus1LyEfficiencyOuter_l4,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay4OuterPreCuts, localPosEfficiencyLay4OuterPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay4Outer, nMinus1LocalPosEfficiencyLay4Outer, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(flipped_ == 1)
			{
				fillPairs(lxNumhitsInnerPreCuts_l4,        lxEfficiencyInnerPreCuts_l4,        lx_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LxNumhitsInner_l4,        nMinus1LxEfficiencyInner_l4,        lx_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(lyNumhitsInnerPreCuts_l4,        lyEfficiencyInnerPreCuts_l4,        ly_,      fillEfficiencyCondition                );
				fillPairs(nMinus1LyNumhitsInner_l4,        nMinus1LyEfficiencyInner_l4,        ly_,      fillEfficiencyCondition, noFidicualsCut);
				fillPairs(localPosNumhitsLay4InnerPreCuts, localPosEfficiencyLay4InnerPreCuts, lx_, ly_, fillEfficiencyCondition                );
				fillPairs(nMinus1LocalPosNumhitsLay4Inner, nMinus1LocalPosEfficiencyLay4Inner, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
	}
	if(det_ == 1)
	{
		cluDistNumhitsForwardPreCuts -> Fill(d_cl);
		hitDistNumhitsForwardPreCuts -> Fill(d_tr);
		const int panelOrientation = (side_ - 1) * 4 + std::abs(ring_ % 2) * 2 + panel_; // +Z, -Z, ring 1, ring 2, panel 1, panel 2
		const int absDisk          = std::abs(disk_);
		rechitOccupancy_fwd -> Fill(diskRingCoord_, bladePanelCoord_);
		rechitPhiVsZ_fwd -> Fill(glz_, phi);
		fillPairs(layersDisksNumhits,              layersDisksEfficiency,              absDisk + 4,                      fillEfficiencyCondition                );
		fillPairs(sensorNumhitsWithCutsPhiVsZ_fwd, sensorEfficiencyWithCutsPhiVsZ_fwd, glz_,           phi,              fillEfficiencyCondition                );
		fillPairs(d0ForwardNumhitsPreCuts,         d0ForwardEfficiencyPreCuts,         trk_.d0,                          fillEfficiencyCondition                );
		fillPairs(dZForwardNumhitsPreCuts,         dZForwardEfficiencyPreCuts,         trk_.dz,                          fillEfficiencyCondition                );
		fillPairs(nMinus1D0ForwardNumhits,         nMinus1D0ForwardEfficiency,         trk_.d0,                          fillEfficiencyCondition, noD0Cut       );
		fillPairs(nMinus1DZForwardNumhits,         nMinus1DZForwardEfficiency,         trk_.dz,                          fillEfficiencyCondition, noDZCut       );
		fillPairs(lxNumhitsPreCuts_fwd,            lxEfficiencyPreCuts_fwd,            lx_,                              fillEfficiencyCondition                );
		fillPairs(nMinus1LxNumhits_fwd,            nMinus1LxEfficiency_fwd,            lx_,                              fillEfficiencyCondition, noFidicualsCut);
		fillPairs(lyNumhitsPreCuts_fwd,            lyEfficiencyPreCuts_fwd,            ly_,                              fillEfficiencyCondition                );
		fillPairs(nMinus1LyNumhits_fwd,            nMinus1LyEfficiency_fwd,            ly_,                              fillEfficiencyCondition, noFidicualsCut);
		fillPairs(rocNumhitsWithCuts_fwd,          rocEfficiencyWithCuts_fwd,          diskRingCoord_, bladePanelCoord_, fillEfficiencyCondition, effCutAll     );
		if(panelOrientation == 1)
		{
			fillPairs(localPosNumhitsForward1PreCuts, localPosEfficiencyForward1PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward1, nMinus1LocalPosEfficiencyForward1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 2)
		{
			fillPairs(localPosNumhitsForward2PreCuts, localPosEfficiencyForward2PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward2, nMinus1LocalPosEfficiencyForward2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 3)
		{
			fillPairs(localPosNumhitsForward3PreCuts, localPosEfficiencyForward3PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward3, nMinus1LocalPosEfficiencyForward3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 4)
		{
			fillPairs(localPosNumhitsForward4PreCuts, localPosEfficiencyForward4PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward4, nMinus1LocalPosEfficiencyForward4, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 5)
		{
			fillPairs(localPosNumhitsForward5PreCuts, localPosEfficiencyForward5PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward5, nMinus1LocalPosEfficiencyForward5, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 6)
		{
			fillPairs(localPosNumhitsForward6PreCuts, localPosEfficiencyForward6PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward6, nMinus1LocalPosEfficiencyForward6, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 7)
		{
			fillPairs(localPosNumhitsForward7PreCuts, localPosEfficiencyForward7PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward7, nMinus1LocalPosEfficiencyForward7, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(panelOrientation == 8)
		{
			fillPairs(localPosNumhitsForward8PreCuts, localPosEfficiencyForward8PreCuts, lx_, ly_, fillEfficiencyCondition);
			fillPairs(nMinus1LocalPosNumhitsForward8, nMinus1LocalPosEfficiencyForward8, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
		}
		if(absDisk == 1)
		{
			if(panelOrientation == 1)
			{
				fillPairs(localPosNumhitsOrientation1Disk1PreCuts, localPosEfficiencyOrientation1Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation1Disk1, nMinus1LocalPosEfficiencyOrientation1Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 2)
			{
				fillPairs(localPosNumhitsOrientation2Disk1PreCuts, localPosEfficiencyOrientation2Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation2Disk1, nMinus1LocalPosEfficiencyOrientation2Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 3)
			{
				fillPairs(localPosNumhitsOrientation3Disk1PreCuts, localPosEfficiencyOrientation3Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation3Disk1, nMinus1LocalPosEfficiencyOrientation3Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 4)
			{
				fillPairs(localPosNumhitsOrientation4Disk1PreCuts, localPosEfficiencyOrientation4Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation4Disk1, nMinus1LocalPosEfficiencyOrientation4Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 5)
			{
				fillPairs(localPosNumhitsOrientation5Disk1PreCuts, localPosEfficiencyOrientation5Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation5Disk1, nMinus1LocalPosEfficiencyOrientation5Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 6)
			{
				fillPairs(localPosNumhitsOrientation6Disk1PreCuts, localPosEfficiencyOrientation6Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation6Disk1, nMinus1LocalPosEfficiencyOrientation6Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 7)
			{
				fillPairs(localPosNumhitsOrientation7Disk1PreCuts, localPosEfficiencyOrientation7Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation7Disk1, nMinus1LocalPosEfficiencyOrientation7Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 8)
			{
				fillPairs(localPosNumhitsOrientation8Disk1PreCuts, localPosEfficiencyOrientation8Disk1PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation8Disk1, nMinus1LocalPosEfficiencyOrientation8Disk1, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
		if(absDisk == 2)
		{
			if(panelOrientation == 1)
			{
				fillPairs(localPosNumhitsOrientation1Disk2PreCuts, localPosEfficiencyOrientation1Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation1Disk2, nMinus1LocalPosEfficiencyOrientation1Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 2)
			{
				fillPairs(localPosNumhitsOrientation2Disk2PreCuts, localPosEfficiencyOrientation2Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation2Disk2, nMinus1LocalPosEfficiencyOrientation2Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 3)
			{
				fillPairs(localPosNumhitsOrientation3Disk2PreCuts, localPosEfficiencyOrientation3Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation3Disk2, nMinus1LocalPosEfficiencyOrientation3Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 4)
			{
				fillPairs(localPosNumhitsOrientation4Disk2PreCuts, localPosEfficiencyOrientation4Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation4Disk2, nMinus1LocalPosEfficiencyOrientation4Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 5)
			{
				fillPairs(localPosNumhitsOrientation5Disk2PreCuts, localPosEfficiencyOrientation5Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation5Disk2, nMinus1LocalPosEfficiencyOrientation5Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 6)
			{
				fillPairs(localPosNumhitsOrientation6Disk2PreCuts, localPosEfficiencyOrientation6Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation6Disk2, nMinus1LocalPosEfficiencyOrientation6Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 7)
			{
				fillPairs(localPosNumhitsOrientation7Disk2PreCuts, localPosEfficiencyOrientation7Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation7Disk2, nMinus1LocalPosEfficiencyOrientation7Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 8)
			{
				fillPairs(localPosNumhitsOrientation8Disk2PreCuts, localPosEfficiencyOrientation8Disk2PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation8Disk2, nMinus1LocalPosEfficiencyOrientation8Disk2, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
		if(absDisk == 3)
		{
			if(panelOrientation == 1)
			{
				fillPairs(localPosNumhitsOrientation1Disk3PreCuts, localPosEfficiencyOrientation1Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation1Disk3, nMinus1LocalPosEfficiencyOrientation1Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 2)
			{
				fillPairs(localPosNumhitsOrientation2Disk3PreCuts, localPosEfficiencyOrientation2Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation2Disk3, nMinus1LocalPosEfficiencyOrientation2Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 3)
			{
				fillPairs(localPosNumhitsOrientation3Disk3PreCuts, localPosEfficiencyOrientation3Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation3Disk3, nMinus1LocalPosEfficiencyOrientation3Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 4)
			{
				fillPairs(localPosNumhitsOrientation4Disk3PreCuts, localPosEfficiencyOrientation4Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation4Disk3, nMinus1LocalPosEfficiencyOrientation4Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 5)
			{
				fillPairs(localPosNumhitsOrientation5Disk3PreCuts, localPosEfficiencyOrientation5Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation5Disk3, nMinus1LocalPosEfficiencyOrientation5Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 6)
			{
				fillPairs(localPosNumhitsOrientation6Disk3PreCuts, localPosEfficiencyOrientation6Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation6Disk3, nMinus1LocalPosEfficiencyOrientation6Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 7)
			{
				fillPairs(localPosNumhitsOrientation7Disk3PreCuts, localPosEfficiencyOrientation7Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation7Disk3, nMinus1LocalPosEfficiencyOrientation7Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
			if(panelOrientation == 8)
			{
				fillPairs(localPosNumhitsOrientation8Disk3PreCuts, localPosEfficiencyOrientation8Disk3PreCuts, lx_, ly_, fillEfficiencyCondition                 );
				fillPairs(nMinus1LocalPosNumhitsOrientation8Disk3, nMinus1LocalPosEfficiencyOrientation8Disk3, lx_, ly_, fillEfficiencyCondition, noFidicualsCut);
			}
		}
	}
	incrementCounters();
}

void FilterCalibrationModule::printCounters()
{
	std::cout << " --- Start cut counter values --- " << std::endl;
	std::cout << "nvtxCut:     " << std::setw(12) << nvtxCut_counter_     << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << nvtxCut_counter_     * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "zerobiasCut: " << std::setw(12) << zerobiasCut_counter_ << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << zerobiasCut_counter_ * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "federrCut:   " << std::setw(12) << federrCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << federrCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "hpCut:       " << std::setw(12) << hpCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << hpCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "ptCut:       " << std::setw(12) << ptCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << ptCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "nstripCut:   " << std::setw(12) << nstripCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << nstripCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "d0Cut:       " << std::setw(12) << d0Cut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << d0Cut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "dzCut:       " << std::setw(12) << dzCut_counter_       << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << dzCut_counter_       * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "pixhitCut:   " << std::setw(12) << pixhitCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << pixhitCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "lxFidCut:    " << std::setw(12) << lxFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lxFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "lyFidCut:    " << std::setw(12) << lyFidCut_counter_    << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << lyFidCut_counter_    * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "valmisCut:   " << std::setw(12) << valmisCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << valmisCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "hitsepCut:   " << std::setw(12) << hitsepCut_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << hitsepCut_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << "total:       " << std::setw(12) << effCutAll_counter_   << " / " << std::setw(12) << entry_counter_ << " (" << std::setprecision(5) << effCutAll_counter_   * 100.0 / entry_counter_<< "%)" << std::endl;
	std::cout << " --- End cut counter values --- " << std::endl;
}

void FilterCalibrationModule::printCutValues()
{
	std::cout << " --- Start cut values --- " << std::endl;
	std::cout << "nvtxCut:     " << nvtxCut     << std::endl;
	std::cout << "zerobiasCut: " << zerobiasCut << std::endl;
	std::cout << "federrCut:   " << federrCut   << std::endl;
	std::cout << "hpCut:       " << hpCut       << std::endl;
	std::cout << "ptCut:       " << ptCut       << std::endl;
	std::cout << "nstripCut:   " << nstripCut   << std::endl;
	std::cout << "d0Cut:       " << d0Cut       << std::endl;
	std::cout << "dzCut:       " << dzCut       << std::endl;
	std::cout << "pixhitCut:   " << pixhitCut   << std::endl;
	std::cout << "lxFidCut:    " << lxFidCut    << std::endl;
	std::cout << "lyFidCut:    " << lyFidCut    << std::endl;
	std::cout << "valmisCut:   " << valmisCut   << std::endl;
	std::cout << "hitsepCut:   " << hitsepCut   << std::endl;
	std::cout << " --- End cut values --- " << std::endl;
}


void FilterCalibrationModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const int& cuts)
{
	if(!cuts) return;
	numHitsHisto -> Fill(xFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void FilterCalibrationModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	numHitsHisto -> Fill(xFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
}
void FilterCalibrationModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const int& cuts)
{
	if(!cuts) return;
	numHitsHisto -> Fill(xFill, yFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}
void FilterCalibrationModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	numHitsHisto -> Fill(xFill, yFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill, yFill);
}

void FilterCalibrationModule::calculateCuts()
{
	const float d_tr = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
	nvtxCut = VERTEX_NUMTRACK_CUT_N_MINUS_1_VAL < trk_.fromVtxNtrk;
	// Zerobias cut
	zerobiasCut = eventField_.trig & ZEROBIAS_BITMASK >> ZEROBIAS_TRIGGER_BIT;
	// Federr cut
	federrCut = eventField_.federrs_size == 0;
	// Hp cut
	hpCut = (trk_.quality & TRACK_QUALITY_HIGH_PURITY_MASK) >> TRACK_QUALITY_HIGH_PURITY_BIT;
	// Pt cut
	ptCut = TRACK_PT_CUT_N_MINUS_1_VAL < trk_.pt;
	// Nstrip cut
	nstripCut = TRACK_NSTRIP_CUT_N_MINUS_1_VAL < trk_.strip;
	// D0 cut
	if(det_ == 0) d0Cut = std::abs(trk_.d0) < TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer_ - 1];
	if(det_ == 1) d0Cut = std::abs(trk_.d0) < TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL;
	// Dz cut
	if(det_ == 0) dzCut = std::abs(trk_.dz) < TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL;
	if(det_ == 1) dzCut = std::abs(trk_.dz) < TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL;
	// Pixhit cut
	if(det_ == 0)
	{
		if(layer_ == 1) pixhitCut =
			(trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0 && trk_.validbpix[3] > 0) ||
			(trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0 && trk_.validfpix[0] > 0) ||
			(trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0) ||
			(trk_.validfpix[0] > 0 && trk_.validfpix[2] > 0 && trk_.validfpix[2] > 0);
		if(layer_ == 2) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validbpix[2] > 0 && trk_.validbpix[3] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validbpix[2] > 0 && trk_.validfpix[0] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0);
		if(layer_ == 3) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[3] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0);
		if(layer_ == 4) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0);
	}
	if(det_ == 1)
	{
		if(std::abs(disk_) == 1) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validbpix[2] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[1] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validfpix[1] > 0 && trk_.validfpix[2] > 0);
		if(std::abs(disk_) == 2) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validbpix[1] > 0 && trk_.validfpix[0] > 0) ||
			(trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[2] > 0);
		if(std::abs(disk_) == 3) pixhitCut =
			(trk_.validbpix[0] > 0 && trk_.validfpix[0] > 0 && trk_.validfpix[1] > 0);
	}
	// Fidicual cuts
	lxFidCut = 1;
	lyFidCut = 1;
	if(det_ == 0)
	{
		lxFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_X_CUT;
		lyFidCut = std::abs(lx_) < BARREL_MODULE_EDGE_Y_CUT;
	}
	// Valmis cut
	valmisCut = trajField_.validhit || trajField_.missing;
	// Hitsep cut
	hitsepCut = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr;
	effCutAll = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
}

void FilterCalibrationModule::incrementCounters()
{
	nvtxCut_counter_     += nvtxCut;
	zerobiasCut_counter_ += zerobiasCut;
	federrCut_counter_   += federrCut;
	hpCut_counter_       += hpCut;
	ptCut_counter_       += ptCut;
	nstripCut_counter_   += nstripCut;
	d0Cut_counter_       += d0Cut;
	dzCut_counter_       += dzCut;
	pixhitCut_counter_   += pixhitCut;
	lxFidCut_counter_    += lxFidCut;
	lyFidCut_counter_    += lyFidCut;
	valmisCut_counter_   += valmisCut;
	hitsepCut_counter_   += hitsepCut;
	effCutAll_counter_   += effCutAll;
	// missing_counter_     += missing;
	// clust_near_counter_  += clust_near;
	entry_counter_++;
}