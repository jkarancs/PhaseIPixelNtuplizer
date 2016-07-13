#ifndef MODULE_STRUCTURE_H
#define MODULE_STRUCTURE_H

#include <map>
#include <string>
#include <utility>
#include <sstream>
#include <iostream>

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class ModuleData {
	public:
		int det;
		int layer;
		int ladder;
		// int half;
		int module;
		int disk;
		int blade;
		int panel;
		
		int federr;
		
		int side;
		int prt;
		int shl;
		int sec;
		
		int outer;
		
		unsigned int rawid;
		
		std::map<int, std::string> federrortypes;
		
		std::string list;
		
		ModuleData() { init(); }
		void init() 
		{
			det=NOVAL_I;
			layer=NOVAL_I;
			ladder=NOVAL_I;
			// half=NOVAL_I;
			module=NOVAL_I;
			disk=NOVAL_I;
			blade=NOVAL_I;
			panel=NOVAL_I;
			federr = NOVAL_I;
			side=NOVAL_I;
			prt=NOVAL_I;
			shl=NOVAL_I;
			sec=NOVAL_I;
			outer=NOVAL_I;
			rawid=abs(NOVAL_I);
			
			
			federrortypes.insert(std::pair<int, std::string>(25, "invalidROC"));
			federrortypes.insert(std::pair<int, std::string>(26, "gap word"));
			federrortypes.insert(std::pair<int, std::string>(27, "dummy word"));
			federrortypes.insert(std::pair<int, std::string>(28, "FIFO full error"));
			federrortypes.insert(std::pair<int, std::string>(29, "timeout error"));
			federrortypes.insert(std::pair<int, std::string>(30, "TBM error trailer"));
			federrortypes.insert(std::pair<int, std::string>(31, "event number error (TBM and FED event number mismatch)"));
			federrortypes.insert(std::pair<int, std::string>(32, "incorrectly formatted Slink Header"));
			federrortypes.insert(std::pair<int, std::string>(33, "incorrectly formatted Slink Trailer"));
			federrortypes.insert(std::pair<int, std::string>(34, "the event size encoded in the Slink Trailer is different than the size found at raw to digi conversion "));
			federrortypes.insert(std::pair<int, std::string>(35, "invalid FED channel number"));
			federrortypes.insert(std::pair<int, std::string>(36, "invalid ROC value "));
			federrortypes.insert(std::pair<int, std::string>(37, "invalid dcol or pixel value "));
			federrortypes.insert(std::pair<int, std::string>(38, "the pixels on a ROC weren't read out from lowest to highest row and dcol value"));
			federrortypes.insert(std::pair<int, std::string>(39, "CRC error"));
#ifdef COMPLETE
			list="det/I:layer:ladder:module:disk:blade:panel:federr:side:prt:shl:"
				"sec:outer:rawid/i";
#else
			list="det/I:layer:ladder:module:disk:blade:panel:federr";
#endif
		};
		
		std::string shell() 
		{
			std::ostringstream ss;
			if(det == 0) 
			{
				ss << "B" << ((module>0) ? "p" : "m") << ((ladder>0) ? "I" : "O");
			} 
			else
			{
				if (det == 1)
				{
					ss << "B" << ((disk>0) ? "p" : "m") << ((blade>0) ? "I" : "O");
				}
			} 
			return ss.str();
		}
		
		int shell_num()
		{
			if(det == 0)
			{
				return ((module>0) ? 0 : 2) + ((ladder>0) ? 0 : 1);
			}
			else
			{
				if(det == 1)
				{
					return ((disk>0) ? 0 : 2) + ((blade>0) ? 0 : 1);
				}
			}
			return -1;
		}
		
		std::string federr_name() 
		{
			std::map<int, std::string>::const_iterator it=federrortypes.find(federr);
			return (it!=federrortypes.end()) ? it->second : "FED error not interpreted";
		}
};

#endif
