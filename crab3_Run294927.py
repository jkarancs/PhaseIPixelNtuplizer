import os
import glob

GlobalTag='92X_dataRun2_Express_v2'
DataSet='/ExpressPhysics/Run2017A-Express-v1/FEVT'
DataTier=DataSet.split('/')[3]

from WMCore.Configuration import Configuration
config = Configuration()

PPNdir = os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PhaseIPixelNtuplizer/'

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Run294927' #can be anything

config.section_('JobType')
config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = PPNdir+'test/run_PhaseIPixelNtuplizer_Data_92X_cfg.py'
config.JobType.pyCfgParams = ['globalTag=92X_dataRun2_Express_v2','dataTier='+DataTier, 'outputFileName=Ntuple.root']
config.JobType.inputFiles = []
config.JobType.outputFiles = ['Ntuple.root']
config.JobType.disableAutomaticOutputCollection = True
#config.JobType.priority = -1

config.section_('Data')
config.Data.inputDataset = '/ExpressPhysics/Run2017A-Express-v1/FEVT'
if DataTier == 'AOD': config.Data.useParent = True
config.Data.outLFNDirBase = '/store/user/jkarancs/PhaseIPixelNtuplizer/2017/'
config.Data.outputDatasetTag = 'v0405_default_920p2_92X_dataRun2_Express_v2_Run294927'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1

#config.Data.totalUnits = 2
config.Data.runRange = '294927'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
