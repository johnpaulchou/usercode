import FWCore.ParameterSet.Config as cms

calcrespcorr = cms.EDProducer(
    'CalcRespCorr',
    clstrCollName = cms.string('ParticleClustering'),
    rootHistFilename = cms.string('respcorrplots.root'),
    maxDeltaR = cms.double(0.3),
    maxModifiedEMF = cms.double(0.05),
    respCorr = cms.vdouble(1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HF-
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HE-
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HB-
                           0.00, # gap
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HB+
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, # HE+
                           1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 # HF+
                           )
    )
