import FWCore.ParameterSet.Config as cms

TimingStudy = cms.EDFilter("TimingStudy",
    trajectoryInput = cms.string('TrackRefitter'),
    OutputFile = cms.string('TTreeFile.root') 
)

