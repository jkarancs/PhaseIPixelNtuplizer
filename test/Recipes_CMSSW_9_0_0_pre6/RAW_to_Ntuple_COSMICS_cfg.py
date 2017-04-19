

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PhaseINtuplizer', eras.Run2_2017)

# import of standard configurations
# process.load('Configuration.StandardSequences.Services_cff')
# process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
# process.load('FWCore.MessageService.MessageLogger_cfi')
# # process.load('Configuration.EventContent.EventContent_cff')
# process.load('Configuration.EventContent.EventContentCosmics_cff')
# # process.load('SimGeneral.MixingModule.mixNoPU_cfi')
# # process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
# process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
# # process.load('Configuration.StandardSequences.MagneticField_cff')
# process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
# process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
# # process.load('Configuration.StandardSequences.Reconstruction_cff')
# process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
# process.load('Configuration.StandardSequences.EndOfProcess_cff')
# process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
inputFilesFromRaw = cms.untracked.vstring('/store/express/Commissioning2017/ExpressCosmics/FEVT/Express-v1/000/291/299/00000/94953192-AE1B-E711-A88B-02163E013506.root')

# Input source
process.source = cms.Source("PoolSource",
    fileNames = inputFilesFromRaw,
    secondaryFileNames = cms.untracked.vstring())

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('PhaseINtuplizer run on reco'),
    name = cms.untracked.string('PhaseINtuplizer'),
    version = cms.untracked.string('$alpha$'))

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_cosmics', '')

# Track Refitter
# from RecoTracker.TrackProducer.TrackRefitters_cff import *
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = "generalTracks"

process.PhaseIPixelNtuplizerPlugin = cms.EDAnalyzer('PhaseIPixelNtuplizer',
    trajectoryInput = cms.InputTag('TrackRefitter'))
# process.PhaseIPixelNtuplizerPlugin.trajectoryInput = cms.InputTag('generalTracks')
	
# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionCosmics)
process.PhaseIPixelNtuplizer_step = cms.Path(process.MeasurementTrackerEvent * process.TrackRefitter * process.PhaseIPixelNtuplizerPlugin)

process.schedule = cms.Schedule(
	process.raw2digi_step,
	process.reconstruction_step,
	process.PhaseIPixelNtuplizer_step)
