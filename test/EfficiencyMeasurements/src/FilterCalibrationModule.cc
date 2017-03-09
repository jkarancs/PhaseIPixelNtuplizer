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
	trk_             (trajField_.trk)
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
		onTrkCluOccupancy_l1              = checkGetHistoFromMap("onTrkCluOccupancy_l1");              onTrkCluOccupancy_l2              = checkGetHistoFromMap("onTrkCluOccupancy_l2");              onTrkCluOccupancy_l3              = checkGetHistoFromMap("onTrkCluOccupancy_l3");              onTrkCluOccupancy_l4              = checkGetHistoFromMap("onTrkCluOccupancy_l4");              onTrkCluOccupancy_fwd             = checkGetHistoFromMap("onTrkCluOccupancy_fwd");
		rocNumhitsWithCuts_l1             = checkGetHistoFromMap("rocNumhitsWithCuts_l1");             rocNumhitsWithCuts_l2             = checkGetHistoFromMap("rocNumhitsWithCuts_l2");             rocNumhitsWithCuts_l3             = checkGetHistoFromMap("rocNumhitsWithCuts_l3");             rocNumhitsWithCuts_l4             = checkGetHistoFromMap("rocNumhitsWithCuts_l4");             rocNumhitsWithCuts_fwd            = checkGetHistoFromMap("rocNumhitsWithCuts_fwd");
		rocEfficiencyWithCuts_l1          = checkGetHistoFromMap("rocEfficiencyWithCuts_l1");          rocEfficiencyWithCuts_l2          = checkGetHistoFromMap("rocEfficiencyWithCuts_l2");          rocEfficiencyWithCuts_l3          = checkGetHistoFromMap("rocEfficiencyWithCuts_l3");          rocEfficiencyWithCuts_l4          = checkGetHistoFromMap("rocEfficiencyWithCuts_l4");          rocEfficiencyWithCuts_fwd         = checkGetHistoFromMap("rocEfficiencyWithCuts_fwd");
		vtxNtrkNumhitsPreCuts             = checkGetHistoFromMap("vtxNtrkNumhitsPreCuts");             vtxNtrkEfficiencyPreCuts          = checkGetHistoFromMap("vtxNtrkEfficiencyPreCuts");
		nMinus1VtxNtrkNumhits             = checkGetHistoFromMap("nMinus1VtxNtrkNumhits");             nMinus1VtxNtrkEfficiency          = checkGetHistoFromMap("nMinus1VtxNtrkEfficiency");
		nMinus1PtNumhits                  = checkGetHistoFromMap("nMinus1PtNumhits");                  nMinus1PtEfficiency               = checkGetHistoFromMap("nMinus1PtEfficiency");
		ptNumhitsPreCuts                  = checkGetHistoFromMap("ptNumhitsPreCuts");                  ptEfficiencyPreCuts               = checkGetHistoFromMap("ptEfficiencyPreCuts");
		stripNumhitsPreCuts               = checkGetHistoFromMap("stripNumhitsPreCuts");               stripEfficiencyPreCuts            = checkGetHistoFromMap("stripEfficiencyPreCuts");
		nMinus1stripNumhits               = checkGetHistoFromMap("nMinus1stripNumhits");               nMinus1stripEfficiency            = checkGetHistoFromMap("nMinus1stripEfficiency");
		lxNumhitsPreCuts                  = checkGetHistoFromMap("lxNumhitsPreCuts");                  lxEfficiencyPreCuts               = checkGetHistoFromMap("lxEfficiencyPreCuts");
		nMinus1LxNumhits                  = checkGetHistoFromMap("nMinus1LxNumhits");                  nMinus1LxEfficiency               = checkGetHistoFromMap("nMinus1LxEfficiency");
		lyNumhitsPreCuts                  = checkGetHistoFromMap("lyNumhitsPreCuts");                  lyEfficiencyPreCuts               = checkGetHistoFromMap("lyEfficiencyPreCuts");
		nMinus1LyNumhits                  = checkGetHistoFromMap("nMinus1LyNumhits");                  nMinus1LyEfficiency               = checkGetHistoFromMap("nMinus1LyEfficiency");
		hitDistNumhitsPreCuts             = checkGetHistoFromMap("hitDistNumhitsPreCuts");             hitDistEfficiencyPreCuts          = checkGetHistoFromMap("hitDistEfficiencyPreCuts");
		nMinus1HitDistNumhits             = checkGetHistoFromMap("nMinus1HitDistNumhits");             nMinus1HitDistEfficiency          = checkGetHistoFromMap("nMinus1HitDistEfficiency");
		cluDistNumhitsPreCuts             = checkGetHistoFromMap("cluDistNumhitsPreCuts");             cluDistEfficiencyPreCuts          = checkGetHistoFromMap("cluDistEfficiencyPreCuts");
		nMinus1CluDistNumhits             = checkGetHistoFromMap("nMinus1CluDistNumhits");             nMinus1CluDistEfficiency          = checkGetHistoFromMap("nMinus1CluDistEfficiency");
		d0BarrelNumhitsPreCuts            = checkGetHistoFromMap("d0BarrelNumhitsPreCuts");            d0BarrelEfficiencyPreCuts         = checkGetHistoFromMap("d0BarrelEfficiencyPreCuts");
		nMinus1D0BarrelNumhits            = checkGetHistoFromMap("nMinus1D0BarrelNumhits");            nMinus1D0BarrelEfficiency         = checkGetHistoFromMap("nMinus1D0BarrelEfficiency");
		d0ForwardNumhitsPreCuts           = checkGetHistoFromMap("d0ForwardNumhitsPreCuts");           d0ForwardEfficiencyPreCuts        = checkGetHistoFromMap("d0ForwardEfficiencyPreCuts");
		nMinus1D0ForwardNumhits           = checkGetHistoFromMap("nMinus1D0ForwardNumhits");           nMinus1D0ForwardEfficiency        = checkGetHistoFromMap("nMinus1D0ForwardEfficiency");
		dZBarrelNumhitsPreCuts            = checkGetHistoFromMap("dZBarrelNumhitsPreCuts");            dZBarrelEfficiencyPreCuts         = checkGetHistoFromMap("dZBarrelEfficiencyPreCuts");         dZForwardNumhitsPreCuts           = checkGetHistoFromMap("dZForwardNumhitsPreCuts");           dZForwardEfficiencyPreCuts        = checkGetHistoFromMap("dZForwardEfficiencyPreCuts");
		nMinus1DZBarrelNumhits            = checkGetHistoFromMap("nMinus1DZBarrelNumhits");            nMinus1DZBarrelEfficiency         = checkGetHistoFromMap("nMinus1DZBarrelEfficiency");
		localPosNumhitsLay1PreCuts        = checkGetHistoFromMap("localPosNumhitsLay1PreCuts");        localPosEfficiencyLay1PreCuts     = checkGetHistoFromMap("localPosEfficiencyLay1PreCuts");     localPosNumhitsLay2PreCuts        = checkGetHistoFromMap("localPosNumhitsLay2PreCuts");        localPosEfficiencyLay2PreCuts     = checkGetHistoFromMap("localPosEfficiencyLay2PreCuts");     localPosNumhitsLay3PreCuts        = checkGetHistoFromMap("localPosNumhitsLay3PreCuts");        localPosEfficiencyLay3PreCuts     = checkGetHistoFromMap("localPosEfficiencyLay3PreCuts");     localPosNumhitsLay4PreCuts        = checkGetHistoFromMap("localPosNumhitsLay4PreCuts");        localPosEfficiencyLay4PreCuts     = checkGetHistoFromMap("localPosEfficiencyLay4PreCuts");
		localPosNumhitsForward1PreCuts    = checkGetHistoFromMap("localPosNumhitsForward1PreCuts");    localPosNumhitsForward2PreCuts    = checkGetHistoFromMap("localPosNumhitsForward2PreCuts");    localPosNumhitsForward3PreCuts    = checkGetHistoFromMap("localPosNumhitsForward3PreCuts");    localPosNumhitsForward4PreCuts    = checkGetHistoFromMap("localPosNumhitsForward4PreCuts");    localPosNumhitsForward5PreCuts    = checkGetHistoFromMap("localPosNumhitsForward5PreCuts");    localPosNumhitsForward6PreCuts    = checkGetHistoFromMap("localPosNumhitsForward6PreCuts");    localPosNumhitsForward7PreCuts    = checkGetHistoFromMap("localPosNumhitsForward7PreCuts");    localPosNumhitsForward8PreCuts    = checkGetHistoFromMap("localPosNumhitsForward8PreCuts");
		localPosEfficiencyForward1PreCuts = checkGetHistoFromMap("localPosEfficiencyForward1PreCuts"); localPosEfficiencyForward2PreCuts = checkGetHistoFromMap("localPosEfficiencyForward2PreCuts"); localPosEfficiencyForward3PreCuts = checkGetHistoFromMap("localPosEfficiencyForward3PreCuts"); localPosEfficiencyForward4PreCuts = checkGetHistoFromMap("localPosEfficiencyForward4PreCuts"); localPosEfficiencyForward5PreCuts = checkGetHistoFromMap("localPosEfficiencyForward5PreCuts"); localPosEfficiencyForward6PreCuts = checkGetHistoFromMap("localPosEfficiencyForward6PreCuts"); localPosEfficiencyForward7PreCuts = checkGetHistoFromMap("localPosEfficiencyForward7PreCuts"); localPosEfficiencyForward8PreCuts = checkGetHistoFromMap("localPosEfficiencyForward8PreCuts");
		nMinus1DZForwardNumhits           = checkGetHistoFromMap("nMinus1DZForwardNumhits");           nMinus1DZForwardEfficiency        = checkGetHistoFromMap("nMinus1DZForwardEfficiency");
		nMinus1LocalPosNumhitsLay1        = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay1");        nMinus1LocalPosNumhitsLay2        = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay2");        nMinus1LocalPosNumhitsLay3        = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay3");        nMinus1LocalPosNumhitsLay4        = checkGetHistoFromMap("nMinus1LocalPosNumhitsLay4");
		nMinus1LocalPosEfficiencyLay1     = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay1");     nMinus1LocalPosEfficiencyLay2     = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay2");     nMinus1LocalPosEfficiencyLay3     = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay3");     nMinus1LocalPosEfficiencyLay4     = checkGetHistoFromMap("nMinus1LocalPosEfficiencyLay4");
		nMinus1LocalPosNumhitsForward1    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward1");    nMinus1LocalPosNumhitsForward2    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward2");    nMinus1LocalPosNumhitsForward3    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward3");    nMinus1LocalPosNumhitsForward4    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward4");    nMinus1LocalPosNumhitsForward5    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward5");    nMinus1LocalPosNumhitsForward6    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward6");    nMinus1LocalPosNumhitsForward7    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward7");    nMinus1LocalPosNumhitsForward8    = checkGetHistoFromMap("nMinus1LocalPosNumhitsForward8");
		nMinus1LocalPosEfficiencyForward1 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward1"); nMinus1LocalPosEfficiencyForward2 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward2"); nMinus1LocalPosEfficiencyForward3 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward3"); nMinus1LocalPosEfficiencyForward4 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward4"); nMinus1LocalPosEfficiencyForward5 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward5"); nMinus1LocalPosEfficiencyForward6 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward6"); nMinus1LocalPosEfficiencyForward7 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward7"); nMinus1LocalPosEfficiencyForward8 = checkGetHistoFromMap("nMinus1LocalPosEfficiencyForward8");

 	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void FilterCalibrationModule::fillHistograms()
{
	const float d_cl = sqrt(trajField_.dx_cl * trajField_.dx_cl + trajField_.dy_cl * trajField_.dy_cl);
	const float d_tr = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
	const int        clust_near              = (0 < d_cl) && (d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
	const int        fillEfficiencyCondition = !missing_ || clust_near;
	calculateCuts();
	if(det_ == 0)
	{
		if(layer_ == 1) onTrkCluOccupancy_l1 -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 2) onTrkCluOccupancy_l2 -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 3) onTrkCluOccupancy_l3 -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 4) onTrkCluOccupancy_l4 -> Fill(moduleCoord_, ladderCoord_);
	}
	if(det_ == 1) onTrkCluOccupancy_fwd -> Fill(diskRingCoord_, bladePanelCoord_);
	fillPairs(vtxNtrkNumhitsPreCuts,  vtxNtrkEfficiencyPreCuts, trk_.fromVtxNtrk, fillEfficiencyCondition);
	fillPairs(ptNumhitsPreCuts,       ptEfficiencyPreCuts,      trk_.pt,          fillEfficiencyCondition);
	fillPairs(stripNumhitsPreCuts,    stripEfficiencyPreCuts,   trk_.strip,       fillEfficiencyCondition);
	fillPairs(lxNumhitsPreCuts,       lxEfficiencyPreCuts,      trajField_.lx,    fillEfficiencyCondition);
	fillPairs(lyNumhitsPreCuts,       lyEfficiencyPreCuts,      trajField_.ly,    fillEfficiencyCondition);
	fillPairs(hitDistNumhitsPreCuts,  hitDistEfficiencyPreCuts, d_tr,  fillEfficiencyCondition);
	fillPairs(cluDistNumhitsPreCuts,  cluDistEfficiencyPreCuts, d_cl,  !missing_);
	if(det_ == 0)
	{
		fillPairs(d0BarrelNumhitsPreCuts, d0BarrelEfficiencyPreCuts, trk_.d0, fillEfficiencyCondition);
		fillPairs(dZBarrelNumhitsPreCuts, dZBarrelEfficiencyPreCuts, trk_.dz, fillEfficiencyCondition);
		if(layer_ == 1) fillPairs(localPosNumhitsLay1PreCuts, localPosEfficiencyLay1PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 2) fillPairs(localPosNumhitsLay2PreCuts, localPosEfficiencyLay2PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 3) fillPairs(localPosNumhitsLay3PreCuts, localPosEfficiencyLay3PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 4) fillPairs(localPosNumhitsLay4PreCuts, localPosEfficiencyLay4PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
	}
	if(det_ == 1)
	{
		fillPairs(d0ForwardNumhitsPreCuts, d0ForwardEfficiencyPreCuts, trk_.d0, fillEfficiencyCondition);
		fillPairs(dZForwardNumhitsPreCuts, dZForwardEfficiencyPreCuts, trk_.dz, fillEfficiencyCondition);
		const int panelOrientation = (side_ - 1) * 4 + std::abs(ring_ % 2) * 2 + panel_; // +Z, -Z, ring 1, ring 2, panel 1, panel 2
		if(panelOrientation == 1) fillPairs(localPosNumhitsForward1PreCuts, localPosEfficiencyForward1PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 2) fillPairs(localPosNumhitsForward2PreCuts, localPosEfficiencyForward2PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 3) fillPairs(localPosNumhitsForward3PreCuts, localPosEfficiencyForward3PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 4) fillPairs(localPosNumhitsForward4PreCuts, localPosEfficiencyForward4PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 5) fillPairs(localPosNumhitsForward5PreCuts, localPosEfficiencyForward5PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 6) fillPairs(localPosNumhitsForward6PreCuts, localPosEfficiencyForward6PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 7) fillPairs(localPosNumhitsForward7PreCuts, localPosEfficiencyForward7PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 8) fillPairs(localPosNumhitsForward8PreCuts, localPosEfficiencyForward8PreCuts, trajField_.lx, trajField_.ly, fillEfficiencyCondition);
	}
	fillPairs(nMinus1VtxNtrkNumhits, nMinus1VtxNtrkEfficiency, trk_.fromVtxNtrk, fillEfficiencyCondition,
		{zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	fillPairs(nMinus1PtNumhits, nMinus1PtEfficiency, trk_.pt, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	fillPairs(nMinus1stripNumhits, nMinus1stripEfficiency, trk_.strip, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	{
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
		if(det_ == 0) fillPairs(nMinus1D0BarrelNumhits,  nMinus1D0BarrelEfficiency,  trk_.d0, fillEfficiencyCondition, {cuts_passed});
		if(det_ == 1) fillPairs(nMinus1D0ForwardNumhits, nMinus1D0ForwardEfficiency, trk_.d0, fillEfficiencyCondition, {cuts_passed});
	}
	{
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
		if(det_ == 0) fillPairs(nMinus1DZBarrelNumhits,  nMinus1DZBarrelEfficiency,  trk_.dz, fillEfficiencyCondition, {cuts_passed});
		if(det_ == 1) fillPairs(nMinus1DZForwardNumhits, nMinus1DZForwardEfficiency, trk_.dz, fillEfficiencyCondition, {cuts_passed});
	}
	{
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && valmisCut && hitsepCut;
		fillPairs(nMinus1LxNumhits, nMinus1LxEfficiency, trajField_.lx, fillEfficiencyCondition, {cuts_passed});
		fillPairs(nMinus1LyNumhits, nMinus1LyEfficiency, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
		if(det_ == 0 && !flipped_)
		{
			if(layer_ == 1) fillPairs(nMinus1LocalPosNumhitsLay1, nMinus1LocalPosEfficiencyLay1, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
			if(layer_ == 2) fillPairs(nMinus1LocalPosNumhitsLay2, nMinus1LocalPosEfficiencyLay2, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
			if(layer_ == 3) fillPairs(nMinus1LocalPosNumhitsLay3, nMinus1LocalPosEfficiencyLay3, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
			if(layer_ == 4) fillPairs(nMinus1LocalPosNumhitsLay4, nMinus1LocalPosEfficiencyLay4, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
		}
		if(det_ == 1)
		{
			const int histoIndex = (side_ - 1) * 4 + std::abs(ring_ % 2) * 2 + panel_;
			switch(histoIndex)
			{
				case 1: fillPairs(nMinus1LocalPosNumhitsForward1, nMinus1LocalPosEfficiencyForward1, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 2: fillPairs(nMinus1LocalPosNumhitsForward2, nMinus1LocalPosEfficiencyForward2, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 3: fillPairs(nMinus1LocalPosNumhitsForward3, nMinus1LocalPosEfficiencyForward3, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 4: fillPairs(nMinus1LocalPosNumhitsForward4, nMinus1LocalPosEfficiencyForward4, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 5: fillPairs(nMinus1LocalPosNumhitsForward5, nMinus1LocalPosEfficiencyForward5, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 6: fillPairs(nMinus1LocalPosNumhitsForward6, nMinus1LocalPosEfficiencyForward6, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 7: fillPairs(nMinus1LocalPosNumhitsForward7, nMinus1LocalPosEfficiencyForward7, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
				case 8: fillPairs(nMinus1LocalPosNumhitsForward8, nMinus1LocalPosEfficiencyForward8, trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed}); break;
			}
		}
	}
	fillPairs(nMinus1HitDistNumhits, nMinus1HitDistEfficiency, d_tr, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut});
	fillPairs(nMinus1CluDistNumhits, nMinus1CluDistEfficiency, d_cl, !missing_,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	effCutAll = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	if(effCutAll)
	{
		if(det_ == 1)                fillPairs(rocNumhitsWithCuts_fwd, rocEfficiencyWithCuts_fwd, diskRingCoord_, bladePanelCoord_, fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 1) fillPairs(rocNumhitsWithCuts_l1,  rocEfficiencyWithCuts_l1,  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 2) fillPairs(rocNumhitsWithCuts_l2,  rocEfficiencyWithCuts_l2,  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 3) fillPairs(rocNumhitsWithCuts_l3,  rocEfficiencyWithCuts_l3,  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 4) fillPairs(rocNumhitsWithCuts_l4,  rocEfficiencyWithCuts_l4,  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
	}
	incrementCounters();
	// int after = histograms_.at("nMinus1LocalPosNumhitsLay1") -> GetEntries() - histograms_.at("nMinus1LocalPosEfficiencyLay1") -> GetEntries();
	// if(before != after && before + 1 != after)
	// {
	// 	std::cout << "Before: " << before << std::endl;
	// 	std::cout << "After: " << after  << std::endl;
	// }
	// before = after;
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


void FilterCalibrationModule::fillPairs(TH1* numHitsHisto, TH1* efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	numHitsHisto -> Fill(xFill);
	if(fillEfficiencyCondition) efficiencyHisto -> Fill(xFill);
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
	if(det_ == 0) d0Cut = TRACK_D0_CUT_BARREL_N_MINUS_1_VAL[layer_ - 1] < std::abs(trk_.d0);
	if(det_ == 1) d0Cut = TRACK_D0_CUT_FORWARD_N_MINUS_1_VAL           < std::abs(trk_.d0);
	// Dz cut
	if(det_ == 0) dzCut = TRACK_DZ_CUT_BARREL_N_MINUS_1_VAL  < std::abs(trk_.dz);
	if(det_ == 1) dzCut = TRACK_DZ_CUT_FORWARD_N_MINUS_1_VAL < std::abs(trk_.dz);
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
	// Valmis cut
	valmisCut = trajField_.validhit || trajField_.missing;
	// Hitsep cut
	hitsepCut = MEAS_HITSEP_CUT_N_MINUS_1_VAL < d_tr;
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