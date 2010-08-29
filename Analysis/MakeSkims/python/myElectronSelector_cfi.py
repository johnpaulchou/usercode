import FWCore.ParameterSet.Config as cms

mySelectedPatElectrons = cms.EDFilter("MyPATElectronSelector",
                                      src = cms.InputTag("patElectrons"),
                                      vertexSrc = cms.InputTag("selectedVertices"),
                                      EBRecHitsSrc = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                      tracksSrc = cms.InputTag("generalTracks"),
                                      isolatedLabel = cms.string("isolated"),
                                      nonisolatedLabel = cms.string("nonisolated"),
                                      looseLabel = cms.string("loose"),
                                      requireCaloDrivenSeed = cms.bool(True),
                                      minEt = cms.double(5.0),
                                      minSuperClusterEt = cms.double(2.0),
                                      maxEta = cms.double(999.),
                                      maxSuperClusterEta = cms.double(2.5),
                                      maxNumLostHits = cms.int32(2),
                                      maxTransverseIP = cms.double(0.04),
                                      maxDeltaZ = cms.double(1.0),
                                      maxSwissCross = cms.double(0.95),
                                      conversionMinSep = cms.double(0.02),
                                      conversionMinDcot = cms.double(0.02),
                                      maxSihihEB = cms.double(0.01),
                                      maxDphiEB = cms.double(0.8),
                                      maxDetaEB = cms.double(0.007),
                                      maxHoeEB = cms.double(0.12),
                                      maxSihihEE = cms.double(0.03),
                                      maxDphiEE = cms.double(0.7),
                                      maxDetaEE = cms.double(9999999.),
                                      maxHoeEE = cms.double(0.05),
                                      maxIsolation = cms.double(0.15),
                                      muonCleaningSrc = cms.InputTag("patMuons"),
                                      muonCleaningIDs = cms.vstring('AllGlobalMuons', 'AllTrackerMuons'),
                                      muonCleaningMinNumValidHits = cms.uint32(0),
                                      muonCleaningMaxDr = cms.double(0.1),
                                      )
                                                         