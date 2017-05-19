# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: -s RAW2DIGI,L1Reco,RECO --data --scenario cosmics --conditions auto:phase1_2017_cosmics --era Run2_2017 --eventcontent RECO --datatier RECO --filein /store/data/Commissioning2017/Cosmics/RAW/v1/000/293/492/00000/FEEE6B6A-9333-E711-94D7-02163E019C99.root --fileout file:ReReco_Cosmics_38T.root --python_filename=Cosmics_92X_cfg.py --runUnscheduled -n 100
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2017)

# import of standard configurations
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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Commissioning2017/Cosmics/RAW/v1/000/293/492/00000/FEEE6B6A-9333-E711-94D7-02163E019C99.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('-s nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:ReReco_Cosmics_38T.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_cosmics', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionCosmics)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)













# begin inserting configs
#------------------------------------------
#  Options - can be given from command line
#------------------------------------------
import FWCore.ParameterSet.VarParsing as opts

opt = opts.VarParsing ('analysis')

opt.register('globalTag',          '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Global Tag, Default="" which uses auto:phase1_2017_cosmics')

opt.register('useTemplates',       True,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Only for On-track clusters! True: use Template reco, False: use Generic reco')

opt.register('saveRECO',           False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Option to keep RECO')

opt.register('useRECO',            False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Option to use RECO')

opt.register('RECOFileName',       '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the histograms file')

opt.register('inputFileName',     '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the input file')

opt.register('outputFileName',     '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the histograms file')

opt.register('useLocalLASim',      False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test LA (SIM) conditions locally (prep/prod database or sqlite file')

opt.register('useLocalQuality',    False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test Quality conditions locally (prep/prod database or sqlite file')

opt.register('useLocalLA',         False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test LA (RECO) conditions locally (prep/prod database or sqlite file')

opt.register('useLocalGain',       False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test Gain conditions locally (prep/prod database or sqlite file')

opt.register('useLocalGenErr',     False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test GenError conditions locally (prep/prod database or sqlite file')

### Events to process: 'maxEvents' is already registered by the framework
opt.setDefault('maxEvents', 100)

# Proceed with settings from command line
opt.parseArguments()

process.maxEvents.input = opt.maxEvents
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Set some default options based on others
if opt.useTemplates:
	process.cosmictrackfinderCosmics.TTRHBuilder = 'WithAngleAndTemplate'
	if opt.RECOFileName == '': opt.RECOFileName = 'file:RECO_TemplateReco_'+str(opt.maxEvents)+'.root'
else:
	process.cosmictrackfinderCosmics.TTRHBuilder = 'WithTrackAngle'
	if opt.RECOFileName == '': opt.RECOFileName = 'file:RECO_GenericReco_'+str(opt.maxEvents)+'.root'

# Add Input file in case using it
if opt.useRECO:
	process.setName_("TEST")
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
		# This is the file you create with saveRECO, by default
		opt.RECOFileName # Use previously saved RECO as input
		))

#________________________________________________________________________
#                        Main Analysis Module

# Specify inputs/outputs
if opt.useTemplates:
	if opt.outputFileName == '': opt.outputFileName = 'Ntuple_TemplateReco_'+str(opt.maxEvents)+'.root'
else:
	if opt.outputFileName == '': opt.outputFileName = 'Ntuple_GenericReco_'+str(opt.maxEvents)+'.root'

if opt.inputFileName != '':
    process.source.fileNames = cms.untracked.vstring(opt.inputFileName)

# HLT Filter
import HLTrigger.HLTfilters.hltHighLevel_cfi

process.HLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    HLTPaths = ['HLT_L1SingleMuCosmics_v*',
                'HLT_L1SingleMuOpen_v*',
                'HLT_L1SingleMuOpen_DT_v*',
                'HLT_L1SingleMu3_v*'],
    throw = False #dont throw except on unknown path names
)
process.hltFilter_step = cms.Path(process.HLTFilter)

# Refitter
from RecoTracker.TrackProducer.TrackRefitters_cff import *
process.Refitter = RecoTracker.TrackProducer.TrackRefitterP5_cfi.TrackRefitterP5.clone()
process.Refitter.src = 'ctfWithMaterialTracksP5'
process.Refitter.NavigationSchool = ""

#---------------------------
#  PhaseIPixelNtuplizer
#---------------------------
process.PhaseINtuplizerPlugin = cms.EDAnalyzer("PhaseIPixelNtuplizer",
	trajectoryInput = cms.InputTag('Refitter'),
	outputFileName = cms.untracked.string(opt.outputFileName),
	cosmics = cms.untracked.bool(True),
 	clusterSaveDownscaleFactor = cms.untracked.int32(1),
	)
process.PhaseIPixelNtuplizer_step = cms.Path(process.PhaseINtuplizerPlugin)

# myAnalyzer Path
process.myAnalyzer_step = cms.Path(process.MeasurementTrackerEvent*process.Refitter*process.PhaseINtuplizerPlugin)

#________________________________________________________________________
#                        DataBase Stuff

# Print settings
print "Using options: "
if opt.globalTag == '':
    print "  globalTag (auto:phase1_2017_cosmics) = "+str(process.GlobalTag.globaltag)
else:
    if "auto:" in opt.globalTag:
	process.GlobalTag = GlobalTag(process.GlobalTag, opt.globalTag, '')
	print "  globalTag ("+opt.globalTag+") = "+str(process.GlobalTag.globaltag)
    else:
	process.GlobalTag.globaltag = opt.globalTag
	print "  globalTag (manually chosen)            = "+str(process.GlobalTag.globaltag)
print "  useTemplates                           = "+str(opt.useTemplates)
print "  saveRECO                               = "+str(opt.saveRECO)
print "  useRECO                                = "+str(opt.useRECO)
print "  RECOFileName                           = "+str(opt.RECOFileName)
print "  inputFileName                          = "+str(opt.inputFileName)
print "  outputFileName                         = "+str(opt.outputFileName)
print "  maxEvents                              = "+str(opt.maxEvents)
print "  useLocalLASim                          = "+str(opt.useLocalLASim)
print "  useLocalQuality                        = "+str(opt.useLocalQuality)
print "  useLocalLA                             = "+str(opt.useLocalLA)
print "  useLocalGain                           = "+str(opt.useLocalGain)
print "  useLocalGenErr                         = "+str(opt.useLocalGenErr)

# Test Local DB conditions
# LA (SIM)
LASim_tag       = "SiPixelLorentzAngleSim_phase1_mc_v1"
#LASim_db        = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngleSim_phase1_mc_v1.db'
LASim_db        = 'frontier://FrontierPrep/CMS_CONDITIONS'
#LASim_db        = 'frontier://FrontierProd/CMS_CONDITIONS'

# Quality
Qua_tag         = 'SiPixelQuality_phase1_ideal'
#Qua_db          = 'sqlite_file:../../../../../DB/phase1/SiPixelQuality_phase1_ideal.db'
Qua_db          = 'frontier://FrontierPrep/CMS_CONDITIONS'
#Qua_db          = 'frontier://FrontierProd/CMS_CONDITIONS'

# LA (RECO)
LA_tag          = 'SiPixelLorentzAngle_phase1_mc_v1'
LA_Width_tag    = 'SiPixelLorentzAngle_phase1_mc_v1'
#LA_db           = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db'
#LA_db           = 'frontier://FrontierPrep/CMS_CONDITIONS'
LA_db           = 'frontier://FrontierProd/CMS_CONDITIONS'
#LA_Width_db     = 'sqlite_file:../../../../../DB/phase1/SiPixelLorentzAngle_phase1_mc_v1.db'
#LA_Width_db     = 'frontier://FrontierPrep/CMS_CONDITIONS'
LA_Width_db     = 'frontier://FrontierProd/CMS_CONDITIONS'

# Gains
#Gain_tag        = 'SiPixelGainCalibration_phase1_mc_v2'
Gain_tag        = 'SiPixelGainCalibration_2017_v1_offline'
#Gain_tag        = 'SiPixelGainCalibration_phase1_ideal'
#Gain_tag        = 'SiPixelGainCalibration_phase1_ideal_v2'
Gain_db         = 'sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase1/2017_05_17/SiPixelGainCalibration_2017_v1_offline.db'
#Gain_db         = 'sqlite_file:../../../../../DB/phase1/SiPixelGainCalibration_phase1_ideal_v2.db'
#Gain_db         = 'frontier://FrontierPrep/CMS_CONDITIONS'
#Gain_db         = 'frontier://FrontierProd/CMS_CONDITIONS'

# GenErrors
GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_mc_v1'
#GenErr_db       = 'sqlite_file:siPixelGenErrors38T.db'
#GenErr_db       = 'frontier://FrontierPrep/CMS_COND_PIXEL'
#GenErr_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
GenErr_db       = 'frontier://FrontierProd/CMS_CONDITIONS'

#LA
if opt.useLocalLASim :
	process.LASimReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleSimRcd"),
			tag = cms.string(LASim_tag))),
		connect = cms.string(LASim_db))
	process.lasimprefer = cms.ESPrefer("PoolDBESSource","LASimReader")

# Quality
if opt.useLocalQuality :
	process.QualityReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelQualityFromDbRcd'),
			tag = cms.string('SiPixelQuality_phase1_ideal'))),
		connect = cms.string(Qua_tag))
	process.es_prefer_QualityReader = cms.ESPrefer("PoolDBESSource","QualityReader")

# for reco
# LA 
if opt.useLocalLA :
	process.LAReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleRcd"),
			tag = cms.string(LA_tag))),
		connect = cms.string(LA_db))
	process.LAprefer = cms.ESPrefer("PoolDBESSource","LAReader")
	# now the forWidth LA
	process.LAWidthReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string("SiPixelLorentzAngleRcd"),
			label = cms.untracked.string("forWidth"),
			tag = cms.string(LA_Width_tag))),
		connect = cms.string(LA_Width_db))
	process.LAWidthprefer = cms.ESPrefer("PoolDBESSource","LAWidthReader")

# Gain 
if opt.useLocalGain :
	process.GainsReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelGainCalibrationOfflineRcd'),
			tag = cms.string(Gain_tag))),
		connect = cms.string(Gain_db))
	process.Gainprefer = cms.ESPrefer("PoolDBESSource","GainsReader")

# GenError
if opt.useLocalGenErr :
	process.GenErrReader = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			messageLevel = cms.untracked.int32(0),
			authenticationPath = cms.untracked.string('')),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelGenErrorDBObjectRcd'),
			tag = cms.string(GenErr_tag))),
		connect = cms.string(GenErr_db))
	process.generrprefer = cms.ESPrefer("PoolDBESSource","GenErrReader")



# Temporary cabling map fix
process.CablingMapDBReader = cms.ESSource("PoolDBESSource",
  BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
  DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0),
    authenticationPath = cms.untracked.string('')
  ),
  connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS'), 
  toGet = cms.VPSet(
    cms.PSet(
      record = cms.string('SiPixelFedCablingMapRcd'),
      tag = cms.string('SiPixelFedCablingMap_phase1_v7_May2'),  
    )
  )
)
process.es_prefer_CablingReader = cms.ESPrefer("PoolDBESSource","CablingMapDBReader")
# New alignment
#process.NewAlignment = cms.ESSource("PoolDBESSource",
#        BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
#        DBParameters = cms.PSet(
#            messageLevel = cms.untracked.int32(0),
#            authenticationPath = cms.untracked.string('')
#        ),
#        connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
#        toGet = cms.VPSet(
#           cms.PSet(
#           record = cms.string('TrackerAlignmentRcd'),
#           tag = cms.string('TrackerAlignment_CRUZET17_v1'),
#           )
#        )
#)
#process.es_prefer_Alignment = cms.ESPrefer("PoolDBESSource","NewAlignment")


#---------------------------
#  Schedule
#---------------------------

# Modify Schedule
process.schedule = cms.Schedule(process.hltFilter_step,process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.myAnalyzer_step)

# End of inserted code




















#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
