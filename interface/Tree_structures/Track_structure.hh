#ifndef TRACK_STRUCTURE_H
#define TRACK_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class TrackData
{
	public:
		// Non-split mode from here - keep order of variables
		int validfpix[2]; // valid recHits in Diks1,2
		int validbpix[4]; // valid recHits in Layer 1,2,3
		int strip; // total valid hits on the strip detector parts
		// int nstripmissing;
		// int nstriplost;
		// int nstriplayer;
		int quality;
		float d0;
		float dz;
		float pt;
		// From here Split mode (if SPLIT defined)
		// float ndof;
		// float chi2;
		float eta;
		float phi;
		float theta;
		float p;
		// int algo;
		// int fromVtx;
		int i;
		int pix; // total valid hits on the pixel detector parts
		int pixhit[2]; // 0: top ,1: bottom
		int validpixhit[2]; // 0: top, 1: bottom
		int fpix[2]; // recHits in Diks1,2
		int bpix[4]; // recHits in Layer 1,2,3
		// int highPurity;

		std::string list;

		TrackData() { init(); }
		void init()
		{
			validfpix[0]=validfpix[1]=NOVAL_I;
			validbpix[0]=validbpix[1]=validbpix[2]=validbpix[3]=NOVAL_I;
			strip=NOVAL_I;
			// nstripmissing=NOVAL_I;
			// nstriplost=NOVAL_I;
			// nstriplayer=NOVAL_I;
			quality=NOVAL_I;
			d0=NOVAL_F;
			dz=NOVAL_F;
			pt=NOVAL_F;
			// ndof=NOVAL_F;
			// chi2=NOVAL_F;
			eta=NOVAL_F;
			phi=NOVAL_F;
			theta=NOVAL_F;
			p=NOVAL_F;
			// algo=NOVAL_I;
			// fromVtx=NOVAL_I;
			i=NOVAL_I;
			pix=NOVAL_I;
			pixhit[0]=pixhit[1]=NOVAL_I;
			validpixhit[0]=validpixhit[1]=NOVAL_I;
			fpix[0]=fpix[1]=NOVAL_I;
			bpix[0]=bpix[1]=bpix[2]=NOVAL_I;
			// highPurity=NOVAL_I;
			list = "validfpix[2]/I:validbpix[4]/I:strip/I:quality/I:d0/F:dz/F:pt/F:eta/F:phi/F:theta/F:p/F:i/I:pix/I:pixhit[2]/I:validpixhit[2]/I:fpix[2]/I:bpix[4]/I";
		};
};

#endif