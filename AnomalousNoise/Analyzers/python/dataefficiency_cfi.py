import FWCore.ParameterSet.Config as cms

dataefficiency = cms.EDAnalyzer(
    'DataEfficiency',
    rbxCollName = cms.string('hcalnoise'),
    jetCollName = cms.string('sisCone5CaloJets'),
    metCollName = cms.string('met'),
    trackCollName = cms.string('generalTracks'),
    hbheRecHitCollName = cms.string('hbhereco'),
    hfRecHitCollName = cms.string('hfreco'),
    caloTowerCollName = cms.string('towerMaker'),
    rootHistFilename = cms.string('dataeffplots.root'),
)
