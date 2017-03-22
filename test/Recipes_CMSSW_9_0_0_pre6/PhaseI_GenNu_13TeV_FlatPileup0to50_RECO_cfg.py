# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: --evt_type SingleNuE10_cfi --python_filename PhaseI_GenNu_13TeV_FlatPileup0to50_RECO_cfg.py --fileout file:PhaseI_GenNu_13TeV_FlatPileup0to50_RECO.root --process PrivateRECO -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --conditions auto:phase1_2017_realistic --era Run2_2017 --geometry DB:Extended --eventcontent RECOSIM --pileup Flat_0_50_25ns --filein /store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/0047A0C1-BDEC-E611-8FD9-0CC47A4D7630.root --pileup_input dbs:/RelValMinBias_13/CMSSW_9_0_0_pre4-90X_upgrade2017_realistic_v6-v1/GEN-SIM --datatier GEN-SIM-RECO --runUnscheduled -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('PrivateRECO',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_Flat_0_50_25ns_cfi')
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

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/0047A0C1-BDEC-E611-8FD9-0CC47A4D7630.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop LHEXMLStringProduct_*_*_*', 
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))

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
    fileName = cms.untracked.string('file:PhaseI_GenNu_13TeV_FlatPileup0to50_RECO.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PixelDigiedmDetSetVector_simSiPixelDigis__PrivateRECO")
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelBarrelHighTof_PrivateRECO")
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelBarrelLowTof_PrivateRECO")
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelEndcapHighTof_PrivateRECO")
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelEndcapLowTof_PrivateRECO")

# Other statements
process.mix.input.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/0047A0C1-BDEC-E611-8FD9-0CC47A4D7630.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/2C7DC4C7-BDEC-E611-BD99-0CC47A78A42E.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/820F7DD4-BFEC-E611-AE85-0025905B85A2.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/CC752313-C1EC-E611-AE74-0CC47A78A41C.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/EC42F413-C1EC-E611-83F5-0CC47A4C8E82.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/F26E7D3B-C0EC-E611-B203-0CC47A4C8E20.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/FA5A56D4-BEEC-E611-9E62-0CC47A7C354A.root', '/store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/FAC70AF9-BEEC-E611-B627-0CC47A4C8E20.root'])
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
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
