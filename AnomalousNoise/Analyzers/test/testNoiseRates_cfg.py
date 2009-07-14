import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

process.load('FWCore.MessageService.MessageLogger_cfi')

# global tag needed for L1 information
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'CRAFT0831X_V1::All'

#load noise rate analyzer
process.load('AnomalousNoise.Analyzers.noiserates_cfi')

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource", fileNames = readFiles)

#for i in range(1, 113):
for i in range(1, 40):
    if(i==5 or i==8 or i==11 or i==12 or i==14 or i==16 or i==19 or i==30 or i==32 or i==35 or i==36 or i==43 or i==48 or i==49 or i==51 or i==54 or i==70 or i==71 or i==74 or i==81 or i==86 or i==88 or i==90 or i==93 or i==97 or i==103 or i==105 or i==108):
        continue
    readFiles.append('dcap://pnfs/cms/WAX/resilient/johnpaul/AnomalousNoise/CaloCommissioning08_R68288/CaloCommissioning08_R68288_'+str(i)+'.root')


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(5000)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# timing
#process.Timing = cms.Service('Timing')

process.p = cms.Path(process.noiserates)
