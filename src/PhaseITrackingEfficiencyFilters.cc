#include "../interface/PhaseITrackingEfficiencyFilters.h"

PhaseITrackingEfficiencyFilters::PhaseITrackingEfficiencyFilters(EventData* const eventFieldArg, TrajMeasurement* const trajFieldArg, TrackData* const trackFieldArg) :
	eventField(eventFieldArg),
	trajField(trajFieldArg),
	trackField(trackFieldArg) {}

PhaseITrackingEfficiencyFilters::~PhaseITrackingEfficiencyFilters() {}

int PhaseITrackingEfficiencyFilters::nvtxCut()
{
	return 0 <= eventField -> nvtx;
}

int PhaseITrackingEfficiencyFilters::zerobiasCut()
{
	return 1;
}

int PhaseITrackingEfficiencyFilters::federrCut()
{
	return eventField -> federrs_size == 0;
}

int PhaseITrackingEfficiencyFilters::hpCut()
{
	return (trackField -> quality & 4) != 0;
}

int PhaseITrackingEfficiencyFilters::ptCut()
{
	return trackField -> pt > 1.0;
}

int PhaseITrackingEfficiencyFilters::nstripCut()
{
	return trackField -> strip > 10;
}

int PhaseITrackingEfficiencyFilters::d0Cut()
{
	if(trajField -> mod_on.det == 0)
	{
		switch(trajField -> mod_on.layer)
		{
			case 1:
				return std::abs(trackField -> d0 < 0.01); // Layer 1
			case 2:
				return std::abs(trackField -> d0 < 0.02); // Layer 2
			case 3:
				return std::abs(trackField -> d0 < 0.02); // Layer 3
			case 4:
				return std::abs(trackField -> d0 < 0.02); // Layer 4
		}
		return 0;
	}
	if(trajField -> mod_on.det == 1)
	{
		return std::abs(trackField -> d0) < 0.05;         // Forward pixels
	}
	return 0;
}

int PhaseITrackingEfficiencyFilters::dzCut()
{
	if(trajField -> mod_on.det == 0) return std::abs(trackField -> dz) < 0.1;
	if(trajField -> mod_on.det == 1) return std::abs(trackField -> dz) < 0.5;
	return 0;
}

int PhaseITrackingEfficiencyFilters::pixhitCut()
{
	// if(trajField -> mod_on.det == 0)
	// {
	// 	switch(trajField -> mod_on.layer)
	// 	{
	// 		case 1:
	// 			return
	// 				(trackField -> validbpix[1] > 0 && trackField -> validbpix[2] > 0) || 
	// 				(trackField -> validbpix[1] > 0 && trackField -> validfpix[0] > 0) || 
	// 				(trackField -> validfpix[0] > 0 && trackField -> validfpix[1] > 0);
	// 		case 2:
	// 			return
	// 				(trackField -> validbpix[0] > 0 && trackField -> validbpix[2] > 0) || 
	// 				(trackField -> validbpix[0] > 0 && trackField -> validfpix[0] > 0);
	// 		case 3:
	// 			return
	// 				(trackField -> validbpix[0] > 0 && trackField -> validbpix[1] > 0);
	// 		case 4:
	// 			return 1;
	// 	}
	// }
	// if(trajField -> mod_on.det == 1)
	// {
	// 	switch(std::abs(trajField -> mod_on.disk))
	// 	{
	// 		case 1:
	// 			return
	// 				(trackField -> validbpix[0] > 0 && trackField -> validfpix[1] > 0) || 
	// 				(trackField -> validbpix[1] > 0 && trackField -> validfpix[1] > 0);
	// 		case 2:
	// 			return
	// 				(trackField -> validbpix[0] > 0 && trackField -> validfpix[0] > 0);
	// 	}
	// }
	// return 0;
	return 1;
}

int PhaseITrackingEfficiencyFilters::lxFidCut()
{
	// int notEdgeLx     = 1;
	// int notRocEdgeLx = 1;
	// // Pixel barrel
	// if(mod_on.det == 0)
	// {
	// 	switch(trajField -> mod_on.half)
	// 	{
	// 		case 0:
	// 			notEdgeLx = 
	// 				std::abs(trajField -> lx)<0.65;
	// 			break;
	// 		case 1:
	// 			notEdgeLx = 
	// 				trajField -> lx > -0.3 && trajField -> lx < 0.25;
	// 			break;
	// 	}
	// }
	// // Pixel endcap
	// if(mod_on.det == 1)
	// {
	// 	switch(trajField -> mod_on.panel)
	// 	{
	// 		// Pixel endcap, forward direction
	// 		case 1:
	// 			// FIXME: Module is always 1, check blade instead
	// 			switch(trajField -> mod_on.module)
	// 			{
	// 				case 1:
	// 					notEdgeLx = 
	// 						trajField -> lx > -0.15 && trajField -> lx < 0.3;
	// 					break;
	// 				case 2:
	// 					if(std::abs(trajField -> mod_on.disk) == 1)
	// 					{
	// 						notEdgeLx = 
	// 							trajField -> lx > -0.55 && trajField -> lx < 0.6;
	// 					}
	// 					if(std::abs(trajField -> mod_on.disk) == 2)
	// 					{
	// 						notEdgeLx = 
	// 							trajField -> lx > -0.6 && trajField -> lx < 0.3;
	// 					}
	// 					break;
	// 				case 3:
	// 					if(std::abs(trajField -> mod_on.disk) == 1)
	// 					{
	// 						notEdgeLx = 
	// 							trajField -> lx > 0.3 && trajField -> lx < 0.6;
	// 					}
	// 					if(std::abs(trajField -> mod_on.disk) == 2)
	// 					{
	// 						notEdgeLx = 
	// 							trajField -> lx>-0.6 && trajField -> lx < 0.5;
	// 					}
	// 					break;
	// 				case 4:
	// 					notEdgeLx = 
	// 							trajField -> lx > -0.3 && trajField -> lx < 0.15;
	// 					break;
	// 			}
	// 		// Pixel endcap, backwards direction
	// 		case 2:
	// 			switch(trajField -> mod_on.module)
	// 			{
	// 				case 1:
	// 					notEdgeLx = 
	// 						trajField -> lx > -0.55 && trajField -> lx < 0.6;
	// 					break;
	// 				case 2:
	// 					switch(std::abs(trajField -> mod_on.disk))
	// 					{
	// 						case 1:
	// 							notEdgeLx = 
	// 								std::abs(trajField -> mod_on.disk);
	// 							break;
	// 						case 2:
	// 							notEdgeLx = 
	// 								trajField -> lx > -0.6 && trajField -> lx < 0.55;
	// 							break;
	// 					}
	// 					break;
	// 				case 3:
	// 					notEdgeLx = 
	// 						std::abs(trajField -> lx) < 0.55;
	// 					break;
	// 			}
	// 	}


	// // FIXME: look ROC edge geometry up
	// int rocedgeLx = !((trajField -> mod_on.half==0
	// 	||(trajField -> mod_on.det==1&&!(trajField -> mod_on.panel==1&&
	// 		(trajField -> mod_on.module==1||trajField -> mod_on.module==4))))&&
	// 		 std::abs(trajField -> lx)<0.06);

	// return edge_lx && rocedgeLx;
	return 1;
}

int PhaseITrackingEfficiencyFilters::lyFidCut()
{
	return 1;
}

int PhaseITrackingEfficiencyFilters::valmisCut()
{
	return trajField -> validhit || trajField -> missing;
}

int PhaseITrackingEfficiencyFilters::hitSepCut()
{
	// return trajField.hit_near;
	return 1;
}

bool PhaseITrackingEfficiencyFilters::performCuts(uint32_t cutList)
{
	int passedCuts = 1;
	if(cutList & Cuts::nvtx)     passedCuts &= nvtxCut();
	if(cutList & Cuts::zerobias) passedCuts &= zerobiasCut();
	if(cutList & Cuts::federr)   passedCuts &= federrCut();
	if(cutList & Cuts::hp)       passedCuts &= hpCut();
	if(cutList & Cuts::pt)       passedCuts &= ptCut();
	if(cutList & Cuts::nstrip)   passedCuts &= nstripCut();
	if(cutList & Cuts::d0)       passedCuts &= d0Cut();
	if(cutList & Cuts::dz)       passedCuts &= dzCut();
	if(cutList & Cuts::pixhit)   passedCuts &= pixhitCut();
	if(cutList & Cuts::lx_fid)   passedCuts &= lxFidCut();
	if(cutList & Cuts::ly_fid)   passedCuts &= lyFidCut();
	if(cutList & Cuts::valmis)   passedCuts &= valmisCut();
	if(cutList & Cuts::hitsep)   passedCuts &= hitSepCut();
	return passedCuts;
}

bool PhaseITrackingEfficiencyFilters::performAllEfficiencyCuts()
{
	int passedCuts = 1;
	passedCuts &= nvtxCut();
	if(passedCuts ^ 1) {std::cout << "nvtxCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= zerobiasCut();
	if(passedCuts ^ 1) {std::cout << "zerobiasCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= federrCut();
	if(passedCuts ^ 1) {std::cout << "federrCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= hpCut();
	if(passedCuts ^ 1) {std::cout << "hpCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= ptCut();
	if(passedCuts ^ 1) {std::cout << "ptCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= nstripCut();
	if(passedCuts ^ 1) {std::cout << "nstripCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= d0Cut();
	if(passedCuts ^ 1) {std::cout << "d0Cut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= dzCut();
	if(passedCuts ^ 1) {std::cout << "dzCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= pixhitCut();
	if(passedCuts ^ 1) {std::cout << "pixhitCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= lxFidCut();
	if(passedCuts ^ 1) {std::cout << "lxFidCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= lyFidCut();
	if(passedCuts ^ 1) {std::cout << "lyFidCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= valmisCut();
	if(passedCuts ^ 1) {std::cout << "valmisCut not passed." << std::endl; std::cin.get(); return 0;}
	passedCuts &= hitSepCut();
	return passedCuts;	
}