#include "/data/jkarancs/CMSSW/PixelHistoMaker_git/common_functions.h"

void Occup_from_ntuple(std::string filename = "Ntuple*.root", int phase=1, bool save_plot=0) {
  TCanvas *c;

  std::vector<bool> digi_clu_hit = {0, 1, 0};
  int divide_roc = 1;
  bool bpix = 1;
  bool fpix = 1;
  remove_prelim_label = 2;
  //save_dir = "/data/vami/projects/FailureScenarios/CMSSW_8_1_0/src/plotting/"+std::string(filename).replace(filename.find(".root"),5,"")+"/";
  save_dir = "/data/vami/projects/FailureScenarios/CMSSW_8_1_0/src/plotting/";
  //formats = { ".png", ".root"};
  formats = { ".png" };

  std::vector<std::string> trees = {"digiTree","clustTree","trajTree"};
  std::vector<std::string> zaxis = {"Digi","Cluster", "On-track Cluster"};
  std::vector<std::string> varname = {"DigiOccupancy","CluOccupancy", "OnTrkCluOccupancy"};
  for (int itree=0; itree<3; ++itree) if (digi_clu_hit[itree]&&!(phase==1&&itree==0)) {
    TChain* tree = new TChain(trees[itree].c_str());
    tree->Add((std::string("/data/gridout/vami/MC/FailureScenario0/")+filename+"/"+trees[itree]).c_str());
    
    // BPix
    if (bpix) {
      int layer = 0;
      std::vector<std::vector<int> > nlad = { { 10, 16, 22 }, { 6, 14, 22, 32 } };
      //std::vector<std::vector<int> > nlad = { { 10 }, { 6 } };
      for (int lay=1, nlay=nlad[phase].size(); lay<=nlay; ++lay) {
	h2 = new TH2D( (varname[itree]+"_l"+std::to_string(lay)).c_str(),
		       (std::string(";ROC / Module;ROC / Ladder;Layer "+std::to_string(lay)+" - ")+zaxis[itree]+" Occupancy").c_str(),
		       72*divide_roc,-4.5,4.5, (nlad[phase][lay-1]*4+(phase>0)*2)*divide_roc,-nlad[phase][lay-1]-(phase>0)*0.5,nlad[phase][lay-1]+(phase>0)*0.5);
	
	c = custom_can_(h2, (std::string("Layer")+std::to_string(lay)+"/"+varname[itree]+"_vs_ROCLadder_vs_ROCModule").c_str(),0,0,800,800,80,140);
	if      (phase==0)
	  tree->Draw((std::string("module_on.ladder_coord:module_on.module_coord>>")+h2->GetName()).c_str(),(std::string("layer==")+std::to_string(lay)).c_str(),"COLZ");
	else if (phase==1)
	  tree->Draw((std::string("ladder_coord:module_coord>>")+h2->GetName()).c_str(),(std::string("layer==")+std::to_string(lay)).c_str(),"COLZ");
	dress_occup_plot(h2,lay, phase);
	if (save_plot) save_plots(c, save_dir, formats, remove_prelim_label);
      } 
    }

    // FPix
    if (fpix) {
      if (phase==0) {
	h2 = new TH2D( (varname[itree]+"_FPix").c_str(),
		       (std::string(";ROC / Disk;ROC / Blade;")+zaxis[itree]+" Occupancy").c_str(),
		       80*divide_roc,-2.5,2.5, 125*std::max(divide_roc,2),-12.5,12.5);
      } else if (phase==1) {
	h2 = new TH2D( (varname[itree]+"_FPix").c_str(),
		       (std::string(";ROC / Disk;ROC / Blade;")+zaxis[itree]+" Occupancy").c_str(),
		       112*divide_roc,-3.5,3.5, 140*divide_roc,-17.5,17.5);
      }
      c = custom_can_(h2, (std::string("FPix/")+varname[itree]+"_vs_ROCBlade_vs_ROCDisk").c_str(),0,0,800,800,80,140);
      if      (phase==0)
	tree->Draw((std::string("module_on.blade_coord:module_on.disk_ring_coord>>")+h2->GetName()).c_str(),"det==1","COLZ");
      else if (phase==1)
	tree->Draw((std::string("mod_on.blade_panel_coord:mod_on.disk_ring_coord>>")+h2->GetName()).c_str(),"det==1","COLZ");
      dress_occup_plot(h2, 0, phase);
      if (save_plot) save_plots(c, save_dir, formats, remove_prelim_label);
    }
  }

  if (save_plot) gApplication->Terminate();
}
