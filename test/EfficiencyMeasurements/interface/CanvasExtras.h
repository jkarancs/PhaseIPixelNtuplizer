#ifndef CANVAS_EXTRAS_H
#define CANVAS_EXTRAS_H

#include "TCanvas.h"
#include "TPaletteAxis.h"
#include "TH2.h"
#include "TList.h"
#include "TColor.h"
#include "TStyle.h"
#include "TText.h"

namespace CanvasExtras
{
	extern const Int_t NRGBs;
	extern const Int_t NCont;
	extern const Int_t nLevels;
	extern Double_t levels[];
	extern Double_t stops[];
	extern Double_t red[];
	extern Double_t green[];
	extern Double_t blue[];

	TCanvas* custom_can_(TH1* h, std::string canname, int gx = 0, int gy = 0,
		int histosize_x = 500, int histosize_y = 500,
		int mar_left = 80, int mar_right = 20, int mar_top = 20, int mar_bottom = 60, 
		int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		bool log=false);
	TCanvas* custom_can_(TH2* h, std::string canname, int gx = 0, int gy = 0,
		int histosize_x = 500, int histosize_y = 500, 
		int mar_left = 80, int mar_right = 120, int mar_top = 20, int mar_bottom = 60, 
		int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		std::string draw="COLZ", bool norm=false, bool log=false);
	void redesignCanvas(TCanvas* canvas, TH1* histogram);
	void redesignCanvas(TCanvas* canvas, TH2* histogram, const std::string& draw="COLZ");
	void setMulticolorColzPalette();
	void setLabelStyleNote(TText& label);
	void setLabelStyleComment(TText& label);
}; // namespace CanvasExtras

#endif