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
	fileNames = cms.untracked.vstring('file:/data/hunyadi/CMSSW/PhaseI_SIM/Tracker_material/CMSSW_8_1_0_pre1/src/Test/Steps_test/out_step_3.root'),
	secondaryFileNames = cms.untracked.vstring()
)
process.demo = cms.EDAnalyzer('PhaseIPixelNtuplizer')
process.p = cms.Path(process.demo)
