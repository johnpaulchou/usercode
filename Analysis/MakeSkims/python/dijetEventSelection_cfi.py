import FWCore.ParameterSet.Config as cms

import os

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *

def setupEventSelection(process, dataset):
    tag = 'DijetSelection'

    ## pick the right set of corrections
    if dataset.isMC:
        corrs=cms.vstring('L2Relative', 'L3Absolute','L5Flavor','L7Parton')
    else:
        corrs=cms.vstring('L2Relative', 'L3Absolute','L2L3Residual','L5Flavor','L7Parton')

    ## add AK7 calojets
    addJetCollection(process,cms.InputTag('ak7CaloJets'),
                     'AK7', 'Calo',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5Calo', corrs),
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
                     jetCorrLabel = ('AK5PF', corrs),
                     doType1MET   = False,
                     doL1Cleaning = False,
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak7GenJets"),
                     doJetID      = True,
                     )

    ## add AK5 calojets
    switchJetCollection(process,
                        cms.InputTag('ak5CaloJets'),
                        doJTA        = True,
                        doBTagging   = True,
                        jetCorrLabel = ('AK5Calo', corrs),
                        doType1MET   = False,
                        genJetCollection=cms.InputTag("ak5GenJets"),
                        doJetID      = True,
                        jetIdLabel   = 'ak5'
                        )

    ## add extra MET categories
    addPfMET(process, 'PF')

    ## do some basic preselection
    process.selectedPatJetsAK7Calo.cut = 'pt()>30.0'
    process.selectedPatJetsAK7PF.cut = 'pt()>30.0'
    process.selectedPatJets.cut = 'pt()>30.0'

    ## HB/HE noise filter
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

    ## dijet event selection
    process.dijetEventSelection = cms.EDFilter('DijetEventSelector',
                                               HLTSrc = cms.InputTag('TriggerResults','','HLT'),
                                               vertexSrc = cms.InputTag('offlinePrimaryVertices'),
                                               minVertexNDOF = cms.int32(5),
                                               maxVertexZ = cms.double(24.0),
                                               maxVertexRho = cms.double(2.0),
                                               doNoiseStep = cms.bool(True),
                                               noiseResultSrc = cms.InputTag("HBHENoiseFilterResultProducer","HBHENoiseFilterResult"),
                                               trackSrc = cms.InputTag('generalTracks'),
                                               minHighQualityTrackFraction = cms.double(0.25),
                                               minNumTracksForMonsterEventCut = cms.int32(11),
                                               jetSrc = cms.InputTag('selectedPatJetsAK7Calo'),
                                               leadJetMinRawPt = cms.double(10.0),
                                               leadJetMinCorrPt = cms.double(100.0),
                                               leadJetMaxAbsEta = cms.double(2.5),
                                               subLeadJetMinRawPt = cms.double(10.0),
                                               subLeadJetMinCorrPt = cms.double(100.0),
                                               subLeadJetMaxAbsEta = cms.double(2.5),
                                               minDijetMass = cms.double(200.0),
                                               maxDijetDeta = cms.double(2.0),
                                               )
    # rename output file
    oldname = os.path.splitext(process.out.fileName.value())[0]
    process.out.fileName = cms.untracked.string(oldname+tag+'.root')

    # setup file service
    process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string(oldname+tag+'Plots.root'),
                                       closeFileFast = cms.untracked.bool(True)
                                       )

    # drop large data
    process.out.outputCommands += ['drop CaloTowers_*_*_*',
                                   'drop recoPFCandidates_*_*_*',
                                   'drop recoBaseTagInfos_selectedPatJets_*_*',
                                   'drop recoBaseTagInfos_selectedPatJetsAK7PF_*_*']
    # setup sequence
    process.dijetEventSelectionSequence = cms.Sequence(process.HBHENoiseFilterResultProducer*process.dijetEventSelection)
    process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )
    process.p = cms.Path(process.patDefaultSequence*process.dijetEventSelectionSequence)
