import FWCore.ParameterSet.Config as cms

mySelectedPatMuons = cms.EDFilter("MyPATMuonSelector",
                                  src = cms.InputTag("patMuons"),
                                  vertexSrc = cms.InputTag("selectedVertices"),
                                  isolatedLabel = cms.string("isolated"),
                                  nonisolatedLabel = cms.string("nonisolated"),
                                  muonID = cms.string("GlobalMuonPromptTight"),
                                  minPt = cms.double(5.0),
                                  maxEta = cms.double(2.5),
                                  minNumValidHits = cms.uint32(11),
                                  maxNormChi2 = cms.double(10.0),
                                  maxIsolation = cms.double(0.15),
                                  maxTransverseIP = cms.double(0.02),
                                  )
