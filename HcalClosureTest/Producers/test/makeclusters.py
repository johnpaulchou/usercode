import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('HcalClosureTest.Producers.singleparticleclusterproducer_cfi')

readFiles = cms.untracked.vstring()
process.source = cms.Source('PoolSource',fileNames = readFiles)
readFiles.extend( [
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_1.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_2.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_3.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_4.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_5.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_6.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_7.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_8.root',
    'dcap:///pnfs/cms/WAX/resilient/duong/Output_219/Sample/singlePiPlusE50_IDEALv5_crab_9.root' ])

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(100)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('file:skim.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep SingleParticleClusters_*_*_*',
                                                                      'keep recoGenParticles_*_*_*',
                                                                      'keep CaloTowersSorted_*_*_*')
                               )

process.p = cms.Path(process.singleparticleclusterproducer)

process.e = cms.EndPath(process.out)

