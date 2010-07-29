import FWCore.ParameterSet.Config as cms

mySelectedVertices = cms.EDFilter('VertexSelector',
                                  src = cms.InputTag('offlinePrimaryVertices'),
                                  cut = cms.string('ndof() > 4 && abs(z())<=15.0 && abs(position().rho())<2.0')
                                  )
                                  
