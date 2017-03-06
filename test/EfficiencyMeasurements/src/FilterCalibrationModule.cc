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
	histograms_      (histogramsArg),
	eventField_      (eventFieldArg),
	trajField_       (trajFieldArg),
	det_             (trajField_.mod_on.det),
	layer_           (trajField_.mod_on.layer),
	flipped_         (trajField_.mod_on.flipped),
	disk_            (trajField_.mod_on.disk),
	blade_           (trajField_.mod_on.blade),
	panel_           (trajField_.mod_on.panel),
	ring_            (trajField_.mod_on.ring),
	missing_         (trajField_.missing),
	ladderCoord_     (trajField_.mod_on.ladder_coord),
	moduleCoord_     (trajField_.mod_on.module_coord),
	bladePanelCoord_ (trajField_.mod_on.blade_panel_coord),
	diskRingCoord_   (trajField_.mod_on.disk_ring_coord),
	trk_             (trajField_.trk) {}

void FilterCalibrationModule::checkHistogramDependencies()
{
	for(const auto& key: histogramKeyList_)
	{
		auto findResults = histograms_.find(std::string(key));
		if(findResults == histograms_.end())
		{
			std::cout << error_prompt << "cannot find histogram: " << key << " required by PileupScanModule. Terminating..." << std::endl;
			exit(-1);
		}
	}
	std::cout << process_prompt << __PRETTY_FUNCTION__ << " successful." << std::endl;
}

void FilterCalibrationModule::fillFilterHistograms()
{
	const float d_cl = sqrt(trajField_.dx_cl * trajField_.dx_cl + trajField_.dy_cl * trajField_.dy_cl);
	const float d_tr = sqrt(trajField_.dx_tr * trajField_.dx_tr + trajField_.dy_tr * trajField_.dy_tr);
	const int        clust_near              = (0 < d_cl) && (d_cl < HIT_CLUST_NEAR_CUT_N_MINUS_1_VAL);
	const int        fillEfficiencyCondition = !missing_ || clust_near;
	calculateCuts();
	if(det_ == 0)
	{
		if(layer_ == 1) histograms_.at("onTrkCluOccupancy_l1") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 2) histograms_.at("onTrkCluOccupancy_l2") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 3) histograms_.at("onTrkCluOccupancy_l3") -> Fill(moduleCoord_, ladderCoord_);
		if(layer_ == 4) histograms_.at("onTrkCluOccupancy_l4") -> Fill(moduleCoord_, ladderCoord_);
	}
	if(det_ == 1) histograms_.at("onTrkCluOccupancy_fwd") -> Fill(diskRingCoord_, bladePanelCoord_);
	fillPairs(histograms_, "vtxNtrkNumhitsPreCuts",  "vtxNtrkEfficiencyPreCuts", trk_.fromVtxNtrk, fillEfficiencyCondition);
	fillPairs(histograms_, "ptNumhitsPreCuts",       "ptEfficiencyPreCuts",      trk_.pt,          fillEfficiencyCondition);
	fillPairs(histograms_, "stripNumhitsPreCuts",    "stripEfficiencyPreCuts",   trk_.strip,       fillEfficiencyCondition);
	fillPairs(histograms_, "lxNumhitsPreCuts",       "lxEfficiencyPreCuts",      trajField_.lx,    fillEfficiencyCondition);
	fillPairs(histograms_, "lyNumhitsPreCuts",       "lyEfficiencyPreCuts",      trajField_.ly,    fillEfficiencyCondition);
	fillPairs(histograms_, "hitDistNumhitsPreCuts",  "hitDistEfficiencyPreCuts", d_tr,  fillEfficiencyCondition);
	fillPairs(histograms_, "cluDistNumhitsPreCuts",  "cluDistEfficiencyPreCuts", d_cl,  fillEfficiencyCondition);
	if(det_ == 0)
	{
		fillPairs(histograms_, "d0BarrelNumhitsPreCuts", "d0BarrelEfficiencyPreCuts", trk_.d0, fillEfficiencyCondition);
		fillPairs(histograms_, "dZBarrelNumhitsPreCuts", "dZBarrelEfficiencyPreCuts", trk_.dz, fillEfficiencyCondition);
		if(layer_ == 1) fillPairs(histograms_, "localPosNumhitsLay1PreCuts", "localPosEfficiencyLay1PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 2) fillPairs(histograms_, "localPosNumhitsLay2PreCuts", "localPosEfficiencyLay2PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 3) fillPairs(histograms_, "localPosNumhitsLay3PreCuts", "localPosEfficiencyLay3PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(layer_ == 4) fillPairs(histograms_, "localPosNumhitsLay4PreCuts", "localPosEfficiencyLay4PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
	}
	if(det_ == 1)
	{
		fillPairs(histograms_, "d0ForwardNumhitsPreCuts", "d0ForwardEfficiencyPreCuts", trk_.d0, fillEfficiencyCondition);
		fillPairs(histograms_, "dZForwardNumhitsPreCuts", "dZForwardEfficiencyPreCuts", trk_.dz, fillEfficiencyCondition);
		const int panelOrientation = std::abs(blade_ % 2) * 2 + panel_; // +Z, -Z, panel 1, panel 2, ring 1, ring 2
		if(panelOrientation == 1) fillPairs(histograms_, "localPosNumhitsForward1PreCuts", "localPosEfficiencyForward1PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 2) fillPairs(histograms_, "localPosNumhitsForward2PreCuts", "localPosEfficiencyForward2PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 3) fillPairs(histograms_, "localPosNumhitsForward3PreCuts", "localPosEfficiencyForward3PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
		if(panelOrientation == 4) fillPairs(histograms_, "localPosNumhitsForward4PreCuts", "localPosEfficiencyForward4PreCuts", trajField_.lx, trajField_.ly, fillEfficiencyCondition);
	}
	fillPairs(histograms_, "nMinus1VtxNtrkNumhits", "nMinus1VtxNtrkEfficiency", trk_.fromVtxNtrk, fillEfficiencyCondition,
		{zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	fillPairs(histograms_, "nMinus1PtNumhits", "nMinus1PtEfficiency", trk_.pt, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	fillPairs(histograms_, "nMinus1stripNumhits", "nMinus1stripEfficiency", trk_.strip, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	{
		static const std::array<std::string, 4> names = {"nMinus1D0BarrelNumhits", "nMinus1D0BarrelEfficiency", "nMinus1D0ForwardNumhits", "nMinus1D0ForwardEfficiency"};
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
		fillPairs(histograms_, names[det_ * 2], names[det_ * 2 + 1], trk_.d0, fillEfficiencyCondition, {cuts_passed});
	}
	{
		static const std::array<std::string, 4> names = {"nMinus1DZBarrelNumhits", "nMinus1DZBarrelEfficiency", "nMinus1DZForwardNumhits", "nMinus1DZForwardEfficiency"};
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
		fillPairs(histograms_, names[det_ * 2], names[det_ * 2 + 1], trk_.dz, fillEfficiencyCondition, {cuts_passed});
	}
	{
		const int cuts_passed = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && valmisCut && hitsepCut;
		fillPairs(histograms_, "nMinus1LxNumhits", "nMinus1LxEfficiency", trajField_.lx, fillEfficiencyCondition, {cuts_passed});
		fillPairs(histograms_, "nMinus1LyNumhits", "nMinus1LyEfficiency", trajField_.ly, fillEfficiencyCondition, {cuts_passed});
		if(det_ == 0 && !flipped_)
		{
			static const std::array<std::string, 4> numhitNames     = {"nMinus1LocalPosNumhitsLay1",    "nMinus1LocalPosNumhitsLay2",    "nMinus1LocalPosNumhitsLay3",    "nMinus1LocalPosNumhitsLay4"   };
			static const std::array<std::string, 4> efficiencyNames = {"nMinus1LocalPosEfficiencyLay1", "nMinus1LocalPosEfficiencyLay2", "nMinus1LocalPosEfficiencyLay3", "nMinus1LocalPosEfficiencyLay4"};
			fillPairs(histograms_, numhitNames[layer_ - 1], efficiencyNames[layer_ - 1], trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
		}
		if(det_ == 1)
		{
			static const std::array<std::string, 4> numhitNames     = {"nMinus1LocalPosNumhitsForward1",    "nMinus1LocalPosNumhitsForward2",    "nMinus1LocalPosNumhitsForward3",    "nMinus1LocalPosNumhitsForward4"   };
			static const std::array<std::string, 4> efficiencyNames = {"nMinus1LocalPosEfficiencyForward1", "nMinus1LocalPosEfficiencyForward2", "nMinus1LocalPosEfficiencyForward3", "nMinus1LocalPosEfficiencyForward4"};
			const int histoIndex = std::abs(blade_ % 2) * 2 + panel_;
			if(ring_ == 1)
				fillPairs(histograms_, numhitNames[histoIndex], efficiencyNames[histoIndex], trajField_.lx, trajField_.ly, fillEfficiencyCondition, {cuts_passed});
		}
	}
	fillPairs(histograms_, "nMinus1HitDistNumhits", "nMinus1HitDistEfficiency", d_tr, fillEfficiencyCondition,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut});
	fillPairs(histograms_, "nMinus1CluDistNumhits", "nMinus1CluDistEfficiency", d_cl, !missing_,
		{nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut});
	effCutAll = nvtxCut && zerobiasCut && federrCut && hpCut && ptCut && nstripCut && d0Cut && dzCut && pixhitCut && lxFidCut && lyFidCut && valmisCut && hitsepCut;
	if(effCutAll)
	{
		if(det_ == 1)                fillPairs(histograms_, "rocNumhitsWithCuts_fwd", "rocEfficiencyWithCuts_fwd", diskRingCoord_, bladePanelCoord_, fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 1) fillPairs(histograms_, "rocNumhitsWithCuts_l1",  "rocEfficiencyWithCuts_l1",  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 2) fillPairs(histograms_, "rocNumhitsWithCuts_l2",  "rocEfficiencyWithCuts_l2",  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 3) fillPairs(histograms_, "rocNumhitsWithCuts_l3",  "rocEfficiencyWithCuts_l3",  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
		if(det_ == 0 && layer_ == 4) fillPairs(histograms_, "rocNumhitsWithCuts_l4",  "rocEfficiencyWithCuts_l4",  moduleCoord_,   ladderCoord_,     fillEfficiencyCondition);
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


void FilterCalibrationModule::fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	try { histograms.at(numHitsHisto) -> Fill(xFill); }
	catch(const std::out_of_range& e)
	{
		std::cout << error_prompt << e.what() << " while looking for: " << numHitsHisto << "." << std::endl;
		exit(-1);
	}
	if(fillEfficiencyCondition) try { histograms.at(efficiencyHisto) -> Fill(xFill); }
	catch(const std::out_of_range& e)
	{
		std::cout << error_prompt << e.what() << " while looking for: " << efficiencyHisto << "." << std::endl;
		exit(-1);
	}
}
void FilterCalibrationModule::fillPairs(const std::map<std::string, std::shared_ptr<TH1>>& histograms, const std::string& numHitsHisto, const std::string& efficiencyHisto, const float& xFill, const float& yFill, const int& fillEfficiencyCondition, const std::initializer_list<int>& cuts)
{
	for(const auto& cut: cuts) if(!cut) return;
	try { histograms.at(numHitsHisto) -> Fill(xFill, yFill); }
	catch(const std::out_of_range& e)
	{
		std::cout << error_prompt << e.what() << " while looking for: " << numHitsHisto << "." << std::endl;
		exit(-1);
	}
	if(fillEfficiencyCondition) try { histograms.at(efficiencyHisto) -> Fill(xFill, yFill); }
	catch(const std::out_of_range& e)
	{
		std::cout << error_prompt << e.what() << " while looking for: " << efficiencyHisto << "." << std::endl;
		exit(-1);
	}
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