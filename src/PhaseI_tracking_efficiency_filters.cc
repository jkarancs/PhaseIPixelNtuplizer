#include "../interface/PhaseI_tracking_efficiency_filters.h"

// TODO: check if the references are correct, try reference constructor if fails
PhaseI_tracking_efficiency_filters::PhaseI_tracking_efficiency_filters(const EventData& event_field_ref, const TrajMeasurement& traj_field_ref) :
event_field(event_field_ref),
traj_field(traj_field_ref)
{
	// event_field	= event_field_ref;
	// traj_field  = traj_field_ref;
}

PhaseI_tracking_efficiency_filters::~PhaseI_tracking_efficiency_filters() {}

int PhaseI_tracking_efficiency_filters::nvtx_cut()
{
	return 0 <= event_field.nvtx;
}

int PhaseI_tracking_efficiency_filters::zerobias_cut()
{
		return 1;
}

int PhaseI_tracking_efficiency_filters::federr_cut()
{
	return event_field.federrs_size == 0;
}

// TODO: get tracks
int PhaseI_tracking_efficiency_filters::hp_cut()
{
	// return traj_field.trk.quality & 4 != 0;
	return 1;
}

// TODO: get tracks
int PhaseI_tracking_efficiency_filters::pt_cut()
{
	// return traj_field.trk.pt > 1.0;
	return 1;
}

int PhaseI_tracking_efficiency_filters::nstrip_cut()
{
	return traj_field.trk.strip > 10;
}

// TODO: get tracks
int PhaseI_tracking_efficiency_filters::d0_cut()
{
	// return traj_field.mod_on.det == 1 && std::abs(traj_field.trk.d0) < 0.05;
	return 1;
}

// TODO: get tracks
int PhaseI_tracking_efficiency_filters::dz_cut()
{
	// (traj_field.mod_on.det == 0 && std::abs(ntuple_reader_p -> traj_field.trk.dz) < 0.1) || 
	// (traj_field.mod_on.det == 1 && std::abs(ntuple_reader_p -> traj_field.trk.dz) < 0.5);
	return 1;
}

// TODO: get tracks
int PhaseI_tracking_efficiency_filters::pixhit_cut()
{
	if(traj_field.mod_on.det == 0)
	{
		switch(traj_field.mod_on.layer)
		{
			case 1:
				return
					(traj_field.trk.validbpix[1] > 0 && traj_field.trk.validbpix[2] > 0) || 
					(traj_field.trk.validbpix[1] > 0 && traj_field.trk.validfpix[0] > 0) || 
					(traj_field.trk.validfpix[0] > 0 && traj_field.trk.validfpix[1] > 0);
			case 2:
				return
					(traj_field.trk.validbpix[0] > 0 && traj_field.trk.validbpix[2] > 0) || 
					(traj_field.trk.validbpix[0] > 0 && traj_field.trk.validfpix[0] > 0);
			case 3:
				return
					(traj_field.trk.validbpix[0] > 0 && traj_field.trk.validbpix[1] > 0);
			case 4:
				return 1;
		}
	}
	if(traj_field.mod_on.det == 1)
	{
		switch(std::abs(traj_field.mod_on.disk))
		{
			case 1:
				return
					(traj_field.trk.validbpix[0] > 0 && traj_field.trk.validfpix[1] > 0) || 
					(traj_field.trk.validbpix[1] > 0 && traj_field.trk.validfpix[1] > 0);
			case 2:
				return
					(traj_field.trk.validbpix[0] > 0 && traj_field.trk.validfpix[0] > 0);
		}
	}
	return 0;
}

int PhaseI_tracking_efficiency_filters::noscan_cut()
{
	return 1;
}

int PhaseI_tracking_efficiency_filters::goodmod_cut()
{
	return 1;
}

int PhaseI_tracking_efficiency_filters::lx_fid_cut()
{
	// int not_edge_lx     = 1;
	// int not_roc_edge_lx = 1;
	// // Pixel barrel
	// if(mod_on.det == 0)
	// {
	// 	switch(traj_field.mod_on.half)
	// 	{
	// 		case 0:
	// 			not_edge_lx = 
	// 				std::abs(traj_field.lx)<0.65;
	// 			break;
	// 		case 1:
	// 			not_edge_lx = 
	// 				traj_field.lx > -0.3 && traj_field.lx < 0.25;
	// 			break;
	// 	}
	// }
	// // Pixel endcap
	// if(mod_on.det == 1)
	// {
	// 	switch(traj_field.mod_on.panel)
	// 	{
	// 		// Pixel endcap, forward direction
	// 		case 1:
	// 			// FIXME: Module is always 1, check blade instead
	// 			switch(traj_field.mod_on.module)
	// 			{
	// 				case 1:
	// 					not_edge_lx = 
	// 						traj_field.lx > -0.15 && traj_field.lx < 0.3;
	// 					break;
	// 				case 2:
	// 					if(std::abs(traj_field.mod_on.disk) == 1)
	// 					{
	// 						not_edge_lx = 
	// 							traj_field.lx > -0.55 && traj_field.lx < 0.6;
	// 					}
	// 					if(std::abs(traj_field.mod_on.disk) == 2)
	// 					{
	// 						not_edge_lx = 
	// 							traj_field.lx > -0.6 && traj_field.lx < 0.3;
	// 					}
	// 					break;
	// 				case 3:
	// 					if(std::abs(traj_field.mod_on.disk) == 1)
	// 					{
	// 						not_edge_lx = 
	// 							traj_field.lx > 0.3 && traj_field.lx < 0.6;
	// 					}
	// 					if(std::abs(traj_field.mod_on.disk) == 2)
	// 					{
	// 						not_edge_lx = 
	// 							traj_field.lx>-0.6 && traj_field.lx < 0.5;
	// 					}
	// 					break;
	// 				case 4:
	// 					not_edge_lx = 
	// 							traj_field.lx > -0.3 && traj_field.lx < 0.15;
	// 					break;
	// 			}
	// 		// Pixel endcap, backwards direction
	// 		case 2:
	// 			switch(traj_field.mod_on.module)
	// 			{
	// 				case 1:
	// 					not_edge_lx = 
	// 						traj_field.lx > -0.55 && traj_field.lx < 0.6;
	// 					break;
	// 				case 2:
	// 					switch(std::abs(traj_field.mod_on.disk))
	// 					{
	// 						case 1:
	// 							not_edge_lx = 
	// 								std::abs(traj_field.mod_on.disk);
	// 							break;
	// 						case 2:
	// 							not_edge_lx = 
	// 								traj_field.lx > -0.6 && traj_field.lx < 0.55;
	// 							break;
	// 					}
	// 					break;
	// 				case 3:
	// 					not_edge_lx = 
	// 						std::abs(traj_field.lx) < 0.55;
	// 					break;
	// 			}
	// 	}


	// // FIXME: look ROC edge geometry up
	// int rocedge_lx = !((traj_field.mod_on.half==0
	// 	||(traj_field.mod_on.det==1&&!(traj_field.mod_on.panel==1&&
	// 		(traj_field.mod_on.module==1||traj_field.mod_on.module==4))))&&
	// 		 std::abs(traj_field.lx)<0.06);

	// return edge_lx && rocedge_lx;
	return 1;
}

// TODO: implement ly fid cut
int PhaseI_tracking_efficiency_filters::ly_fid_cut()
{
	return 1;
}

int PhaseI_tracking_efficiency_filters::valmis_cut()
{
	return traj_field.validhit || traj_field.missing;
}

// FIXME: Find closest clusters for hits
int PhaseI_tracking_efficiency_filters::hitsep_cut()
{
	// return traj_field.hit_near;
	return 1;
}

bool PhaseI_tracking_efficiency_filters::perform_cuts(uint32_t cut_list)
{
	int passed_cuts = 1;
	if(cut_list & Cuts::nvtx)     passed_cuts &= nvtx_cut();
	if(cut_list & Cuts::zerobias) passed_cuts &= zerobias_cut();
	if(cut_list & Cuts::federr)   passed_cuts &= federr_cut();
	if(cut_list & Cuts::hp)       passed_cuts &= hp_cut();
	if(cut_list & Cuts::pt)       passed_cuts &= pt_cut();
	if(cut_list & Cuts::nstrip)   passed_cuts &= nstrip_cut();
	if(cut_list & Cuts::d0)       passed_cuts &= d0_cut();
	if(cut_list & Cuts::dz)       passed_cuts &= dz_cut();
	if(cut_list & Cuts::pixhit)   passed_cuts &= pixhit_cut();
	if(cut_list & Cuts::noscan)   passed_cuts &= noscan_cut();
	if(cut_list & Cuts::goodmod)  passed_cuts &= goodmod_cut();
	if(cut_list & Cuts::lx_fid)   passed_cuts &= lx_fid_cut();
	if(cut_list & Cuts::ly_fid)   passed_cuts &= ly_fid_cut();
	if(cut_list & Cuts::valmis)   passed_cuts &= valmis_cut();
	if(cut_list & Cuts::hitsep)   passed_cuts &= hitsep_cut();
	return passed_cuts;
}

