#ifndef DATASTRUCTURES_CC
#define DATASTRUCTURES_CC
/*
	Creator:         Janos Karancsi, Adam Hunyadi, Viktor Veszpremi

	Description:     Data containers for Phase I Pixel detector

	Comments:        Please, make a new version of this file after each major revision (to be able to track ntuple content)
									 e.g:
			 git add src/DataStructures_v2.cc

			 Please, update the README.md file too (With tested working recipe, stating the working CMSSW version, and ntuple version)
									 Please, also tag corresponding working PhaseIPixelNtuplizer version
			 If you only modify the PhaseIPixelNtuplizer, increase the subversion number in the tag, i.e: v1.1

			 How to do it:
			 - Inside PhaseIPixelNtuplizer.cc
			 #include "../src/DataStructures_v1.cc"

			 cd DPGAnalysis/PhaseIPixelNtuplizer
			 git commit -am "Update: v1.0 Phase I ntuple (CMSSW_8_0_0_pre15), Description: Fixed all trees and most important variables"
			 git tag -a PhaseI_v1.0_CMSSW_8_0_0_pre15 -m "V1 version of Phase I ntuples, tested to work with CMSSW_8_1_0_pre15"
			 git push origin
			 git push origin --tags

	Version history:
	- V1.X - 2016/Nov/04 - First working version, containing most of the required event content for Phase I
*/


#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

#include <TROOT.h>
#include <TTree.h>

#include <map>
#include <string>
#include <utility>
#include <sstream>
#include <iostream>


class RunData
{
	public:
		int fill;
		int run;

		std::string list;

		RunData() { init(); };
		void init()
		{
			fill=NOVAL_I;
			run=NOVAL_I;
			list="fill/I:run";
		};
};



class LumiData
{
	public:
		int fill;
		int run;
		int ls;
		unsigned int time; // Unix time - seconds starting from 1970 Jan 01 00:00
		unsigned int beamint[2];
		float intlumi;
		float instlumi;
		float instlumi_ext;
		float pileup;
		int l1_size;
		int l1_prescale[1000]; // prescale for the L1 trigger with idx

		std::string list;

		LumiData()
		{
			init();
		};
		void init()
		{
			fill = NOVAL_I;
			run = NOVAL_I;
			ls = NOVAL_I;
			time = abs(NOVAL_I);
			beamint[0] = beamint[1] = abs(NOVAL_I);
			intlumi = NOVAL_F;
			instlumi = NOVAL_F;
			instlumi_ext = NOVAL_F;
			pileup = NOVAL_F;
			l1_size = 0;
			for(size_t i = 0; i<1000; i++)
			{
				l1_prescale[i] = NOVAL_I;
			}

			list =  "fill/I:run:ls:time/i:beamint[2]:intlumi/F:instlumi:instlumi_ext:"
							"pileup:l1_size/I:l1_prescale[l1_size]";
		}
};



class EventData
{
	public:
		int fill;
		int run;
		int ls;
		int orb;
		int bx;
		int evt;
		int nvtx;
		int trig;
		int nclu[7]; // [0-3]: layer 1-4, [4-6]: disk 1-3]
		int npix[7]; // [0-3]: layer 1-4, [4-6]: disk 1-3]
		unsigned int beamint[2];
		float l1_rate;
		float intlumi;
		float instlumi;
		float instlumi_ext;
		float pileup;
		float weight;
		float vtxndof;
		float vtxchi2;
		float vtxD0;
		float vtxX;
		float vtxY;
		float vtxZ;
		int vtxntrk;
		int good;
		float tmuon;
		float tmuon_err;
		float tecal;
		float tecal_raw;
		float tecal_err;
		float field;
		int wbc;
		int delay;
		int ntracks;
		int ntrackFPix[3]; // tracks crossing the pixels
		int ntrackBPix[4]; // tracks crossing the pixels
		int ntrackFPixvalid[3]; // tracks crossing the pixels with valid hits
		int ntrackBPixvalid[4]; // tracks crossing the pixels with valid hits
		float trackSep;
		int federrs_size;
		// must be the last variable of the object saved to TTree:
		int federrs[16][2]; // [error index] [0:Nerror, 1:errorType]

		std::string list;

		EventData() { init(); };

		void init()
		{
			fill = NOVAL_I;
			run = NOVAL_I;
			ls = NOVAL_I;
			orb = NOVAL_I;
			bx = NOVAL_I;
			evt = NOVAL_I;
			nvtx = NOVAL_I;
			trig = NOVAL_I;
			for(size_t i = 0; i<7; i++) nclu[i] = npix[i] = NOVAL_I;
			beamint[0] = beamint[1] = abs(NOVAL_I);
			l1_rate = NOVAL_F;
			intlumi = NOVAL_F;
			instlumi = NOVAL_F;
			instlumi_ext = NOVAL_F;
			pileup = NOVAL_F;
			weight = NOVAL_F;
			vtxndof = vtxD0 = vtxZ = NOVAL_F;
			vtxX = vtxY = vtxchi2 = NOVAL_F;
			vtxntrk = NOVAL_I;
			good = NOVAL_I;
			tmuon = NOVAL_F;
			tmuon_err = NOVAL_F;
			tecal = NOVAL_F;
			tecal_raw = NOVAL_F;
			tecal_err = NOVAL_F;
			field = NOVAL_F;
			wbc = NOVAL_I;
			delay = NOVAL_I;
			ntracks = NOVAL_I;
			for(int i=0; i<4; i++)
			{
				ntrackBPix[i]=NOVAL_I;
				ntrackBPixvalid[i]=NOVAL_I;
			}
			for(int i=0; i<3; i++)
			{
				ntrackFPix[i]=NOVAL_I;
				ntrackFPixvalid[i]=NOVAL_I;
			}
			trackSep = NOVAL_F;
			federrs_size = 0;
			for(size_t i = 0; i < 16; i++) federrs[i][0]=federrs[i][1]=NOVAL_I;

			list =  "fill/I:run:ls:orb:bx:evt:nvtx:trig:nclu[7]:npix[7]:beamint[2]/i:"
							"l1_rate/F:intlumi:instlumi:instlumi_ext:pileup:weight:vtxndof:vtxchi2:"
							"vtxD0:vtxX:vtxY:vtxZ:vtxntrk/I:good:tmuon/F:tmuon_err:tecal:tecal_raw:"
							"tecal_err:field:wbc/I:delay:ntracks:ntrackFPix[3]:ntrackBPix[4]:"
							"ntrackFPixvalid[3]:ntrackBPixvalid[4]:trackSep/F:federrs_size/I:"
							"federrs[federrs_size][2]";
		};
};



class ModuleData
{
	public:
		int det;
		int shl;
		int layer;
		int sec;
		int ladder;
		int module;
		int outer;
		int half;
		int side;
		int disk;
		int blade;
		int panel;
		int ring;
		int federr;
		unsigned int rawid;

		std::string list;
		std::map<int, std::string> federrortypes;

		ModuleData() { init(); }

		void init()
		{
			det    = NOVAL_I;
			shl    = NOVAL_I;
			layer  = NOVAL_I;
			sec    = NOVAL_I;
			ladder = NOVAL_I;
			module = NOVAL_I;
			outer  = NOVAL_I;
			half   = NOVAL_I;
			side   = NOVAL_I;
			disk   = NOVAL_I;
			blade  = NOVAL_I;
			panel  = NOVAL_I;
			ring   = NOVAL_I;
			federr = NOVAL_I;
			rawid  = std::abs(NOVAL_I);

			list = "det/I:shl:layer:sec:ladder:module:outer:half:side:disk:blade:panel:ring:federr:rawid/i";

			federrortypes =
			{
				std::pair<int, std::string>(25, "invalidROC"),
				std::pair<int, std::string>(26, "gap word"),
				std::pair<int, std::string>(27, "dummy word"),
				std::pair<int, std::string>(28, "FIFO full error"),
				std::pair<int, std::string>(29, "timeout error"),
				std::pair<int, std::string>(30, "TBM error trailer"),
				std::pair<int, std::string>(31, "event number error (TBM and FED event number mismatch)"),
				std::pair<int, std::string>(32, "incorrectly formatted Slink Header"),
				std::pair<int, std::string>(33, "incorrectly formatted Slink Trailer"),
				std::pair<int, std::string>(34, "the event size encoded in the Slink Trailer is different than the size found at raw to digi conversion "),
				std::pair<int, std::string>(35, "invalid FED channel number"),
				std::pair<int, std::string>(36, "invalid ROC value "),
				std::pair<int, std::string>(37, "invalid dcol or pixel value "),
				std::pair<int, std::string>(38, "the pixels on a ROC weren't read out from lowest to highest row and dcol value"),
				std::pair<int, std::string>(39, "CRC error")
			};
		};

		std::string federr_name()
		{
			auto found = federrortypes.find(federr);
			if(found != federrortypes.end()) return found -> second;
			return "FED error not interpreted";
		}

};



class DigiData
{
	public:
		int i; // serial num of digi in the given module
		int row;
		int col;
		int adc;

		std::string list;

		DigiData() { init(); }
		void init()
		{
			i=NOVAL_I;
			row=NOVAL_I;
			col=NOVAL_I;
			adc=NOVAL_I;
			list="i/I:row:col:adc";
		};
};



class Digi : public DigiData
{
	public:

		ModuleData mod_on; // online module number
		ModuleData mod;    // offline module number

		Digi() { init(); }
		void init()
		{
			DigiData::init();
			mod_on.init();
			mod.init();
		};
};



class ClustData
{
	public:
		// Paired branches (SPLIT mode)
		float x;
		float y;
		int sizeX;
		int sizeY;
		// From here Split mode (if SPLIT defined)
		int i; // serial num of cluster in the given module
		int edge;     // set if there is a valid hit
		int badpix;   // set if there is a valid hit
		int tworoc;   // set if there is a valid hit
		int size;
		float charge;
		// adc must be the last variable of the branch
		float adc[1000];
		float pix[1000][2];

		std::string list;

		ClustData() { init(); }
		void init()
		{
			x=NOVAL_F;
			y=NOVAL_F;
			sizeX=NOVAL_I;
			sizeY=NOVAL_I;
			i=NOVAL_I;
			// edge=NOVAL_I;
			// badpix=NOVAL_I;
			// tworoc=NOVAL_I;
			size=0;
			charge=NOVAL_F;
			for(size_t j=0; j<1000; j++) adc[j]=pix[j][0]=pix[j][1]=NOVAL_F;

			list="x:y:sizeX/I:sizeY:i:edge:badpix:tworoc:size:charge/F:adc[size]:pix[size][2]";
		};
};



class Cluster : public ClustData
{
	public:

		ModuleData mod_on; // online module number
		ModuleData mod;    // offline module number

		Cluster() { init(); };
		void init()
		{
			ClustData::init();
			mod_on.init();
			mod.init();
		}

};



class TrackData
{
	public:
		// Non-split mode from here - keep order of variables
		float pt;
		float eta;
		float phi;
		float theta;
		float p;
		float d0;
		float dz;
		// From here Split mode (if SPLIT defined)
		// float ndof;
		// float chi2;
		// int algo;
		// int fromVtx;
		// int nstripmissing;
		// int nstriplost;
		// int nstriplayer;
		int strip; // total valid hits on the strip detector parts
		int quality;
		int i;
		int pix; // total valid hits on the pixel detector parts
		int pixhit[2]; // 0: top ,1: bottom
		int validpixhit[2]; // 0: top, 1: bottom
		int fpix[3]; // recHits in Diks1,2,3
		int bpix[4]; // recHits in Layer 1,2,3,4
		int validfpix[3]; // valid recHits in Diks1,2,3
		int validbpix[4]; // valid recHits in Layer 1,2,3,4
		// int highPurity;

		std::string list;

		TrackData() { init(); }
		void init()
		{
			pt=NOVAL_F;
			eta=NOVAL_F;
			phi=NOVAL_F;
			theta=NOVAL_F;
			p=NOVAL_F;
			d0=NOVAL_F;
			dz=NOVAL_F;
			// ndof=NOVAL_F;
			// chi2=NOVAL_F;
			// algo=NOVAL_I;
			// fromVtx=NOVAL_I;
			// nstripmissing=NOVAL_I;
			// nstriplost=NOVAL_I;
			// nstriplayer=NOVAL_I;
			strip=NOVAL_I;
			quality=NOVAL_I;
			i=NOVAL_I;
			pix=NOVAL_I;
			pixhit[0]=pixhit[1]=NOVAL_I;
			validpixhit[0]=validpixhit[1]=NOVAL_I;
			fpix[0]=fpix[1]=fpix[2]=NOVAL_I;
			bpix[0]=bpix[1]=bpix[2]=bpix[3]=NOVAL_I;
			validfpix[0]=validfpix[1]=validfpix[2]=NOVAL_I;
			validbpix[0]=validbpix[1]=validbpix[2]=validbpix[3]=NOVAL_I;
			// highPurity=NOVAL_I;
			list = "pt/F:eta:phi:theta:p:d0:dz:strip/I:quality:"
						 "i:pix:pixhit[2]:validpixhit[2]:fpix[3]:bpix[4]:validfpix[3]:validbpix[4]";
		};
};



class TrajMeasData
{
	public:
		// Non-split mode from here - keep order of variables
		int validhit;
		int missing;
		float lx;
		float ly;
		float lz;
		float glx;
		float gly;
		float glz;
		// float res_dx;
		// float res_dz;
		// float lev;
		int clust_near;
		int hit_near;
		int pass_effcuts;
		// Paired branch (keep order)
		// int nclu_mod;
		// int nclu_roc;
		// int npix_mod;
		// int npix_roc;
		float alpha;
		float beta;
		float norm_charge;
		float d_tr;
		float dx_tr;
		float dy_tr;
		float d_cl;
		float dx_cl;
		float dy_cl;
		float dx_hit;
		float dy_hit;
		// // From here Split mode (if SPLIT defined)
		// float lxmatch;
		// float lymatch;
		// int i; // serial num of trajectory measurement on the (single) track of the event
		int onedge;
		// int inactive;
		// int badhit;
		// int telescope;
		// int telescope_valid;
		// int dmodule; // D(propagated hit, valid hit)
		// int dladder; // D(propagated hit, valid hit)
		// float glmatch;
		float lx_err;
		float ly_err;
		// float lz_err;
		// float lxymatch;
		// float res_hit;
		// float sig_hit;

		std::string list;

		TrajMeasData() { init(); }
		void init()
		{
			validhit = NOVAL_I;
			missing  = NOVAL_I;
			lx       = NOVAL_F;
			ly       = NOVAL_F;
			lz       = NOVAL_F;
			glx = NOVAL_F;
			gly = NOVAL_F;
			glz = NOVAL_F;
			// res_dx=NOVAL_F;
			// res_dz=NOVAL_F;
			// lev=NOVAL_F;
			clust_near   = NOVAL_I;
			hit_near     = NOVAL_I;
			pass_effcuts = NOVAL_I;
			// nclu_mod=NOVAL_I;
			// nclu_roc=NOVAL_I;
			// npix_mod=NOVAL_I;
			// npix_roc=NOVAL_I;
			alpha = NOVAL_F;
			beta = NOVAL_F;
			norm_charge=NOVAL_F;
			d_tr  = NOVAL_F;
			dx_tr = NOVAL_F;
			dy_tr = NOVAL_F;
			d_cl  = NOVAL_F;
			dx_cl = NOVAL_F;
			dy_cl = NOVAL_F;
			dx_hit=NOVAL_F;
			dy_hit=NOVAL_F;
			// lxmatch=NOVAL_F;
			// lymatch=NOVAL_F;
			// i=NOVAL_I;
			onedge = NOVAL_I;
			// inactive=NOVAL_I;
			// badhit=NOVAL_I;
			// telescope=NOVAL_I;
			// telescope_valid=NOVAL_I;
			// dmodule=NOVAL_I;
			// dladder=NOVAL_I;
			// glmatch=NOVAL_F;
			lx_err = NOVAL_F;
			ly_err = NOVAL_F;
			// lz_err=NOVAL_F;
			// lxymatch=NOVAL_F;
			// res_hit=NOVAL_F;
			// sig_hit=NOVAL_F;

			list =
				// "validhit/I:missing:lx/F:ly:lz:res_dx:res_dz:lev:clust_near/I:hit_near:"
				// "pass_efcuts:nclu_mod:nclu_roc:npix_mod:npix_roc:alpha/F:beta:lx_err:ly_err:dx_cl[2]:"
				// "dy_cl[2]:dx_hit:dy_hit:norm_charge";
				"validhit/I:missing:lx/F:ly:lz:glx:gly:glz:clust_near/I:hit_near:pass_effcuts:alpha/F:beta:"
				"norm_charge:d_tr:dx_tr:dy_tr:d_cl:dx_cl:dy_cl:dx_hit:dy_hit:onedge/I:lx_err/F:ly_err/F";
		};

};



class TrajMeasurement : public TrajMeasData
{
	public:
		// Extra: TrajMeasData in Traj_measurement_structure.hh
		ModuleData mod_on; // online module number
		ModuleData mod;    // offline module number
		ClustData clu;
		TrackData trk;

		TrajMeasurement() { init(); };

		void init()
		{
			TrajMeasData::init();
			mod_on.init();
			mod.init();
			clu.init();
			trk.init();
		};
};

#endif
