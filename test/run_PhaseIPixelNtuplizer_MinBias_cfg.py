# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --evt_type MinBias_cfi --conditions auto:phase1_2017_realistic --era Run2_2017 --geometry DB:Extended --fileout file:MinBias_GENSIMRECO.root --python_filename=PahseI_MinBias_cfg.py --runUnscheduled -n 10
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('MinBias_cfi nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:MinBias_GENSIMRECO.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.XMLFromDBSource.label = cms.string("Extended")
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters'),
        processParameters = cms.vstring('MSEL=0         ! User defined processes', 
            'MSUB(11)=1     ! Min bias process', 
            'MSUB(12)=1     ! Min bias process', 
            'MSUB(13)=1     ! Min bias process', 
            'MSUB(28)=1     ! Min bias process', 
            'MSUB(53)=1     ! Min bias process', 
            'MSUB(68)=1     ! Min bias process', 
            'MSUB(92)=1     ! Min bias process, single diffractive', 
            'MSUB(93)=1     ! Min bias process, single diffractive', 
            'MSUB(94)=1     ! Min bias process, double diffractive', 
            'MSUB(95)=1     ! Min bias process'),
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model', 
            'MSTU(21)=1     ! Check on possible errors during program execution', 
            'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
            'PARP(89)=1960. ! sqrts for which PARP82 is set', 
            'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
            'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
            'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
            'PARP(67)=2.5    ! amount of initial-state radiation', 
            'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(62)=1.25   ! ', 
            'PARP(64)=0.2    ! ', 
            'MSTP(91)=1      !', 
            'PARP(91)=2.1   ! kt distribution', 
            'PARP(93)=15.0  ! ')
    ),
    comEnergy = cms.double(10000.0),
    filterEfficiency = cms.untracked.double(1.0),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(0)
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq











#-------------------------- BEGIN INSERTED CODE --------------------------------

#--------------- Added for TimingStudy ---------------

process.maxEvents.input = -1

#---------------------------
#  Pile-up (RunIISummer15GS)
#---------------------------
runOnGrid = False
saveRECO  = False
useRECO   = True

# Flat pileup modification: 1-20 uniform
##  process.mix.input.nbPileupEvents.probFunctionVariable = cms.vint32(range(0, 21))
##  process.mix.input.nbPileupEvents.probValue = cms.vdouble([0.0] + [0.05] * 20)
##  
##  if runOnGrid:
##  	from DPGAnalysis.PhaseIPixelNtuplizer.PoolSource_13TeV_RunII_pre17_GS import RunII_GS_Phase_I_2017_pileup_file_paths
##  	process.mix.input.fileNames = RunII_GS_Phase_I_2017_pileup_file_paths
##  else:
##  	process.mix.input.fileNames = cms.untracked.vstring(
##  		'file:/data/hunyadi/CMSSW/PhaseI_SIM/Tracker_material/CMSSW_8_1_0_pre8/src/Test/Stepenkent/out_step_1.root',
##  		# '/store/user/ahunyadi/GEN_SIM/PhaseI_CMSSW81X/PhaseI_81X_mcRun2_GEN_SIM_evt100000/160708_091624/0000/GEN_SIM_81x_PHASEI_104.root',
##  		# "file:/data/store/user/ahunyadi/GEN_SIM/PhaseI_CMSSW81X/PhaseI_81X_mcRun2_GEN_SIM_evt100000/160708_091624/0000/GEN_SIM_81x_PHASEI_104.root",
##  	)

#---------------------------
#  DynIneff from DB
#---------------------------
useSqlite = False

if useSqlite:
	from CondCore.CondDB.CondDB_cfi import *
	process.DynIneffDBSource = cms.ESSource("PoolDBESSource",
		DBParameters = cms.PSet(
			authenticationPath = cms.untracked.string(''),
			authenticationSystem = cms.untracked.int32(0),
			security = cms.untracked.string(''),
			messageLevel = cms.untracked.int32(0),
		),
		connect = cms.string('sqlite_file:hardpixelineff_0_05_for_testing_81x.db'),
		toGet = cms.VPSet(cms.PSet(
			record = cms.string('SiPixelDynamicInefficiencyRcd'),
			tag = cms.string('SiPixelDynamicInefficiency_v1')
		))
	)
	process.es_prefer_DynIneffDBSource = cms.ESPrefer("PoolDBESSource","DynIneffDBSource")

#---------------------------
#  PhaseIPixelNtuplizer
#---------------------------
process.PhaseINtuplizerPlugin = cms.EDAnalyzer('PhaseIPixelNtuplizer')
process.PhaseINtuplizerPlugin.trajectoryInput = cms.InputTag('generalTracks')
#process.PhaseINtuplizerPlugin.trajectoryInput = cms.InputTag('initialStepTracks')
process.PhaseIPixelNtuplizer_step = cms.Path(process.PhaseINtuplizerPlugin)

#---------------------------
#  Path/Schedule
#---------------------------
if useRECO:
    process.setName_("NTUPLE")
    process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
	    # This is the file you create with saveRECO, by default
	    #'file:MinBias_GENSIMRECO.root'
	    # 50k event made with new geom, generic CPE
	    #'file:/data/jkarancs/CMSSW/PhaseI/Latest/GENSIMRECO_MuPt10_GenericReco_50000.root'
	    # 810 relval
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/04BA77F5-4ABF-E611-9CEC-0CC47A78A2F6.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/0A9CE862-43BF-E611-B83A-0025905A6132.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/583A740D-58BF-E611-B960-0CC47A4C8ED8.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/86563DDC-43BF-E611-A69C-0CC47A4D7678.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/865B21CC-42BF-E611-BE4B-0CC47A78A45A.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/8E1A04B2-58BF-E611-AE25-0CC47A7C345C.root',
	    'file:/data/store/relval/CMSSW_8_1_0/RelValMinBias_13/GEN-SIM-RECO/81X_upgrade2017_realistic_v26_HLT2017-v1/10000/F04F1CE0-43BF-E611-B4F2-0025905A6132.root'
	    ))
    process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
    process.PhaseINtuplizerPlugin.trajectoryInput = 'TrackRefitter'
    process.PhaseIPixelNtuplizer_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter*process.PhaseINtuplizerPlugin)
    process.schedule = cms.Schedule(process.PhaseIPixelNtuplizer_step)
else:
    if not saveRECO: process.schedule.remove(process.RECOSIMoutput_step)
    process.schedule.remove(process.endjob_step)
    process.schedule.remove(process.genfiltersummary_step)
    process.schedule.append(process.PhaseIPixelNtuplizer_step)

#---------------------------
#  MessageLogger
#---------------------------
#process.MessageLogger = cms.Service("MessageLogger",
#	destinations = cms.untracked.vstring('cerr'),
#	cerr = cms.untracked.PSet(threshold  = cms.untracked.string('DEBUG')),
#	debugModules = cms.untracked.vstring('PhaseINtuplizerPlugin'))
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#--------------------------- END INSERTED CODE ---------------------------------













#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Customisation from command line
