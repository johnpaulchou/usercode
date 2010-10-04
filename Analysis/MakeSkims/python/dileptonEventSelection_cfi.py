import FWCore.ParameterSet.Config as cms

import os;

def setupEventSelection(process):
    tag = 'DileptonSelection'

    ## HB/HE noise filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

    ## dilepton event selection
    process.dileptonEventSelection = cms.EDFilter('DileptonEventSelector',
                                                  genParticleSrc = cms.InputTag('genParticles'),
                                                  minNumGenLeptons = cms.int32(2),
                                                  motherPdgIds = cms.vint32(23, 24, -24),
                                                  requiredTriggers = cms.vstring('HLT_Mu9','HLT_Ele10_LW_L1R'),
                                                  vertexSrc = cms.InputTag('offlinePrimaryVertices'),
                                                  minNumGoodVertices = cms.int32(1),
                                                  doNoiseStep = cms.bool(True),
                                                  noiseResultSrc = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
                                                  trackSrc = cms.InputTag('generalTracks'),
                                                  minHighQualityTrackFraction = cms.double(0.25),
                                                  minNumTracksForMonsterEventCut = cms.int32(11),
                                                  patElectronSrc = cms.InputTag('cleanPatElectrons'),
                                                  patMuonSrc = cms.InputTag('cleanPatMuons'),
                                                  minElectronEt = cms.double(20.0),
                                                  minElectronSuperClusterEt = cms.double(10.0),
                                                  minMuonPt = cms.double(20.0),
                                                  excludeLepInvMass = cms.vdouble(0.0, 10.0),
                                                  excludeSameTypeLepInvMass = cms.vdouble(76.0, 106.0),
                                                  )

    # rename output file
    oldname = os.path.splitext(process.out.fileName.value())[0]
    process.out.fileName = cms.untracked.string(oldname+tag+'.root')

    # setup file service
    process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string(oldname+tag+'Plots.root'),
                                       closeFileFast = cms.untracked.bool(True)
                                       )

    # setup sequence
    process.dileptonEventSelectionSequence = cms.Sequence(process.HBHENoiseFilterResultProducer*process.dileptonEventSelection)
    process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )
    process.p = cms.Path(process.patDefaultSequence*process.dileptonEventSelectionSequence)
