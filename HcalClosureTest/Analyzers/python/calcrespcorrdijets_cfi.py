import FWCore.ParameterSet.Config as cms

calcrespcorrdijets = cms.EDProducer(
    'CalcRespCorrDiJets',
    jetCollName = cms.string('iterativeCone5CaloJets'),
    rootHistFilename = cms.string('respcorrdijetplots.root'),
    maxDeltaEta = cms.double(0.6),
    minDeltaPhi = cms.double(2.0),
    minJetEt = cms.double(30),
    maxThirdJetEt = cms.double(10)
    )
