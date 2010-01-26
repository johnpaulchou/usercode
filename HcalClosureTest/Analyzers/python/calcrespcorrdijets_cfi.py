import FWCore.ParameterSet.Config as cms

calcrespcorrdijets = cms.EDProducer(
    'CalcRespCorrDiJets',
    jetCollName      = cms.string('sisCone5CaloJets'),
    jetCorrName      = cms.string('L2L3JetCorrectorSC5Calo'),
    genJetCollName   = cms.string('sisCone5GenJets'),
    rootHistFilename = cms.string('dijettree.root'),
    maxDeltaEta      = cms.double(1.5),
    minTagJetEta     = cms.double(0.0),
    maxTagJetEta     = cms.double(5.0),
    minSumJetEt      = cms.double(10.),
    minJetEt         = cms.double(5.0),
    maxThirdJetEt    = cms.double(100.),
    maxJetEMF        = cms.double(0.9),
    debug            = cms.untracked.bool(False)
    )
