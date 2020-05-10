import os
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

process = cms.Process('NTUPLE',Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/t/tvami/public/PixelALCARECOs/forPR/CMSSW_11_0_0_pre9/src/SiPixelCalSingleMuon_SingleThread_4kEvent.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/t/tvami/public/HSCP/HSCP/CMSSW_10_6_1_patch1/src/0RECOChain/HSCPSingalDY/3HSCP_DY_Mass800_RAWtoRECO_TrackingOnly_SignalOnly.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/t/tvami/public/PixelALCARECOs/forPR/CMSSW_11_1_X_2019-12-09-2300/src/SiPixelCalSingleMuon_100evt.root'),
    fileNames = cms.untracked.vstring('file:Run320500.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/t/tvami/public/PixelALCARECOs/forPR/CMSSW_11_0_0_pre9/src/SiPixelCalSingleMuon_noInclusiveClust.root'),
    #fileNames = cms.untracked.vstring('file:/eos/user/t/tvami/SingleMuon_2018_data_RECO.root'),
    secondaryFileNames = cms.untracked.vstring(),
    bypassVersionCheck = cms.untracked.bool(True)

)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound'), wantSummary = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '110X_dataRun2_v12', '')

# Efficiency

process.PhaseINtuplizerPlugin = cms.EDAnalyzer("PhaseIPixelNtuplizer",
    trajectoryInput = cms.InputTag('TrackRefitter'),
    outputFileName = cms.untracked.string('Efficiency_ALCARECO.root'),
    # Global muon collection
    muonCollection                 = cms.InputTag("muons"),
    keepAllGlobalMuons             = cms.untracked.bool(False),
    keepAllTrackerMuons            = cms.untracked.bool(False),
    # Save everything
    ##  clusterSaveDownscaleFactor     = cms.untracked.int32(1),
    ##  trackSaveDownscaleFactor       = cms.untracked.int32(1),
    ##  saveDigiTree                   = cms.untracked.bool(True),
    ##  saveTrackTree                  = cms.untracked.bool(True),
    ##  saveNonPropagatedExtraTrajTree = cms.untracked.bool(True),
    # Do not save everything and downscale clusters
    clusterSaveDownscaleFactor     = cms.untracked.int32(10), # don't save clusters
    trackSaveDownscaleFactor       = cms.untracked.int32(10), # Save only muon tracks
    saveDigiTree                   = cms.untracked.bool(False),
    saveTrackTree                  = cms.untracked.bool(True),
    saveNonPropagatedExtraTrajTree = cms.untracked.bool(False),
    # ALCARECO collections
    #clusterCollection              = cms.InputTag("siPixelClusters"),
    clusterCollection              = cms.InputTag("ALCARECOSiPixelCalSingleMuon"),
)

# Lorentz Angle
orbit_begin_array = [0]
orbit_end_array = [1434229500]
index = 0
process.SiPixelLorentzAngle = cms.EDAnalyzer("SiPixelLorentzAngle",
        src = cms.InputTag('TrackRefitter'),
        fileName = cms.string('LA_ALCARECO.root'),
        fileNameFit     = cms.string(""),
        binsDepth       = cms.int32(50),
        binsDrift =     cms.int32(200),
        ptMin = cms.double(3),
        ptMinFPix = cms.double(0.1),
        #in case of MC set this to true to save the simhits (does not work currently, Mixing Module needs to be included correctly)
        simData = cms.bool(False),
        normChi2Max = cms.double(2),
        clustSizeYMin = cms.int32(2),
        residualMax = cms.double(0.01),
        clustChargeMax = cms.double(120000)
)

# BPix Resolution
process.BPixResolution_Template = cms.EDAnalyzer('Pixel',
        triggerSource = cms.InputTag('TriggerResults::HLT'),
        ttrhBuilder = cms.string('WithAngleAndTemplate'),
        orbit_beginning = cms.int32(orbit_begin_array[index]),
        orbit_end = cms.int32(orbit_end_array[index]),
        )

process.BPixResolution_Generic = process.BPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle'),
    orbit_beginning = cms.int32(orbit_begin_array[index]),
    orbit_end = cms.int32(orbit_end_array[index]),
)

# FPix Resolution
process.FPixResolution_Template = cms.EDAnalyzer('Pixel_phase1',
    triggerSource = cms.InputTag('TriggerResults::HLT'),
    ttrhBuilder = cms.string('WithAngleAndTemplate'),
    doBPix = cms.bool(False),
    doFPix = cms.bool(True),
    orbit_beginning = cms.int32(orbit_begin_array[index]),
    orbit_end = cms.int32(orbit_end_array[index]),
)
process.FPixResolution_Generic = process.FPixResolution_Template.clone(
    ttrhBuilder = cms.string('WithTrackAngle'),
    orbit_beginning = cms.int32(orbit_begin_array[index]),
    orbit_end = cms.int32(orbit_end_array[index]),
)

# TFileService used for both BPix/FPix resolution
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('Resolution_ALCARECO.root'),
)

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                      = cms.untracked.int32(4),
    rootFileName                 = cms.untracked.string("pixelTreeBasedALCARECO.root"),
    associateRecoTracks = cms.bool(False),
    associateStrip = cms.bool(False),
    associatePixel = cms.bool(True),
    #RecHitProducer = cms.string('siStripMatchedRecHits'),
    pixelSimLinkSrc = cms.InputTag("simSiPixelDigis"),
    stripSimLinkSrc = cms.InputTag("simSiStripDigis"),
    ROUList = cms.vstring(
        'TrackerHitsPixelBarrelLowTof', 
        'TrackerHitsPixelBarrelHighTof', 
        'TrackerHitsPixelEndcapLowTof', 
        'TrackerHitsPixelEndcapHighTof'),
    #type                         = cms.untracked.string(getDataset(process.source.fileNames[0])),
    globalTag                    = process.GlobalTag.globaltag,
    dumpAllEvents                = cms.untracked.int32(0),
    PrimaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    muonCollectionLabel          = cms.untracked.InputTag('muons'),
    trajectoryInputLabel         = cms.untracked.InputTag('TrackRefitter::NTUPLE'),
    trackCollectionLabel         = cms.untracked.InputTag('ALCARECOSiPixelCalSingleMuon'),
    pixelClusterLabel            = cms.untracked.InputTag('siPixelClusters'),
    pixelRecHitLabel             = cms.untracked.InputTag('siPixelRecHits'),
    HLTProcessName               = cms.untracked.string('HLT'),
    L1GTReadoutRecordLabel       = cms.untracked.InputTag('gtDigis'),
    hltL1GtObjectMap             = cms.untracked.InputTag('hltL1GtObjectMap'),
    accessSimHitInfo             = cms.untracked.bool(False),
)

# Path and EndPath definitions
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi") 

process.MeasurementTrackerEvent.pixelClusterProducer = 'ALCARECOSiPixelCalSingleMuon'
process.MeasurementTrackerEvent.stripClusterProducer = 'ALCARECOSiPixelCalSingleMuon'
process.MeasurementTrackerEvent.inactivePixelDetectorLabels = cms.VInputTag()
process.MeasurementTrackerEvent.inactiveStripDetectorLabels = cms.VInputTag()

process.TrackRefitter.src = 'ALCARECOSiPixelCalSingleMuon'
process.TrackRefitter.TrajectoryInEvent = True

process.TrackRefitter_step = cms.Path(
    process.offlineBeamSpot*
    process.MeasurementTrackerEvent*
    process.TrackRefitter#*
  #process.siPixelRecHits
)
process.Efficiency_step = cms.Path(process.PhaseINtuplizerPlugin)
process.LATrees_step        = cms.Path(process.SiPixelLorentzAngle)
process.BPixResolution_step = cms.Path(process.BPixResolution_Template*process.BPixResolution_Generic)
process.FPixResolution_step = cms.Path(process.FPixResolution_Template*process.FPixResolution_Generic)
process.PixelTree_step = cms.Path(process.PixelTree)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(
    process.TrackRefitter_step,
    #process.LATrees_step,
    process.Efficiency_step,
    process.endjob_step
)

##Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(8)
#process.options.numberOfStreams=cms.untracked.uint32(0)
#process.options.numberOfConcurrentLuminosityBlocks=cms.untracked.uint32(1)
