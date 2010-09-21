import FWCore.ParameterSet.Config as cms

import os;

from PhysicsTools.PatAlgos.tools.jetTools import *

def setupEventSelection(process):
    tag = 'DijetSelection'

    ## add AK7 calojets
    addJetCollection(process,cms.InputTag('ak7CaloJets'),
                     'AK7', "Calo",
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK7', 'Calo'),
                     doType1MET   = False,
                     doL1Cleaning = False,
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak7GenJets"),
                     doJetID      = True,
                     jetIdLabel   = 'ak7'
                     )
    
    ## add AK7 PF jets
    addJetCollection(process,cms.InputTag('ak7PFJets'),
                     'AK7', 'PF',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK7', 'PF'),
                     doType1MET   = False, ## there is no use to apply residual jet corrections on CaloMET
                     doL1Cleaning = False,
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak7GenJets"),
                     doJetID      = False  ## there is no use of jetID for Pflow jets
                     )

    ## HB/HE noise filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

    ## dijet event selection
    process.dijetEventSelection = cms.EDFilter('DijetEventSelector',
                                               HLTSrc = cms.InputTag('TriggerResults','','HLT'),
                                               vertexSrc = cms.InputTag('selectedVertices'),
                                               minNumGoodVertices = cms.int32(1),
                                               doNoiseStep = cms.bool(True),
                                               noiseResultSrc = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
                                               trackSrc = cms.InputTag('generalTracks'),
                                               minHighQualityTrackFraction = cms.double(0.25),
                                               minNumTracksForMonsterEventCut = cms.int32(11),
                                               jetSrc = cms.InputTag('selectedPatJetsAK7Calo'),
                                               leadJetMinRawPt = cms.double(15.0),
                                               leadJetMinCorrPt = cms.double(15.0),
                                               leadJetMaxAbsEta = cms.double(2.5),
                                               subLeadJetMinRawPt = cms.double(15.0),
                                               subLeadJetMinCorrPt = cms.double(15.0),
                                               subLeadJetMaxAbsEta = cms.double(2.5),
                                               minDijetMass = cms.double(100.0),
                                               maxDijetDeta = cms.double(1.3),
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
    process.dijetEventSelectionSequence = cms.Sequence(process.HBHENoiseFilterResultProducer*process.dijetEventSelection)
    process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )
    process.p = cms.Path(process.patDefaultSequence*process.dijetEventSelectionSequence)
