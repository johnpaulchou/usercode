import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

# reconstruction sequence
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/VtxSmearedEarly10TeVCollision_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/L1Reco_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = "MC_31X_V9::All"

# special running for hcalnoise filter
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.writeAllRBXs = cms.bool(True)
process.hcalnoise.maxCaloTowerIEta = cms.int32(20)
process.hcalnoise.maxTrackEta = cms.double(2.0) 

# Tone down the logging messages, MessageLogger!
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

# run over files
readFiles = cms.untracked.vstring()
import FWCore.Utilities.FileUtils as FileUtils
readFiles = cms.untracked.vstring( FileUtils.loadListFromFile ('filelists/QCD_Pt15_raw.list') )
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep CaloTowersSorted_*_*_*',
                                           'keep recoHcalNoiseRBXs_*_*_*',
                                           'keep HcalNoiseSummary_*_*_*',
                                           'keep recoCaloMETs_*_*_*', 
                                           'keep recoCaloJets_*_*_*',
                                           'keep recoTracks_generalTracks_*_*',
                                           'keep L1GlobalTriggerRecord_*_*_*'
                                           ),
    fileName = cms.untracked.string('out.root')
    )

# path & schedule (w/o unneccessary steps)
process.digitisation_step = cms.Path(process.pdigi)
process.digi2raw_step = cms.Path(process.siPixelRawData*process.SiStripDigiToRaw*process.ecalPacker*process.esDigiToRaw*process.hcalRawData)
process.raw2digi_step = cms.Path(process.siPixelDigis+process.siStripDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis)
process.localreco = cms.Sequence(process.trackerlocalreco+process.calolocalreco)
process.globalreco = cms.Sequence(process.offlineBeamSpot+process.recopixelvertexing*process.ckftracks+process.ecalClusters+process.caloTowersRec*process.vertexreco*process.recoJets)
process.highlevelreco = cms.Sequence(process.metNoHF+process.hcalnoise)
process.reconstruction_step = cms.Path(process.localreco*process.globalreco*process.highlevelreco)
process.out_step = cms.EndPath(process.output)
process.schedule = cms.Schedule(process.digitisation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.out_step)
