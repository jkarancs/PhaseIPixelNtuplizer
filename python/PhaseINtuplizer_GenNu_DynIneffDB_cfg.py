# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: --step=GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --datatier GEN-SIM-RECO --mc --evt_type SingleNuE10_cfi --conditions auto:phase1_2017_realistic --era Run2_2017 --fileout file:GENSIMRECO.root --geometry DB:Extended --python_filename=TimingStudy_PhaseI_GenNu_DynIneffDB_cfg.py --number=10 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
# process.load('SimGeneral.MixingModule.mixNoPU_cfi') # -
process.load('SimGeneral.MixingModule.mix_Flat_0_50_25ns_cfi') # +
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
# process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
	annotation = cms.untracked.string('SingleNuE10_cfi nevts:10'),
	name = cms.untracked.string('Applications'),
	version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
	SelectEvents = cms.untracked.PSet(
		SelectEvents = cms.vstring('generation_step')
	),
	dataset = cms.untracked.PSet(
		dataTier = cms.untracked.string('GEN-SIM-RECO'),
		filterName = cms.untracked.string('')
	),
	eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
	fileName = cms.untracked.string('file:GENSIMRECO.root'),
	outputCommands = process.RECOSIMEventContent.outputCommands,
	splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.XMLFromDBSource.label = cms.string("Extended")
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.generator = cms.EDProducer("FlatRandomEGunProducer",
	AddAntiParticle = cms.bool(False),
	PGunParameters = cms.PSet(
		MaxE = cms.double(10.01),
		MaxEta = cms.double(2.5),
		MaxPhi = cms.double(3.14159265359),
		MinE = cms.double(9.99),
		MinEta = cms.double(-2.5),
		MinPhi = cms.double(-3.14159265359),
		PartID = cms.vint32(12)
	),
	Verbosity = cms.untracked.int32(0),
	firstRun = cms.untracked.uint32(1),
	psethack = cms.string('single Nu E 10')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
# process.reconstruction_step = cms.Path(process.pixeltrackerlocalreco)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 



#--------------- Added for TimingStudy ---------------

#---------------------------
#  Pile-up (RunIISummer15GS)
#---------------------------
runOnGrid = False

# Flat pileup modification: 1-20 uniform
process.mix.input.nbPileupEvents.probFunctionVariable = cms.vint32(range(0, 21))
process.mix.input.nbPileupEvents.probValue = cms.vdouble([0.0] + [0.05] * 20)

if runOnGrid:
	from DPGAnalysis.PhaseIPixelNtuplizer.PoolSource_13TeV_RunII_pre17_GS import RunII_GS_Phase_I_2017_pileup_file_paths
	process.mix.input.fileNames = RunII_GS_Phase_I_2017_pileup_file_paths
else:
	process.mix.input.fileNames = cms.untracked.vstring(
		'file:/data/hunyadi/CMSSW/PhaseI_SIM/Tracker_material/CMSSW_8_1_0_pre8/src/Test/Stepenkent/out_step_1.root',
		# '/store/user/ahunyadi/GEN_SIM/PhaseI_CMSSW81X/PhaseI_81X_mcRun2_GEN_SIM_evt100000/160708_091624/0000/GEN_SIM_81x_PHASEI_104.root',
		# "file:/data/store/user/ahunyadi/GEN_SIM/PhaseI_CMSSW81X/PhaseI_81X_mcRun2_GEN_SIM_evt100000/160708_091624/0000/GEN_SIM_81x_PHASEI_104.root",
	)

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
process.PhaseIPixelNtuplizer_step = cms.Path(process.PhaseINtuplizerPlugin)

#---------------------------
#  Path/Schedule
#---------------------------
process.schedule.remove(process.RECOSIMoutput_step)
process.schedule.remove(process.endjob_step)
process.schedule.remove(process.genfiltersummary_step)
process.schedule.append(process.PhaseIPixelNtuplizer_step)

#---------------------------
#  MessageLogger
#---------------------------
# process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger = cms.Service("MessageLogger",
	destinations = cms.untracked.vstring('cerr'),
	cerr = cms.untracked.PSet(threshold  = cms.untracked.string('DEBUG')),
	debugModules = cms.untracked.vstring('PhaseINtuplizerPlugin'))
