import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("MinimalisticTest", eras.Run2_2017)

# # Adding geometry
# process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
# from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2017', '')
# process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')

# Message logger service
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
	destinations = cms.untracked.vstring('cerr'),
	cerr = cms.untracked.PSet(threshold  = cms.untracked.string('DEBUG')),
	debugModules = cms.untracked.vstring('PhaseIPixelNtuplizerPlugin'))

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('file:out_step_3.root'),
	secondaryFileNames = cms.untracked.vstring()
)

#---------------------------
#  Track Refitter
#---------------------------
# from RecoTracker.TrackProducer.TrackRefitters_cff import *
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'generalTracks'
process.TrackRefitterP5.TrajectoryInEvent = True

process.PhaseIPixelNtuplizerPlugin = cms.EDAnalyzer('PhaseIPixelNtuplizer')
process.PhaseIPixelNtuplizerPlugin.trajectoryInput = cms.InputTag('TrackRefitter')
# process.PhaseIPixelNtuplizerPlugin.trajectoryInput = cms.InputTag('generalTracks')
process.PhaseIPixelNtuplizer_step = cms.Path(process.PhaseIPixelNtuplizerPlugin)
