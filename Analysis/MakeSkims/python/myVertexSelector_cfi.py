import FWCore.ParameterSet.Config as cms

mySelectedVertices = cms.EDFilter('VertexSelector',
                                  src = cms.InputTag('offlinePrimaryVertices'),
                                  cut = cms.string('!isFake && ndof() > 4 && abs(z())<=24.0 && abs(position().rho())<2.0')
                                  )
                                  
