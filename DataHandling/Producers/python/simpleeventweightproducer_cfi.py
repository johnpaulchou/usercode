import FWCore.ParameterSet.Config as cms

simpleEW = cms.EDProducer(
    'SimpleEventWeightProducer',
    integratedLumi = cms.untracked.double(1000.),
    integratedLumiError = cms.untracked.double(0.),
    crossSection = cms.untracked.double(1.),
    crossSectionError = cms.untracked.double(0.),
    scaleFactor = cms.untracked.double(1.),
    scaleFactorError = cms.untracked.double(0.),
    numEvents = cms.untracked.int32(1)
    )
