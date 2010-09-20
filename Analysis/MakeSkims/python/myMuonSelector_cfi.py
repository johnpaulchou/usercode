import FWCore.ParameterSet.Config as cms

mySelectedPatMuons = cms.EDFilter("MyPATMuonSelector",
                                  src = cms.InputTag("patMuons"),
                                  vertexSrc = cms.InputTag("selectedVertices"),
                                  minPt = cms.double(6.0),
                                  maxEta = cms.double(2.5),
                                  )
