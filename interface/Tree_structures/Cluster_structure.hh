#ifndef CLUSTER_STRUCTURE_H
#define CLUSTER_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

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
		// int edge;     // set if there is a valid hit
		// int badpix;   // set if there is a valid hit
		// int tworoc;   // set if there is a valid hit
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
			for (size_t j=0; j<1000; j++) adc[j]=pix[j][0]=pix[j][1]=NOVAL_F;
			
			list="x:y:sizeX/I:sizeY:i:edge:badpix:tworoc:size:charge/F:adc[size]";
		};
};

#endif