#include "../interface/TTreeTools.h"

int TTreeTools::treeCheck(TTree* tree, std::string errorMessage, bool stopExecution)
{
	if(tree == nullptr)
	{
		std::cerr << error_prompt << errorMessage << std::endl;
		if(stopExecution) exit(-1);
		return(-1);
	}
	return 0;
}

void TTreeTools::branchCheck(TTree* tree, const std::string& branchName)
{
	TBranch* branch = static_cast<TBranch*>(tree -> GetListOfBranches() -> FindObject(branchName.c_str()));
	if(!branch)
	{
		std::cerr << error_prompt << "Missing branch: " << branchName << std::endl;	
		tree -> SetBranchStatus(branchName.c_str(), 0);
	}
	tree -> SetBranchStatus(branchName.c_str(), 1);
}

TBranch* TTreeTools::checkGetBranch(TTree* tree, const std::string& branchName)
{
	branchCheck(tree, branchName);
	TBranch* branch = tree -> GetBranch(branchName.c_str());
	return branch;
}
