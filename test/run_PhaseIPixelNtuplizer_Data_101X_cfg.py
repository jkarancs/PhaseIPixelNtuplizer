# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: RECO -s RAW2DIGI,L1Reco,RECO --data --scenario pp --conditions auto:run2_data --era Run2_2018 --process NTUPLE --eventcontent RECO --datatier RECO --filein dummy.root --secondfilein dummy.root --python_filename=test/run_PhaseIPixelNtuplizer_Data_101X_cfg.py --runUnscheduled -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NTUPLE',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('dummy.root'),
    secondaryFileNames = cms.untracked.vstring('dummy.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('RECO_RAW2DIGI_L1Reco_RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
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
	     'Global Tag, Default="" which uses auto:run2_data')

opt.register('dataTier',           'RECO',
             opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
             'Input file data tier')

opt.register('useTemplates',       True,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Only for On-track clusters! True: use Template reco, False: use Generic reco')

opt.register('saveRECO',           False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Option to keep GEN-SIM-RECO')

opt.register('RECOFileName',  '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Name of the RECO output file in case saveRECO was used')

opt.register('inputFileName',   '',
             opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
             'Name of the input root file')

opt.register('secondaryInputFileName',   '',
             opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
             'Name of the RAW (parent) input root file')

opt.register('outputFileName',   'Ntuple.root',
             opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
             'Name of the histograms file')

opt.register('noMagField',         False,
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

opt.register('useLocalTemplates',  False,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
	     'Test Template conditions locally (prep/prod database or sqlite file')

opt.register('prescale',           1,
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.int,
	     'Save only 1/nth of the events (to conserve disk space for long runs)')

opt.register('loadTagsFromPrep',   '',
             opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
             'Load and use condition(s) from Prep automatically (can specify more if separated by commas, useful for quick validation)')

### Events to process: 'maxEvents' is already registered by the framework
opt.setDefault('maxEvents', 100)

# Proceed with settings from command line
opt.parseArguments()

process.maxEvents.input = opt.maxEvents
process.MessageLogger.cerr.FwkReport.reportEvery = 10

# Switch off magnetic field if needed
if opt.noMagField:
	process.load('Configuration.StandardSequences.MagneticField_0T_cff')

# Set some default options based on others
if opt.RECOFileName == '': opt.RECOFileName = 'file:RECO_'+str(opt.maxEvents)+'.root'
# Input file
if opt.inputFileName == '':
    if opt.dataTier == 'RAW':
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(
                                        'file:/data/store/data/Run2016B/ZeroBias/RAW/v2/000/273/158/00000/C62669DA-7418-E611-A8FB-02163E01377A.root' #273158 RAW
                                        )
                                    )
    elif opt.dataTier == 'AOD':
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/AOD/PromptReco-v2/000/276/317/00000/12A3F60B-1145-E611-83B1-02163E01431C.root'),
                                    secondaryFileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/RAW/v2/000/276/317/00000/46CDE349-0842-E611-A1F4-02163E012067.root')
                                    )
    else:
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(
                                        '/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/294/928/00000/6ADE5F77-D03F-E711-BFED-02163E01A6C2.root' #first run
                                        )
                                    )
else:
    if opt.secondaryInputFileName == '':
        process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(opt.inputFileName))
    else:
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(opt.inputFileName),
                                    secondaryFileNames = cms.untracked.vstring(opt.secondaryInputFileName)
                                    )

#________________________________________________________________________
#                        Main Analysis Module

# Refitter
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# Specify inputs/outputs
if opt.useTemplates:
	process.TrackRefitter.TTRHBuilder = 'WithAngleAndTemplate'
	if opt.outputFileName == '': opt.outputFileName = 'Ntuple_'+str(opt.maxEvents)+'.root'
else:
	process.TrackRefitter.TTRHBuilder = 'WithTrackAngle'
	if opt.outputFileName == '': opt.outputFileName = 'Ntuple_GenericReco_'+str(opt.maxEvents)+'.root'

# Load and confiugre the plugin you want to use
#---------------------------
#  PhaseIPixelNtuplizer
#---------------------------
process.PhaseINtuplizerPlugin = cms.EDAnalyzer("PhaseIPixelNtuplizer",
	trajectoryInput = cms.InputTag('TrackRefitter'),
	outputFileName = cms.untracked.string(opt.outputFileName),
	# Do not save everything and downscale clusters
 	clusterSaveDownscaleFactor     = cms.untracked.int32(100),
 	eventSaveDownscaleFactor       = cms.untracked.int32(opt.prescale),
	saveDigiTree                   = cms.untracked.bool(False),
	saveTrackTree                  = cms.untracked.bool(False),
	saveNonPropagatedExtraTrajTree = cms.untracked.bool(False),
	)
process.PhaseIPixelNtuplizer_step = cms.Path(process.PhaseINtuplizerPlugin)

# myAnalyzer Path
process.myAnalyzer_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter*process.PhaseINtuplizerPlugin)

#________________________________________________________________________
#                        DataBase Stuff

# Print settings
print "Using options: "
if opt.globalTag == '':
    print "  globalTag (auto:run2_data) = "+str(process.GlobalTag.globaltag)
else:
    if "auto:" in opt.globalTag:
	process.GlobalTag = GlobalTag(process.GlobalTag, opt.globalTag, '')
	print "  globalTag ("+opt.globalTag+") = "+str(process.GlobalTag.globaltag)
    else:
	process.GlobalTag.globaltag = opt.globalTag
	print "  globalTag (manually chosen)            = "+str(process.GlobalTag.globaltag)
print "  dataTier                               = "+str(opt.dataTier)
print "  useTemplates                           = "+str(opt.useTemplates)
print "  saveRECO                               = "+str(opt.saveRECO)
print "  RECOFileName                           = "+str(opt.RECOFileName)
print "  inputFileName                          = "+str(opt.inputFileName)
print "  secondaryInputFileName                 = "+str(opt.secondaryInputFileName)
print "  outputFileName                         = "+str(opt.outputFileName)
print "  noMagField                             = "+str(opt.noMagField)
print "  maxEvents                              = "+str(opt.maxEvents)
print "  useLocalQuality                        = "+str(opt.useLocalQuality)
print "  useLocalLA                             = "+str(opt.useLocalLA)
print "  useLocalGain                           = "+str(opt.useLocalGain)
print "  useLocalGenErr                         = "+str(opt.useLocalGenErr)
print "  useLocalTemplates                      = "+str(opt.useLocalTemplates)
print "  prescale                               = "+str(opt.prescale)

if opt.loadTagsFromPrep != '':
	Rcds = {
		"SiPixelQuality":		   "SiPixelQualityFromDbRcd",
		"SiPixelLorentzAngle":		   "SiPixelLorentzAngleRcd",
		"SiPixelGainCalibration":          "SiPixelGainCalibrationOfflineRcd",
		"SiPixelGenErrorDBObject":	   "SiPixelGenErrorDBObjectRcd",
		"SiPixelTemplateDBObject":	   "SiPixelTemplateDBObjectRcd"
		}
	for cond in str(opt.loadTagsFromPrep).split(','):
		print "Loading condition: "+cond
		if opt.noMagField and "Template" in cond:
			process.GlobalTag.toGet.append(cms.PSet(
				connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS'),
				record = cms.string(Rcds[cond.split("_")[0]]),
				tag = cms.string(cond),
				label = cms.untracked.string('0T')))
		else:
			process.GlobalTag.toGet.append(cms.PSet(
				connect = cms.string('frontier://FrontierPrep/CMS_CONDITIONS'),
				record = cms.string(Rcds[cond.split("_")[0]]),
				tag = cms.string(cond)))
else:
	dir   = 'sqlite_file:/afs/cern.ch/user/j/jkarancs/public/DB/Phase1/'
	Danek = 'sqlite_file:/afs/cern.ch/user/d/dkotlins/public/CMSSW/DB/phase1/'
	
        # Test Local DB conditions
        # Quality
        #Qua_db          = 'frontier://FrontierProd/CMS_CONDITIONS'
        Qua_db          = 'frontier://FrontierPrep/CMS_CONDITIONS'
        #Qua_tag         = 'SiPixelQuality_phase1_2017_v1_hltvalidation'
        Qua_tag         = 'SiPixelQuality_phase1_2017_v2' # 2017 May 18 version from Tamas
        
        # Gains
        #Gain_db         = 'frontier://FrontierProd/CMS_CONDITIONS'
        Gain_db         = 'frontier://FrontierPrep/CMS_CONDITIONS'
        #Gain_db         = dir + '2017_05_17/SiPixelGainCalibration_2017_v1_offline.db'
        #Gain_tag        = 'SiPixelGainCalibration_2017_v1_hltvalidation'
        Gain_tag        = 'SiPixelGainCalibration_2017_v4'
        
        # LA (RECO)
        #LA_db           = 'frontier://FrontierProd/CMS_CONDITIONS'
        LA_db           = 'frontier://FrontierPrep/CMS_CONDITIONS'
        # MC
        #LA_db           = dir+'2017_02_13/SiPixelLorentzAngle_phase1_mc_v2.db'
        #LA_tag          = 'SiPixelLorentzAngle_phase1_mc_v2'
        # Data
        #LA_db           = dir+'2017_04_05/SiPixelLorentzAngle_phase1_2017_v1.db'
        LA_tag          = 'SiPixelLorentzAngle_phase1_2017_v4'
        
        # LA (Width)
        #LA_Width_db     = 'frontier://FrontierProd/CMS_CONDITIONS'
        #LA_Width_db     = 'frontier://FrontierPrep/CMS_CONDITIONS'
        LA_Width_db     = dir+'2017_02_13/SiPixelLorentzAngle_forWidth_phase1_mc_v2.db'
        LA_Width_tag    = 'SiPixelLorentzAngle_forWidth_phase1_mc_v2'
        
        # GenErrors
        if opt.noMagField:
        	# 0T GenErrors
        	#GenErr_db       = 'frontier://FrontierProd/CMS_CONDITIONS'
        	#GenErr_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
        	# MC
        	GenErr_db       = dir+'2017_04_05/SiPixelGenErrorDBObject_phase1_00T_mc_v2.db'
        	GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_00T_mc_v2'
        	# Data
        	#GenErr_db       = dir+'2017_03_20/SiPixelGenErrorDBObject_phase1_00T_2017_v1.db'
        	#GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_00T_2017_v1'
        	
        	# 0T Templates
        	#Templates_db       = 'frontier://FrontierProd/CMS_CONDITIONS'
        	#Templates_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
        	# MC
        	Templates_db       = dir+'2017_04_05/SiPixelTemplateDBObject_phase1_00T_mc_v2.db'
        	Templates_tag      = 'SiPixelTemplateDBObject_phase1_00T_mc_v2'
        	# Data
        	#Templates_db       = dir+'2017_03_20/SiPixelTemplateDBObject_phase1_00T_2017_v1.db'
        	#Templates_tag      = 'SiPixelTemplateDBObject_phase1_00T_2017_v1'
        else:
        	# 3.8T GenErrors
        	#GenErr_db       = 'frontier://FrontierProd/CMS_CONDITIONS'
        	GenErr_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
        	# MC
        	#GenErr_db       = dir+'2017_02_13/SiPixelGenErrorDBObject_phase1_38T_mc_v2.db'
        	#GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_mc_v2'
        	# Data
        	#GenErr_db       = dir+'2017_04_05/SiPixelGenErrorDBObject_phase1_38T_2017_v1.db'
        	#GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_2017_v1'
        	#GenErr_db       = dir+'2017_07_06/SiPixelGenErrorDBObject_phase1_38T_2017_v4_bugfix.db'
        	#GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_2017_v4_bugfix'
        	GenErr_tag      = 'SiPixelGenErrorDBObject_phase1_38T_2017_v8'
        	
        	# 3.8T Templates
        	#Templates_db       = 'frontier://FrontierProd/CMS_CONDITIONS'
        	Templates_db       = 'frontier://FrontierPrep/CMS_CONDITIONS'
        	# MC
        	#Templates_db       = dir+'2017_02_13/SiPixelTemplateDBObject_phase1_38T_mc_v2.db'
        	#Templates_tag      = 'SiPixelTemplateDBObject_phase1_38T_mc_v2'
        	# Data
        	#Templates_db       = dir+'2017_04_05/SiPixelTemplateDBObject_phase1_38T_2017_v1.db'
        	#Templates_tag      = 'SiPixelTemplateDBObject_phase1_38T_2017_v1'
        	#Templates_db       = dir+'2017_07_06/SiPixelTemplateDBObject_phase1_38T_2017_v4_bugfix.db'
        	#Templates_tag      = 'SiPixelTemplateDBObject_phase1_38T_2017_v4_bugfix'
        	Templates_tag      = 'SiPixelTemplateDBObject_phase1_38T_2017_v8'
        
        # Quality
        if opt.useLocalQuality :
        	process.QualityReader = cms.ESSource("PoolDBESSource",
        		DBParameters = cms.PSet(
        			messageLevel = cms.untracked.int32(0),
        			authenticationPath = cms.untracked.string('')),
        		toGet = cms.VPSet(cms.PSet(
        			record = cms.string('SiPixelQualityFromDbRcd'),
        			tag = cms.string(Qua_tag))),
        		connect = cms.string(Qua_db))
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
        
        # Templates
        if opt.useLocalTemplates :
        	process.TemplatesReader = cms.ESSource("PoolDBESSource",
        		DBParameters = cms.PSet(
        			messageLevel = cms.untracked.int32(0),
        			authenticationPath = cms.untracked.string('')),
        		toGet = cms.VPSet(cms.PSet(
        			record = cms.string('SiPixelTemplateDBObjectRcd'),
        			tag = cms.string(Templates_tag))),
        		connect = cms.string(Templates_db))
        	if opt.noMagField:
        		process.TemplatesReader.toGet = cms.VPSet(
        			cms.PSet(
        				label = cms.untracked.string('0T'),
        				record = cms.string('SiPixelTemplateDBObjectRcd'),
        				tag = cms.string(Templates_tag)
        				)
        			)
        	process.templateprefer = cms.ESPrefer("PoolDBESSource","TemplatesReader")

#---------------------------
#  Schedule
#---------------------------

# Modify Schedule
if opt.dataTier == 'RECO' or opt.dataTier == 'FEVT':
    process.schedule = cms.Schedule(process.myAnalyzer_step)
else:
    if not opt.saveRECO:
        process.schedule.remove(process.RECOoutput_step)
    else:
        process.RECOoutput.fileName = opt.RECOFileName
    # Remove unnecessary steps and add Analyzer in the end of the chain
    process.schedule.remove(process.endjob_step)
    process.schedule.append(process.myAnalyzer_step)
# End of inserted code
















#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
