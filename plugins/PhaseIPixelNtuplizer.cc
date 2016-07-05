#include "PhaseIPixelNtuplizer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG


PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(edm::ParameterSet const& iConfig)
{
	LogDebug("step") << "PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogError("category")    << "error    PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogProblem("category")  << "problem  PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogWarning ("category") << "warning  PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogPrint("category")    << "print    PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogInfo("category")     << "infor    PhaseIPixelNtuplizer() constructor called." << std::endl;
	// edm::LogVerbatim("category") << "verbatim PhaseIPixelNtuplizer() constructor called." << std::endl;
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer()
{
	LogDebug("step") << "~PhaseIPixelNtuplizer()" << std::endl;

}

void PhaseIPixelNtuplizer::beginJob()
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginJob()..." << std::endl;
	this -> iConfig = iConfig;

	if(iConfig.exists("fileName"))
	{
		ntuple_output_filename = iConfig.getParameter<std::string>("filename");
	}

	////////////////////////
	// Create output file //
	////////////////////////

	ntuple_output_file = new TFile(ntuple_output_filename.c_str(), "RECREATE");
	
	if(!(ntuple_output_file -> IsOpen()))
	{
		edm::LogError("file_operations") << "Failed to open output file: " << ntuple_output_filename << std::endl; 
		exit(-1);
	}

	LogDebug("file_operations") << "Output file: \"" << ntuple_output_filename << "\" created." << std::endl;


}

void PhaseIPixelNtuplizer::endJob()
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endJob()..." << std::endl;

	LogDebug("file_operations") << "Writing plots to file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Write();
	LogDebug("file_operations") << "Closing file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << ntuple_output_filename << "\"." << std::endl;
}

void PhaseIPixelNtuplizer::analyze(const edm::Event&, const edm::EventSetup&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginRun()..." << std::endl;
}

void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endRun()..." << std::endl;
}

void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::beginLuminosityBlock()..." << std::endl;
}

void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing PhaseIPixelNtuplizer::endLuminosityBlock()..." << std::endl;
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);