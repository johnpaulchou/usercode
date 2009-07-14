import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

# Conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
process.prefer("GlobalTag")

# Magnetic field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

#raw-to-digi
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.L1GlobalTriggerRawToDigi.l1GtRecord_cfi')
process.RawToDigi = cms.Sequence(process.gctDigis+process.gtDigis+process.gtEvmDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis)

#reco for cosmics
process.load('RecoLocalCalo.Configuration.RecoLocalCalo_cff')
process.load('RecoJets.Configuration.RecoCaloTowersGR_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.reconstructionCosmics = cms.Sequence(process.l1GtRecord*process.calolocalreco*process.recoCaloTowersGR*process.hcalnoise)

# rate analyzer
process.load('AnomalousNoise.Analyzers.noiserates_cfi')

process.hcalnoise.dropRefVectors = cms.bool(False)
process.hcalnoise.minRBXEnergy = cms.double(10)
process.hcalnoise.minHPDEnergy = cms.double(10)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.fillJets = cms.bool(False)

# run over files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FE92951A-3CA8-DD11-8981-001D09F23A4D.root')
readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FCEA0CB3-3FA8-DD11-82E6-001D09F28D4A.root')

process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1), run = cms.untracked.int32(68288) )

# Tone down the logging messages, MessageLogger!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# output
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
#                                           'keep HBHEDataFramesSorted_*_*_*',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep CaloTowersSorted_*_*_*',
                                           'keep recoHcalNoiseRBXs_*_*_*',
                                           'keep HcalNoiseSummary_*_*_*',
                                           'keep L1GlobalTriggerRecord_*_*_*'
                                           ),
    fileName = cms.untracked.string('out.root')
    )


# schedule
process.digi = cms.Path(process.RawToDigi)
process.reco = cms.Path(process.reconstructionCosmics)
process.ana = cms.Path(process.noiserates)
process.e = cms.EndPath(process.output)

#process.schedule = cms.Schedule(process.digi,process.reco,process.ana,process.e)
process.schedule = cms.Schedule(process.digi,process.reco,process.ana)
