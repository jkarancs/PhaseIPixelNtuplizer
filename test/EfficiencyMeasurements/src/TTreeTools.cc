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
int TTreeTools::treeCheck(TChain* tree, std::string errorMessage, bool stopExecution)
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
void TTreeTools::branchCheck(TChain* tree, const std::string& branchName)
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
// Does not seem to work, use SetBranchaddress instead!
TBranch* TTreeTools::checkGetBranch(TChain* chain, const std::string& branchName)
{
	branchCheck(chain, branchName);
	TBranch* branch = chain -> GetBranch(branchName.c_str());
	if(chain -> GetBranchStatus(branchName.c_str()) == 0)
	{
		std::cout << "Chain has a branch (" << branchName << ") with a status of 0. Setting it to 1..." << std::endl;
		chain -> SetBranchStatus(branchName.c_str(), 1);
	}
	return branch;
}
