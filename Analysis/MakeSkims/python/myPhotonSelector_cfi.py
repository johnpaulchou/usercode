import FWCore.ParameterSet.Config as cms

mySelectedPatPhotons = cms.EDFilter("MyPATPhotonSelector",
                                    src = cms.InputTag("patPhotons"),
                                    vertexSrc = cms.InputTag("selectedVertices"),
                                    EBRecHitsSrc = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                    vetoPixelSeed = cms.bool(True),
                                    minEt = cms.double(10.0),
                                    maxEta = cms.double(2.6),
                                    maxSwissCross = cms.double(0.95),
                                    muonCleaningSrc = cms.InputTag("patMuons"),
                                    muonCleaningIDs = cms.vstring('AllGlobalMuons', 'AllTrackerMuons'),
                                    muonCleaningMinNumValidHits = cms.uint32(0),
                                    muonCleaningMaxDr = cms.double(0.1),
                                    embedConversions = cms.bool(True)
                                    )
