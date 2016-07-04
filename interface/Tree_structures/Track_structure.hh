#ifndef TRACK_STRUCTURE_H
#define TRACK_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class TrackData {
 public:
	// Non-split mode from here - keep order of variables
	int validfpix[2]; // valid recHits in Diks1,2 
	int validbpix[3]; // valid recHits in Layer 1,2,3
	int strip; // total valid hits
	int nstripmissing;
	int nstriplost;
	int nstriplayer;
	int quality;
	float d0;
	float dz;
	float pt;
	// From here Split mode (if SPLIT defined)
	float ndof;
	float chi2;
	float eta;
	float phi;
	float theta;
	float p;
	int algo;
	int fromVtx;
	int i;
	int pix; // total valid hits
	int pixhit[2]; // 0: top, 1: bottom
	int validpixhit[2]; // 0: top, 1: bottom
	int fpix[2]; // recHits in Diks1,2 
	int bpix[3]; // recHits in Layer 1,2,3
	int highPurity;
	
	std::string list;
	
	TrackData() { init(); }
	void init() {
		validfpix[0]=validfpix[1]=NOVAL_I;
		validbpix[0]=validbpix[1]=validbpix[2]=NOVAL_I;
		strip=NOVAL_I;
		nstripmissing=NOVAL_I;
		nstriplost=NOVAL_I;
		nstriplayer=NOVAL_I;
		quality=NOVAL_I;
		d0=NOVAL_F;
		dz=NOVAL_F;
		pt=NOVAL_F;
		ndof=NOVAL_F;
		chi2=NOVAL_F;
		eta=NOVAL_F;
		phi=NOVAL_F;
		theta=NOVAL_F;
		p=NOVAL_F;
		algo=NOVAL_I;
		fromVtx=NOVAL_I;
		i=NOVAL_I;
		pix=NOVAL_I;
		pixhit[0]=pixhit[1]=NOVAL_I;
		validpixhit[0]=validpixhit[1]=NOVAL_I;
		fpix[0]=fpix[1]=NOVAL_I;
		bpix[0]=bpix[1]=bpix[2]=NOVAL_I;
		highPurity=NOVAL_I;
#ifdef COMPLETE
		list="validfpix[2]/I:validbpix[3]:strip:nstripmissing:nstriplost:nstriplayer:"
			"quality:d0/F:dz:pt:ndof:chi2:eta:phi:theta:p:algo/I:fromVtx:i:pix:pixhit[2]:"
			"validpixhit[2]:fpix[2]:bpix[3]:highPurity";
#else
		list="validfpix[2]/I:validbpix[3]:strip:nstripmissing:nstriplost:nstriplayer:"
			"quality:d0/F:dz:pt:ndof:chi2:eta:phi";
#endif
	};

	void* get(std::string field_name_p)
	{
		if(field_name_p == "validfpix")     { return &(this -> validfpix); }
		if(field_name_p == "validbpix")     { return &(this -> validbpix); }
		if(field_name_p == "strip")         { return &(this -> strip); }
		if(field_name_p == "nstripmissing") { return &(this -> nstripmissing); }
		if(field_name_p == "nstriplost")    { return &(this -> nstriplost); }
		if(field_name_p == "nstriplayer")   { return &(this -> nstriplayer); }
		if(field_name_p == "quality")       { return &(this -> quality); }
		if(field_name_p == "d0")            { return &(this -> d0); }
		if(field_name_p == "dz")            { return &(this -> dz); }
		if(field_name_p == "pt")            { return &(this -> pt); }
		if(field_name_p == "ndof")          { return &(this -> ndof); }
		if(field_name_p == "chi2")          { return &(this -> chi2); }
		if(field_name_p == "eta")           { return &(this -> eta); }
		if(field_name_p == "phi")           { return &(this -> phi); }
		if(field_name_p == "theta")         { return &(this -> theta); }
		if(field_name_p == "p")             { return &(this -> p); }
		if(field_name_p == "algo")          { return &(this -> algo); }
		if(field_name_p == "fromVtx")       { return &(this -> fromVtx); }
		if(field_name_p == "i")             { return &(this -> i); }
		if(field_name_p == "pix")           { return &(this -> pix); }
		if(field_name_p == "pixhit")        { return &(this -> pixhit); }
		if(field_name_p == "validpixhit")   { return &(this -> validpixhit); }
		if(field_name_p == "fpix")          { return &(this -> fpix); }
		if(field_name_p == "bpix")          { return &(this -> bpix); }
		if(field_name_p == "highPurity")    { return &(this -> highPurity); }
		if(field_name_p == "list")          { return &(this -> list); }
		std::cerr << "Error: TrackData::get() is unable to find the field: " << field_name_p << ". " << std::endl;
		exit(-1);
	}
	
};

#endif