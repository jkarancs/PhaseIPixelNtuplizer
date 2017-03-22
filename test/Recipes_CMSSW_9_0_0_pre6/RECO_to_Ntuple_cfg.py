

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PhaseINtuplizer', eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
inputFilesFromReco = cms.untracked.vstring('file:PhaseI_TTbar_13TeV_NoPu_RECO.root')

# Input source
process.source = cms.Source("PoolSource",
    fileNames = inputFilesFromReco,
    secondaryFileNames = cms.untracked.vstring())

process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('PhaseINtuplizer run on reco'),
    name = cms.untracked.string('PhaseINtuplizer'),
    version = cms.untracked.string('$alpha$'))

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

#---------------------------
#  Track Refitter
#---------------------------
# from RecoTracker.TrackProducer.TrackRefitters_cff import *
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = "generalTracks"

process.PhaseIPixelNtuplizerPlugin = cms.EDAnalyzer('PhaseIPixelNtuplizer',
    trajectoryInput = cms.InputTag('TrackRefitter'))
# process.PhaseIPixelNtuplizerPlugin.trajectoryInput = cms.InputTag('generalTracks')
process.PhaseIPixelNtuplizer_step = cms.Path(process.MeasurementTrackerEvent * process.TrackRefitter * process.PhaseIPixelNtuplizerPlugin)

# do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)
