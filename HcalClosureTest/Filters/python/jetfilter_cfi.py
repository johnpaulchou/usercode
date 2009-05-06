import FWCore.ParameterSet.Config as cms

jetfilter = cms.EDFilter(
    'JetFilter',
    minNumJets = cms.untracked.int32(2),
    maxNumJets = cms.untracked.int32(999999),

    minFirstJetEt = cms.untracked.double(10.),
    maxFirstJetEt = cms.untracked.double(999999.),
    minSecondJetEt = cms.untracked.double(10.),
    maxSecondJetEt = cms.untracked.double(999999.),
    minThirdJetEt = cms.untracked.double(0.),
    maxThirdJetEt = cms.untracked.double(10.),
    minFourthJetEt = cms.untracked.double(0.),
    maxFourthJetEt = cms.untracked.double(10.),
    minRestJetEt = cms.untracked.double(0.),
    maxRestJetEt = cms.untracked.double(10.),
    
    caloJetCollName = cms.untracked.string('iterativeCone5CaloJets')
    )
