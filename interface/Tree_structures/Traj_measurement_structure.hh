#ifndef TRAJ_MEASUREMENT_STRUCTURE_H
#define TRAJ_MEASUREMENT_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

#include <string>

//////////////////////
// Static variables //
//////////////////////

template<class Dummy>
struct TrajMeasData_static_variablse
{
	static std::map<int, std::string> federrortypes;
	static std::string list;
};

#ifdef COMPLETE
template<class Dummy>
std::string TrajMeasData_static_variablse<Dummy>::list =
    "validhit/I:missing:lx/F:ly:res_dx:res_dz:lev:clust_near/I:hit_near:"
    "pass_efcuts:nclu_mod:nclu_roc:npix_mod:npix_roc:alpha/F:beta:dx_cl[2]:"
    "dy_cl[2]:dx_hit:dy_hit:norm_charge:lz:glx:gly:glz:lxmatch:lymatch:i/I:"
    "onedge:inactive:badhit:telescope:telescope_valid:dmodule:dladder:"
    "glmatch/F:lx_err:ly_err:lz_err:lxymatch:res_hit:sig_hit:d_cl[2]";
#else
template<class Dummy>
std::string TrajMeasData_static_variablse<Dummy>::list =
    "validhit/I:missing:lx/F:ly:res_dx:res_dz:lev:clust_near/I:hit_near:"
    "pass_efcuts:nclu_mod:nclu_roc:npix_mod:npix_roc:alpha/F:beta:dx_cl[2]:"
    "dy_cl[2]:dx_hit:dy_hit:norm_charge";
#endif

/////////////////////////////
// TrajMeasData definition //
/////////////////////////////

class TrajMeasData : public TrajMeasData_static_variablse<void>
{
	public:
		// Non-split mode from here - keep order of variables
		int validhit;
		int missing;
		int col;
		int row;
		float lx;
		float ly;
		float glx;
		float gly;
		float glz;
		// float res_dx;
		// float res_dz;
		// float lev;
		// int clust_near;
		// int hit_near;
		// int pass_effcuts;
		// Paired branch (keep order)
		int nclu_mod;
		int nclu_roc;
		int npix_mod;
		int npix_roc;
		float alpha;
		float beta;
		float dx_cl[2];
		float dy_cl[2];
		float dx_hit;
		float dy_hit;
		// From here Split mode (if SPLIT defined)
		float norm_charge;
		float lz;
		float lxmatch;
		float lymatch;
		int i; // serial num of trajectory measurement on the (single) track of the event
		int onedge;
		int inactive;
		int badhit;
		int telescope;
		int telescope_valid;
		int dmodule; // D(propagated hit, valid hit)
		int dladder; // D(propagated hit, valid hit)
		float glmatch;
		float lx_err;
		float ly_err;
		float lz_err;
		float lxymatch;
		float res_hit;
		float sig_hit;
		float d_cl[2];

		TrajMeasData() { init(); }
		void init()
		{
			validhit=NOVAL_I;
			missing=NOVAL_I;
			row=NOVAL_I;
			col=NOVAL_I;
			lx=NOVAL_F;
			ly=NOVAL_F;
			// res_dx=NOVAL_F;
			// res_dz=NOVAL_F;
			// lev=NOVAL_F;
			// clust_near=NOVAL_I;
			// hit_near=NOVAL_I;
			// pass_effcuts=NOVAL_I;
			nclu_mod=NOVAL_I;
			nclu_roc=NOVAL_I;
			npix_mod=NOVAL_I;
			npix_roc=NOVAL_I;
			alpha=NOVAL_F;
			beta=NOVAL_F;
			dx_cl[0]=dx_cl[1]=NOVAL_F;
			dy_cl[0]=dy_cl[1]=NOVAL_F;
			dx_hit=NOVAL_F;
			dy_hit=NOVAL_F;
			lz=NOVAL_F;
			glx=NOVAL_F;
			gly=NOVAL_F;
			glz=NOVAL_F;
			lxmatch=NOVAL_F;
			lymatch=NOVAL_F;
			norm_charge=NOVAL_F;
			i=NOVAL_I;
			onedge=NOVAL_I;
			inactive=NOVAL_I;
			badhit=NOVAL_I;
			telescope=NOVAL_I;
			telescope_valid=NOVAL_I;
			dmodule=NOVAL_I;
			dladder=NOVAL_I;
			glmatch=NOVAL_F;
			lx_err=NOVAL_F;
			ly_err=NOVAL_F;
			lz_err=NOVAL_F;
			lxymatch=NOVAL_F;
			res_hit=NOVAL_F;
			sig_hit=NOVAL_F;
			d_cl[0]=d_cl[1]=NOVAL_F;
		};
};

#endif