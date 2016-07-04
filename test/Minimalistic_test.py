import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations = cms.untracked.vstring(
        'cerr',
         ),
    cerr = cms.untracked.PSet(
        threshold  = cms.untracked.string('DEBUG') 
         ),
    debugModules = cms.untracked.vstring('*')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('/store/mc/RunIISummer15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v2/10000/004CC894-4877-E511-A11E-0025905C3DF8.root'),
	inputCommands = cms.untracked.vstring('keep *', 
		'drop LHEXMLStringProduct_*_*_*', 
		'keep *', 
		'drop *_genParticles_*_*', 
		'drop *_genParticlesForJets_*_*', 
		'drop *_kt4GenJets_*_*', 
		'drop *_kt6GenJets_*_*', 
		'drop *_iterativeCone5GenJets_*_*', 
		'drop *_ak4GenJets_*_*', 
		'drop *_ak7GenJets_*_*', 
		'drop *_ak8GenJets_*_*', 
		'drop *_ak4GenJetsNoNu_*_*', 
		'drop *_ak8GenJetsNoNu_*_*', 
		'drop *_genCandidatesForMET_*_*', 
		'drop *_genParticlesForMETAllVisible_*_*', 
		'drop *_genMetCalo_*_*', 
		'drop *_genMetCaloAndNonPrompt_*_*', 
		'drop *_genMetTrue_*_*', 
		'drop *_genMetIC5GenJs_*_*'),
	secondaryFileNames = cms.untracked.vstring()
)
process.demo = cms.EDAnalyzer('PhaseIPixelNtuplizer')
process.p = cms.Path(process.demo)
