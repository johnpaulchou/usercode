import FWCore.ParameterSet.Config as cms

# define the dataset here
dataset = 2 # 1=CRAFT, 2=MinBiasData

process = cms.Process("ANALYSIS")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('AnomalousNoise.Analyzers.isolatednoise_cfi')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('RecoLocalCalo.CaloTowersCreator.calotowermaker_cfi')

process.load('RecoJets.Configuration.CaloTowersES_cfi')
process.load('RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')

if dataset==1:
    process.GlobalTag.globaltag = 'CRAFT09_R_V10::All'
elif dataset==2:
    process.GlobalTag.globaltag = 'GR_R_35X_V7::All'

process.isolatednoise = cms.EDAnalyzer(
    'IsolatedNoise',

    # this way we use the same settings as calotowers
    hbheInput = process.calotowermaker.hbheInput,
    ecalInputs = process.calotowermaker.ecalInputs,
    HBThreshold = process.calotowermaker.HBThreshold,
    HESThreshold = process.calotowermaker.HESThreshold,
    HEDThreshold = process.calotowermaker.HEDThreshold,
    EBThreshold = process.calotowermaker.EBThreshold,
    EEThreshold = process.calotowermaker.EEThreshold,
    HcalAcceptSeverityLevel = process.calotowermaker.HcalAcceptSeverityLevel,
    EcalAcceptSeverityLevel = process.calotowermaker.EcalAcceptSeverityLevel,
    UseHcalRecoveredHits = process.calotowermaker.UseHcalRecoveredHits,
    UseEcalRecoveredHits = process.calotowermaker.UseEcalRecoveredHits,
)

# Tone down the logging messages, MessageLogger!
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

# run over files
readFiles = cms.untracked.vstring()
if dataset==1:
    readFiles.append('/store/data/CRAFT09/Calo/RECO/v1/000/112/237/08AEF410-D093-DE11-93B4-001D09F28F25.root')
elif dataset==2:
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/00064D6A-8D4C-DF11-B7C4-002618943829.root')
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.p = cms.Path(process.isolatednoise)
