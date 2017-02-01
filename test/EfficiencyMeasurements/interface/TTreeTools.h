#ifndef TTREE_TOOLS_H
#define TTREE_TOOLS_H

#include "../interface/CommonActors.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>

class TTreeTools
{
	public:
		static int treeCheck(TTree* tree, std::string errorMessage, bool stopExecution = false);
		static void branchCheck(TTree* tree, const std::string& branchName);
		static TBranch* checkGetBranch(TTree* tree, const std::string& branchName);
};

#endif