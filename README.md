# PhaseIPixelNtuplizer
"PixelTimingStudy" for the Phase I pixel detector

### &#x1F539; Base recipe:

```bash
export SCRAM_ARCH=slc6_amd64_gcc530
cmsrel CMSSW_9_0_0_pre3
cd CMSSW_9_0_0_pre3/src
cmsenv
# git cms-init #optional, use this, if you want to check out CMSSW packages later
git cms-merge-topic 17384
scram b -j 20
git clone git@github.com:jkarancs/PhaseIPixelNtuplizer.git DPGAnalysis/PhaseIPixelNtuplizer
cd DPGAnalysis/PhaseIPixelNtuplizer
scram b -j 20
cmsRun test/run_PhaseIPixelNtuplizer_MinBias_cfg.py
```

### &#x1F539; Starting from GEN-SIM
The run_PhaseIPixelNtuplizer_MinBias_cfg.py creates/uses MinBias RECO events for input for the Ntuplizer, 1k events were generated with new geometry, but new ones can be made (add argument useRECO=0, saveRECO=1 maxEvents=1000).

### &#x1F539; Running on Reco
This is the default setting.

### &#x1F539; CMSSW config file cmsDriver.py recipe used as a base, can be modified to generate different types of events, change mixing etc

```bash
cmsDriver.py -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --evt_type MinBias_13TeV_pythia8_TuneCUETP8M1_cfi --conditions auto:phase1_2017_realistic --era Run2_2017 --geometry Extended2017 --fileout file:GENSIMRECO_MinBias.root --python_filename=PhaseI_MinBias_cfg.py --runUnscheduled -n 10
```
