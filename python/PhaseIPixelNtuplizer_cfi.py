import FWCore.ParameterSet.Config as cms

PhaseIPixelNtuplizer = cms.EDFilter("PhaseIPixelNtuplizer",
    trajectoryInput = cms.string('TrackRefitter'),
    OutputFile = cms.string('TTreeFile.root') 
)

