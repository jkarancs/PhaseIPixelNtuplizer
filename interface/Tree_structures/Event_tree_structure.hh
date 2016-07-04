#ifndef EVENT_TREE_STRUCTURE_H
#define EVENT_TREE_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

#include <iostream>
#include <string>

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
		int nclu[4]; // [0: fpix, i: layer i]
		int npix[4]; // [0: fpix, i: layer i]
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
		int ntrackFPix[2]; // tracks crossing the pixels
		int ntrackBPix[3]; // tracks crossing the pixels
		int ntrackFPixvalid[2]; // tracks crossing the pixels with valid hits
		int ntrackBPixvalid[3]; // tracks crossing the pixels with valid hits
		float trackSep;
		int federrs_size;
		// must be the last variable of the object saved to TTree:
		int federrs[16][2]; // [error index] [0:Nerror, 1:errorType]

		std::string list;

		EventData()
		{ 
			this -> init(); 
		};
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
			for (size_t i = 0; i<4; i++) nclu[i] = npix[i] = NOVAL_I;
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
			ntrackFPix[0] = ntrackFPix[1] = NOVAL_I;
			ntrackBPix[0] = ntrackBPix[1] = ntrackBPix[2] = NOVAL_I;
			ntrackFPixvalid[0] = ntrackFPixvalid[1] = NOVAL_I;
			ntrackBPixvalid[0] = ntrackBPixvalid[1] = ntrackBPixvalid[2] = NOVAL_I;
			trackSep = NOVAL_F;
			federrs_size = 0;
			for(size_t i = 0; i < 16; i++) 
			{
				federrs[i][0]=federrs[i][1]=NOVAL_I;
			}
			list = 	"fill/I:run:ls:orb:bx:evt:nvtx:trig:nclu[4]:npix[4]:beamint[2]/i:"
					"l1_rate/F:intlumi:instlumi:instlumi_ext:pileup:weight:vtxndof:vtxchi2:"
					"vtxD0:vtxX:vtxY:vtxZ:vtxntrk/I:good:tmuon/F:tmuon_err:tecal:tecal_raw:"
					"tecal_err:field:wbc/I:delay:ntracks:ntrackFPix[2]:ntrackBPix[3]:"
					"ntrackFPixvalid[2]:ntrackBPixvalid[3]:trackSep/F:federrs_size/I:"
					"federrs[federrs_size][2]";
		};

		void* get(std::string field_name_p)
		{
			if(field_name_p == "fill")            { return &(this -> fill); }
			if(field_name_p == "run")             { return &(this -> run); }
			if(field_name_p == "ls")              { return &(this -> ls); }
			if(field_name_p == "orb")             { return &(this -> orb); }
			if(field_name_p == "bx")              { return &(this -> bx); }
			if(field_name_p == "evt")             { return &(this -> evt); }
			if(field_name_p == "nvtx")            { return &(this -> nvtx); }
			if(field_name_p == "trig")            { return &(this -> trig); }
			if(field_name_p == "nclu")            { return &(this -> nclu); } // [0: fpix, i: layer i]
			if(field_name_p == "npix")            { return &(this -> npix); } // [0: fpix, i: layer i]
			if(field_name_p == "beamint")         { return &(this -> beamint); }
			if(field_name_p == "l1_rate")         { return &(this -> l1_rate); }
			if(field_name_p == "intlumi")         { return &(this -> intlumi); }
			if(field_name_p == "instlumi")        { return &(this -> instlumi); }
			if(field_name_p == "instlumi_ext")    { return &(this -> instlumi_ext); }
			if(field_name_p == "pileup")          { return &(this -> pileup); }
			if(field_name_p == "weight")          { return &(this -> weight); }
			if(field_name_p == "vtxndof")         { return &(this -> vtxndof); }
			if(field_name_p == "vtxchi2")         { return &(this -> vtxchi2); }
			if(field_name_p == "vtxD0")           { return &(this -> vtxD0); }
			if(field_name_p == "vtxX")            { return &(this -> vtxX); }
			if(field_name_p == "vtxY")            { return &(this -> vtxY); }
			if(field_name_p == "vtxZ")            { return &(this -> vtxZ); }
			if(field_name_p == "vtxntrk")         { return &(this -> vtxntrk); }
			if(field_name_p == "good")            { return &(this -> good); }
			if(field_name_p == "tmuon")           { return &(this -> tmuon); }
			if(field_name_p == "tmuon_err")       { return &(this -> tmuon_err); }
			if(field_name_p == "tecal")           { return &(this -> tecal); }
			if(field_name_p == "tecal_raw")       { return &(this -> tecal_raw); }
			if(field_name_p == "tecal_err")       { return &(this -> tecal_err); }
			if(field_name_p == "field")           { return &(this -> field); }
			if(field_name_p == "wbc")             { return &(this -> wbc); }
			if(field_name_p == "delay")           { return &(this -> delay); }
			if(field_name_p == "ntracks")         { return &(this -> ntracks); }
			if(field_name_p == "ntrackFPix")      { return &(this -> ntrackFPix); } // tracks crossing the pixels
			if(field_name_p == "ntrackBPix")      { return &(this -> ntrackBPix); } // tracks crossing the pixels
			if(field_name_p == "ntrackFPixvalid") { return &(this -> ntrackFPixvalid); } // tracks crossing the pixels with valid hits
			if(field_name_p == "ntrackBPixvalid") { return &(this -> ntrackBPixvalid); } // tracks crossing the pixels with valid hits
			if(field_name_p == "trackSep")        { return &(this -> trackSep); }
			if(field_name_p == "federrs_size")    { return &(this -> federrs_size); }
			if(field_name_p == "federrs")         { return &(this -> federrs); } // [error index] [0:Nerror, 1:errorType]
			std::cerr << "Error: EventData::get() is unable to find the field: " << field_name_p << ". " << std::endl;
			exit(-1);
		}
};

#endif