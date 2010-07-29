import FWCore.ParameterSet.Config as cms

mySelectedPatElectrons = cms.EDFilter("MyPATElectronSelector",
                                      src = cms.InputTag("patElectrons"),
                                      vertexSrc = cms.InputTag("selectedVertices"),
                                      isolatedLabel = cms.string("isolated"),
                                      nonisolatedLabel = cms.string("nonisolated"),
                                      electronID = cms.string("eidLoose"),
                                      minEt = cms.double(5.0),
                                      maxEta = cms.double(2.4),
                                      maxNumLostHits = cms.int32(2),
                                      maxTransverseIP = cms.double(0.04),
                                      maxIsolation = cms.double(0.15),
                                      muonCleaningSrc = cms.InputTag("patMuons"),
                                      muonCleaningIDs = cms.vstring('AllGlobalMuons', 'AllTrackerMuons'),
                                      muonCleaningMinNumValidHits = cms.uint32(11),
                                      muonCleaningMaxDr = cms.double(0.1),
                                      )
                                                         
