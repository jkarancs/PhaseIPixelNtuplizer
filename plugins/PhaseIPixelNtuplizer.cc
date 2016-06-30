#include "PhaseIPixelNtuplizer.h"

#define DEBUGNTUPLIZER_ALL

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(const edm::ParameterSet&)
{

}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer()
{
	#ifdef DEBUGNTUPLIZER_ALL
		std::cerr << "Debug: ~PhaseIPixelNtuplizer()" << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::beginJob()
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::beginJob()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::endJob()
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::endJob()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::analyze(const edm::Event&, const edm::EventSetup&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::beginRun()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::endRun()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::beginLuminosityBlock()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << "Executing PhaseIPixelNtuplizer::endLuminosityBlock()..." << std::endl;
	#endif
}

DEFINE_FWK_MODULE(PhaseIPixelNtuplizer);