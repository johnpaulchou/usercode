import FWCore.ParameterSet.Config as cms

# define the dataset here
dataset = 3 # 1=QCDPt15, 2 = QCDPt300, 3 = QCDPt3000, 4=R112237

process = cms.Process("ANALYSIS")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('AnomalousNoise.Analyzers.dataefficiency_cfi')

# Tone down the logging messages, MessageLogger!
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
import FWCore.Utilities.FileUtils as FileUtils
if dataset == 1:
    process.dataefficiency.rootHistFilename = cms.string('qcd15.root')
    for i in range(1,21):
        readFiles.append('dcap:///pnfs/cms/WAX/11/store/user/johnpaul/AnomalousNoise/QCD_Pt15/out_'+str(i)+'.root')

elif dataset == 2:
    process.dataefficiency.rootHistFilename = cms.string('qcd300.root')
    for i in range(1,42):
        readFiles.append('dcap:///pnfs/cms/WAX/11/store/user/johnpaul/AnomalousNoise/QCD_Pt300/out_'+str(i)+'.root')

elif dataset == 3:
    process.dataefficiency.rootHistFilename = cms.string('qcd3000.root')
    for i in range(1,41):
        readFiles.append('dcap:///pnfs/cms/WAX/11/store/user/johnpaul/AnomalousNoise/QCD_Pt3000/out_'+str(i)+'.root')

elif dataset == 4:
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = 'CRAFT09_R2_V2::All'
    process.dataefficiency.rootHistFilename = cms.string('R112237.root')
    for i in range(1, 249):
        readFiles.append('dcap:///pnfs/cms/WAX/11/store/user/johnpaul/AnomalousNoise/R112237/out_'+str(i)+'.root')


process.source = cms.Source ("PoolSource",fileNames = readFiles)
print readFiles


# path & schedule
process.p = cms.Path(process.dataefficiency)
