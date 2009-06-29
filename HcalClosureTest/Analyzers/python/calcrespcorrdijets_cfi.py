import FWCore.ParameterSet.Config as cms

calcrespcorrdijets = cms.EDProducer(
    'CalcRespCorrDiJets',
    jetCollName = cms.string('iterativeCone5CaloJets'),
    rootHistFilename = cms.string('respcorrdijetplots.root'),
    maxDeltaEta = cms.double(0.6),
    minDeltaPhi = cms.double(2.0),
    maxModifiedEMF = cms.double(0.80),
    minJetEt = cms.double(70),
    maxThirdJetEt = cms.double(10),
    maxCalibratedIEta = cms.int32(15),
    respCorr = cms.vdouble(1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HF-
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HE-
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HB-
                           0.00, # gap
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HB+
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HE+
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 # HF+
                           )
    )
