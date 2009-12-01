import FWCore.ParameterSet.Config as cms

calcrespcorrdijets = cms.EDProducer(
    'CalcRespCorrDiJets',
    jetCollName = cms.string('iterativeCone5CaloJets'),
    genJetCollName = cms.string('iterativeCone5GenJets'),
    rootHistFilename = cms.string('respcorrdijettree.root'),
    maxDeltaEta = cms.double(0.5),
    minDeltaPhi = cms.double(2.0),
    minTagJetEta = cms.double(1.0),
    maxTagJetEta = cms.double(5.0),
    minJetEt = cms.double(30),
    maxThirdJetEt = cms.double(10),
    maxJetEMF = cms.double(0.9)
    )
