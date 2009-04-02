import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('HcalClosureTest.Producers.singleparticleclusterproducer_cfi')

process.load('DataHandling.ConfigFiles.DiJetPt600to800_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(100)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('file:skim.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep SingleParticleClusters_*_*_*',
                                                                      'keep recoGenParticles_*_*_*',
                                                                      'keep CaloTowersSorted_*_*_*',
                                                                      'keep EventWeight_*_*_*')
                               )

process.datahandling_step = cms.Path(process.DataHandling)
process.cluster_step = cms.Path(process.singleparticleclusterproducer)
process.out_step = cms.EndPath(process.out)

process.schedule = cms.Schedule(process.datahandling_step,
                                process.cluster_step,
                                process.out_step)

