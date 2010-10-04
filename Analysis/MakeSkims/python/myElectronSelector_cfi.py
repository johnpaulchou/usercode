import FWCore.ParameterSet.Config as cms

mySelectedPatElectrons = cms.EDFilter("MyPATElectronSelector",
                                      src = cms.InputTag("patElectrons"),
                                      vertexSrc = cms.InputTag("offlinePrimaryVertices"),
                                      EBRecHitsSrc = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                      tracksSrc = cms.InputTag("generalTracks"),
                                      requireCaloDrivenSeed = cms.bool(True),
                                      minEt = cms.double(10.0),
                                      minSuperClusterEt = cms.double(5.0),
                                      maxEta = cms.double(2.6),
                                      maxSwissCross = cms.double(0.95),
                                      muonCleaningSrc = cms.InputTag("patMuons"),
                                      muonCleaningIDs = cms.vstring('AllGlobalMuons', 'AllTrackerMuons'),
                                      muonCleaningMinNumValidHits = cms.uint32(0),
                                      muonCleaningMaxDr = cms.double(0.1),
                                      )
                                                         
