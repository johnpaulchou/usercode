import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.metTools import *

from Analysis.MakeSkims.myMuonSelector_cfi import *
from Analysis.MakeSkims.myElectronSelector_cfi import *
from Analysis.MakeSkims.myVertexSelector_cfi import *

def ReplaceSequenceWithMySelectors(process):
    # print warning message
    print "Replacing default PAT selectors with my own.  Warning: This function is not particularly smart and assumes some things about the sequence.  Use with caution."
    print "================================================="

    ## add PF jets
    addJetCollection(process,cms.InputTag('ak5PFJets'),
                     'AK5', 'PF',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5', 'PF'),
                     doType1MET   = False, ## there is no use to apply residual jet corrections on CaloMET
                     doL1Cleaning = True,
                     doL1Counters = False,
                     genJetCollection=cms.InputTag("ak5GenJets"),
                     doJetID      = False  ## there is no use of jetID for Pflow jets
                     )

    ## add JPT jets
    addJetCollection(process,cms.InputTag('ak5TrackJets'),
                     'AK5', 'JPT',
                     doJTA        = True,
                     doBTagging   = True,
                     jetCorrLabel = ('AK5', 'JPT'),
                     doType1MET   = False,
                     doL1Cleaning = True,
                     doL1Counters = False,
                     genJetCollection=cms.InputTag('ak5GenJets'),
                     doJetID      = True
                     )

    ## add extra MET categories
    addPfMET(process, 'PF')
    addTcMET(process, 'JPT')

    # do some replacing
    process.selectedPatMuons = mySelectedPatMuons
    process.selectedPatElectrons = mySelectedPatElectrons
    process.selectedPatJets.cut = 'pt()>10.0'
    process.selectedPatJetsAK5PF.cut = 'pt()>10.0'

    # essentially renaming the filter
    process.selectedVertices = mySelectedVertices

    # add vertex selection to sequence
    # assumes that we've kept patElectrons, and that it is still at the beginning
    process.patDefaultSequence.replace(process.patElectrons,process.selectedVertices+process.patElectrons)

    # setup the cleaning
    process.cleanPatMuons.src = cms.InputTag('selectedPatMuons','isolated')

    process.cleanPatElectrons.src = cms.InputTag('selectedPatElectrons','isolated')
    process.cleanPatElectrons.checkOverlaps = cms.PSet()

    process.cleanPatPhotons.src = cms.InputTag('selectedPatPhotons')
    process.cleanPatPhotons.checkOverlaps = cms.PSet()

    process.cleanPatTaus.src = cms.InputTag('selectedPatTaus')
    process.cleanPatTaus.checkOverlaps = cms.PSet()
    
    process.cleanPatJets.checkOverlaps = cms.PSet(
        electrons = cms.PSet(
        src = cms.InputTag('cleanPatElectrons'),
        deltaR = cms.double(0.4),
        pairCut = cms.string(''),
        checkRecoComponents = cms.bool(False),
        algorithm = cms.string('byDeltaR'),
        preselection = cms.string(''),
        requireNoOverlaps = cms.bool(False)
        ),
        muons = cms.PSet(
        src = cms.InputTag('cleanPatMuons'),
        deltaR = cms.double(0.4),
        pairCut = cms.string(''),
        checkRecoComponents = cms.bool(False),
        algorithm = cms.string('byDeltaR'),
        preselection = cms.string(''),
        requireNoOverlaps = cms.bool(False)
        ),
        )
    process.cleanPatJetsAK5PF.checkOverlaps = process.cleanPatJets.checkOverlaps
    process.cleanPatJetsAK5JPT.checkOverlaps = process.cleanPatJets.checkOverlaps

    # updated output
    process.out.outputCommands += ['keep *_selectedPatPhotons_*_*',
                                   'keep *_selectedPatElectrons_*_*',
                                   'keep *_selectedPatMuons_*_*',
                                   'keep *_selectedPatTaus_*_*',
                                   'keep *_selectedPatJets*_*_*',
                                   'keep *_selectedVertices_*_*']
