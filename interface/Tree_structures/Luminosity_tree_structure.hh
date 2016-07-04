#ifndef LUMINOSITY_TREE_STRUCTURE_H
#define LUMINOSITY_TREE_STRUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

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

		list =	"fill/I:run:ls:time/i:beamint[2]:intlumi/F:instlumi:instlumi_ext:"
				"pileup:l1_size/I:l1_prescale[l1_size]";
	}

	void* get(std::string field_name_p)
	{
		if(field_name_p == "fill")         { return &(this -> fill); }
		if(field_name_p == "run")          { return &(this -> run); }
		if(field_name_p == "ls")           { return &(this -> ls); }
		if(field_name_p == "time")         { return &(this -> time); }
		if(field_name_p == "beamint")      { return &(this -> beamint); }
		if(field_name_p == "intlumi")      { return &(this -> intlumi); }
		if(field_name_p == "instlumi")     { return &(this -> instlumi); }
		if(field_name_p == "instlumi_ext") { return &(this -> instlumi_ext); }
		if(field_name_p == "pileup")       { return &(this -> pileup); }
		if(field_name_p == "l1_size")      { return &(this -> l1_size); }
		if(field_name_p == "l1_prescale")  { return &(this -> l1_prescale); }
		std::cerr << "Error: Lumidata::get() is unable to find the field: " << field_name_p << ". " << std::endl;
		exit(-1);
	}
};

#endif