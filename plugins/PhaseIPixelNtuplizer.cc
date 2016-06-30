#include "PhaseIPixelNtuplizer.h"

#define DEBUGNTUPLIZER_ALL

void test_prompts_and_colors();

PhaseIPixelNtuplizer::PhaseIPixelNtuplizer(const edm::ParameterSet&)
{
	test_prompts_and_colors();
	#ifdef DEBUGNTUPLIZER_ALL
		std::cerr << debug_prompt << "PhaseIPixelNtuplizer() constructor called." << std::endl;
	#endif
}

PhaseIPixelNtuplizer::~PhaseIPixelNtuplizer()
{
	#ifdef DEBUGNTUPLIZER_ALL
		std::cerr << debug_prompt << "~PhaseIPixelNtuplizer()" << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::beginJob()
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << debug_prompt << "Executing PhaseIPixelNtuplizer::beginJob()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::endJob()
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << debug_prompt << "Executing PhaseIPixelNtuplizer::endJob()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::analyze(const edm::Event&, const edm::EventSetup&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << debug_prompt << "Executing PhaseIPixelNtuplizer::analyze()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << debug_prompt << "Executing PhaseIPixelNtuplizer::beginRun()..." << std::endl;
	#endif
}

void PhaseIPixelNtuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
	#ifdef DEBUGNTUPLIZER_ALL
	std::cerr << debug_prompt << "Executing PhaseIPixelNtuplizer::endRun()..." << std::endl;
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