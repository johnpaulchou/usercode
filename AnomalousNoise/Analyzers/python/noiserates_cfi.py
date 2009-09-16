import FWCore.ParameterSet.Config as cms

noiserates = cms.EDAnalyzer(
    'NoiseRates',
    rbxCollName = cms.string('hcalnoise'),
    metCollName = cms.string('metNoHF'),
    rootHistFilename = cms.string('noiserateplots.root'),
    trigNames = cms.vstring('L1_SingleJet10','L1_SingleEG1','L1_SingleMuBeamHalo','L1_SingleMuOpen'),
    minRBXEnergy = cms.double(20.0),
    minHitEnergy = cms.double(1.5)
    )
