# PhaseIPixelNtuplizer
"PixelTimingStudy" for the Phase I pixel detector

### &#x1F539; Base recipe:

```bash
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_6_8_patch1
cd CMSSW_10_6_8_patch1/src
cmsenv
# git cms-init #optional, use this, if you want to check out CMSSW packages later
git clone https://github.com/jkarancs/PhaseIPixelNtuplizer.git DPGAnalysis/PhaseIPixelNtuplizer
cd DPGAnalysis/PhaseIPixelNtuplizer
scram b -j 8
mkdir Example
cd Example
cp $CMSSW_BASE/src/DPGAnalysis/PhaseIPixelNtuplizer/test/Recipes_CMSSW_9_0_0_pre6/PhaseI_TTbar_13TeV_NoPu_RECO_cfg.py .
cp $CMSSW_BASE/src/DPGAnalysis/PhaseIPixelNtuplizer/test/Recipes_CMSSW_9_0_0_pre6/RECO_to_Ntuple_cfg.py .
cmsRun PhaseI_TTbar_13TeV_NoPu_RECO_cfg.py
cmsRun RECO_to_Ntuple_cfg.py
```

### &#x1F539; Running on Reco
This is the default setting.

### &#x1F539; Some example recipes

TTbar RECO, no pileup:

```bash
cmsDriver.py TTbar_13TeV_TuneCUETP8M1_cfi \
--python_filename PhaseI_TTbar_13TeV_NoPu_RECO_cfg.py \
--fileout file:PhaseI_TTbar_13TeV_NoPu_RECO.root \
--process PrivateRECO \
--conditions auto:phase1_2017_realistic \
--era Run2_2017 \
--geometry DB:Extended \
--eventcontent RECOSIM \
-s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,L1Reco,RECO \
--datatier GEN-SIM-RECO \
--runUnscheduled \
-n 10 \
--no_exec
```

MinBias RECO, pileup + added DynIneff:

```bash
cmsDriver.py \
--evt_type SingleNuE10_cfi \
--python_filename PhaseI_GenNu_13TeV_FlatPileup0to50_RECO_cfg.py \
--fileout file:PhaseI_GenNu_13TeV_FlatPileup0to50_RECO.root \
--process PrivateRECO \
-s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO \
--conditions auto:phase1_2017_realistic \
--era Run2_2017 \
--geometry DB:Extended \
--eventcontent RECOSIM \
--pileup Flat_0_50_25ns \
--filein /store/relval/CMSSW_9_0_0_pre4/RelValMinBias_13/GEN-SIM/90X_upgrade2017_realistic_v6-v1/10000/0047A0C1-BDEC-E611-8FD9-0CC47A4D7630.root \
--pileup_input dbs:/RelValMinBias_13/CMSSW_9_0_0_pre4-90X_upgrade2017_realistic_v6-v1/GEN-SIM \
--datatier GEN-SIM-RECO \
--runUnscheduled \
-n 10 \
--no_exec
```

### &#x1F539; cmsRun cfg. additions

```python
# Additional output definition
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PixelDigiedmDetSetVector_simSiPixelDigis__PrivateRECO")        # replace PrivateRECO with your process name
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelBarrelHighTof_PrivateRECO") # replace PrivateRECO with your process name
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelBarrelLowTof_PrivateRECO")  # replace PrivateRECO with your process name
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelEndcapHighTof_PrivateRECO") # replace PrivateRECO with your process name
process.RECOSIMoutput.outputCommands += cms.untracked.vstring("keep PSimHits_g4SimHits_TrackerHitsPixelEndcapLowTof_PrivateRECO")  # replace PrivateRECO with your process name
```

```python
# Pileup
from DPGAnalysis.PhaseIPixelNtuplizer.PoolSource_13TeV_upgrade2017_realistic_v21_v1_81X_GS import pileupFileNames
process.mix.input.fileNames = pileupFileNames
process.mix.input.nbPileupEvents.probFunctionVariable = cms.vint32([
    0, 1, 2, 3, 4, 
    5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 
    15, 16, 17, 18, 19, 
    20, 21, 22, 23, 24, 
    25, 26, 27, 28, 29, 
    30, 31, 32, 33, 34, 
    35, 36, 37, 38, 39, 
    40, 41, 42, 43, 44, 
    45, 46, 47, 48, 49, 
    50])
process.mix.input.nbPileupEvents.probValue = cms.vdouble([
    0.0, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02, 0.02, 0.02, 0.02, 0.02, 
    0.02])
process.mix.input.type = cms.string('probFunction')
```

```python
# Efficiency
from CondCore.DBCommon.CondDBSetup_cfi import *
process.DynIneffDBSource = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    connect = cms.string('sqlite_file:phase1_efficiencies_85.db'), # replace with the db you want to use
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('SiPixelDynamicInefficiencyRcd'),
        tag = cms.string('SiPixelDynamicInefficiency_v1')
        # tag = cms.string(globaltag)
    ))
)
process.es_prefer_DynIneffDBSource = cms.ESPrefer("PoolDBESSource","DynIneffDBSource")
```
