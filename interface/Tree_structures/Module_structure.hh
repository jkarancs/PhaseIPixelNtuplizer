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

//////////////////////
// Static variables //
//////////////////////

template<class Dummy>
struct ModuleData_static_variables
{
	static std::map<int, std::string> federrortypes;
	static std::string list;
};

template<class Dummy>
std::string ModuleData_static_variables<Dummy>::list = "det/I:layer:ladder:module:side:disk:blade:panel:ring:federr";
template<class Dummy>
std::map<int, std::string> ModuleData_static_variables<Dummy>::federrortypes =
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

///////////////////////////
// ModuleData definition //
///////////////////////////

class ModuleData : public ModuleData_static_variables<void>
{
	public:
		int det;
		int layer;
		int ladder;
		int module;
		int side;
		int disk;
		int blade;
		int panel;
		int ring;
		int federr;
		unsigned int rawid;

		ModuleData()
		{
			init();
		}

		void init()
		{
			det    = NOVAL_I;
			layer  = NOVAL_I;
			ladder = NOVAL_I;
			module = NOVAL_I;
			side   = NOVAL_I;
			disk   = NOVAL_I;
			blade  = NOVAL_I;
			panel  = NOVAL_I;
			ring   = NOVAL_I;
			federr = NOVAL_I;
			rawid  = std::abs(NOVAL_I);
		};

		std::string federr_name()
		{
			auto found = federrortypes.find(federr);
			if(found != federrortypes.end())
			{
				return found -> second;
			} 
			return "FED error not interpreted";
		}
};
#endif
