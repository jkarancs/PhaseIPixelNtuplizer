#include "../interface/PhaseITrackingEfficiencyFilters.h"

// TODO: check if the references are correct, try reference constructor if fails
PhaseITrackingEfficiencyFilters::PhaseITrackingEfficiencyFilters(const EventData& eventFieldArg, const TrajMeasurement& trajFieldArg) :
eventField(eventFieldArg),
trajField(trajFieldArg)
{
	// eventField	= eventFieldArg;
	// trajField  = trajFieldArg;
}

PhaseITrackingEfficiencyFilters::~PhaseITrackingEfficiencyFilters() {}

int PhaseITrackingEfficiencyFilters::nvtxCut()
{
	return 0 <= eventField.nvtx;
}

int PhaseITrackingEfficiencyFilters::zerobiasCut()
{
	return 1;
}

int PhaseITrackingEfficiencyFilters::federrCut()
{
	return eventField.federrs_size == 0;
}

int PhaseITrackingEfficiencyFilters::hpCut()
{
	return (trajField.trk.quality & 4) != 0;
}

int PhaseITrackingEfficiencyFilters::ptCut()
{
	return trajField.trk.pt > 1.0;
	// return 1;
}

int PhaseITrackingEfficiencyFilters::nstripCut()
{
	return trajField.trk.strip > 10;
}

int PhaseITrackingEfficiencyFilters::d0Cut()
{
	if(trajField.mod_on.det == 0)
	{
		switch(trajField.mod_on.layer)
		{
			case 1:
				return std::abs(trajField.trk.d0 < 0.01); // Layer 1
			case 2:
				return std::abs(trajField.trk.d0 < 0.02); // Layer 2
			case 3:
				return std::abs(trajField.trk.d0 < 0.02); // Layer 3
			case 4:
				return std::abs(trajField.trk.d0 < 0.02); // Layer 4
		}
		return 0;
	}
	if(trajField.mod_on.det == 1)
	{
		return std::abs(trajField.trk.d0) < 0.05;         // Forward pixels
	}
	return 0;
}

int PhaseITrackingEfficiencyFilters::dzCut()
{
	return 
		(trajField.mod_on.det == 0 && std::abs(trajField.trk.dz) < 0.1) || 
		(trajField.mod_on.det == 1 && std::abs(trajField.trk.dz) < 0.5);
}

int PhaseITrackingEfficiencyFilters::pixhitCut()
{
	// if(trajField.mod_on.det == 0)
	// {
	// 	switch(trajField.mod_on.layer)
	// 	{
	// 		case 1:
	// 			return
	// 				(trajField.trk.validbpix[1] > 0 && trajField.trk.validbpix[2] > 0) || 
	// 				(trajField.trk.validbpix[1] > 0 && trajField.trk.validfpix[0] > 0) || 
	// 				(trajField.trk.validfpix[0] > 0 && trajField.trk.validfpix[1] > 0);
	// 		case 2:
	// 			return
	// 				(trajField.trk.validbpix[0] > 0 && trajField.trk.validbpix[2] > 0) || 
	// 				(trajField.trk.validbpix[0] > 0 && trajField.trk.validfpix[0] > 0);
	// 		case 3:
	// 			return
	// 				(trajField.trk.validbpix[0] > 0 && trajField.trk.validbpix[1] > 0);
	// 		case 4:
	// 			return 1;
	// 	}
	// }
	// if(trajField.mod_on.det == 1)
	// {
	// 	switch(std::abs(trajField.mod_on.disk))
	// 	{
	// 		case 1:
	// 			return
	// 				(trajField.trk.validbpix[0] > 0 && trajField.trk.validfpix[1] > 0) || 
	// 				(trajField.trk.validbpix[1] > 0 && trajField.trk.validfpix[1] > 0);
	// 		case 2:
	// 			return
	// 				(trajField.trk.validbpix[0] > 0 && trajField.trk.validfpix[0] > 0);
	// 	}
	// }
	// return 0;
	return 1;
}

int PhaseITrackingEfficiencyFilters::goodModCut()
{
	return 1;
}

int PhaseITrackingEfficiencyFilters::lxFidCut()
{
	// int notEdgeLx     = 1;
	// int notRocEdgeLx = 1;
	// // Pixel barrel
	// if(mod_on.det == 0)
	// {
	// 	switch(trajField.mod_on.half)
	// 	{
	// 		case 0:
	// 			notEdgeLx = 
	// 				std::abs(trajField.lx)<0.65;
	// 			break;
	// 		case 1:
	// 			notEdgeLx = 
	// 				trajField.lx > -0.3 && trajField.lx < 0.25;
	// 			break;
	// 	}
	// }
	// // Pixel endcap
	// if(mod_on.det == 1)
	// {
	// 	switch(trajField.mod_on.panel)
	// 	{
	// 		// Pixel endcap, forward direction
	// 		case 1:
	// 			// FIXME: Module is always 1, check blade instead
	// 			switch(trajField.mod_on.module)
	// 			{
	// 				case 1:
	// 					notEdgeLx = 
	// 						trajField.lx > -0.15 && trajField.lx < 0.3;
	// 					break;
	// 				case 2:
	// 					if(std::abs(trajField.mod_on.disk) == 1)
	// 					{
	// 						notEdgeLx = 
	// 							trajField.lx > -0.55 && trajField.lx < 0.6;
	// 					}
	// 					if(std::abs(trajField.mod_on.disk) == 2)
	// 					{
	// 						notEdgeLx = 
	// 							trajField.lx > -0.6 && trajField.lx < 0.3;
	// 					}
	// 					break;
	// 				case 3:
	// 					if(std::abs(trajField.mod_on.disk) == 1)
	// 					{
	// 						notEdgeLx = 
	// 							trajField.lx > 0.3 && trajField.lx < 0.6;
	// 					}
	// 					if(std::abs(trajField.mod_on.disk) == 2)
	// 					{
	// 						notEdgeLx = 
	// 							trajField.lx>-0.6 && trajField.lx < 0.5;
	// 					}
	// 					break;
	// 				case 4:
	// 					notEdgeLx = 
	// 							trajField.lx > -0.3 && trajField.lx < 0.15;
	// 					break;
	// 			}
	// 		// Pixel endcap, backwards direction
	// 		case 2:
	// 			switch(trajField.mod_on.module)
	// 			{
	// 				case 1:
	// 					notEdgeLx = 
	// 						trajField.lx > -0.55 && trajField.lx < 0.6;
	// 					break;
	// 				case 2:
	// 					switch(std::abs(trajField.mod_on.disk))
	// 					{
	// 						case 1:
	// 							notEdgeLx = 
	// 								std::abs(trajField.mod_on.disk);
	// 							break;
	// 						case 2:
	// 							notEdgeLx = 
	// 								trajField.lx > -0.6 && trajField.lx < 0.55;
	// 							break;
	// 					}
	// 					break;
	// 				case 3:
	// 					notEdgeLx = 
	// 						std::abs(trajField.lx) < 0.55;
	// 					break;
	// 			}
	// 	}


	// // FIXME: look ROC edge geometry up
	// int rocedgeLx = !((trajField.mod_on.half==0
	// 	||(trajField.mod_on.det==1&&!(trajField.mod_on.panel==1&&
	// 		(trajField.mod_on.module==1||trajField.mod_on.module==4))))&&
	// 		 std::abs(trajField.lx)<0.06);

	// return edge_lx && rocedgeLx;
	return 1;
}

// TODO: implement ly fid cut
int PhaseITrackingEfficiencyFilters::lyFidCut()
{
	return 1;
}

int PhaseITrackingEfficiencyFilters::valmisCut()
{
	return trajField.validhit || trajField.missing;
}

// FIXME: Find closest clusters for hits
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
	if(cutList & Cuts::goodmod)  passedCuts &= goodModCut();
	if(cutList & Cuts::lx_fid)   passedCuts &= lxFidCut();
	if(cutList & Cuts::ly_fid)   passedCuts &= lyFidCut();
	if(cutList & Cuts::valmis)   passedCuts &= valmisCut();
	if(cutList & Cuts::hitsep)   passedCuts &= hitSepCut();
	return passedCuts;
}

