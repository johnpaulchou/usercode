import FWCore.ParameterSet.Config as cms

## Primary Vertex Filter
primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                   vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                   minimumNDOF = cms.uint32(4) ,
                                   maxAbsZ = cms.double(24),
                                   maxd0 = cms.double(2)
                                   )
## "monster event" filter
monsterEventFilter = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.2)
                                  )

## HCAL noise filter
from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

customFilterSequence = cms.Sequence(primaryVertexFilter+monsterEventFilter+HBHENoiseFilter)
