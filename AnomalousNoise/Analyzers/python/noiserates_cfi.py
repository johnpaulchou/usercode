import FWCore.ParameterSet.Config as cms

noiserates = cms.EDAnalyzer(
    'NoiseRates',
    rbxCollName = cms.string('hcalnoise'),
    metCollName = cms.string('metNoHF'),
    hbheRecHitCollName = cms.string('hbhereco'),
    caloTowerCollName = cms.string('towerMaker'),
    rootHistFilename = cms.string('noiserateplots.root'),
    findTrigger = cms.bool(True),
    trigName = cms.string('L1_SingleJet10'),
    minRBXEnergy = cms.double(20.0),
    minHitEnergy = cms.double(1.5)
    )
