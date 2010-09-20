import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.metTools import *

from Analysis.MakeSkims.myMuonSelector_cfi import *
from Analysis.MakeSkims.myElectronSelector_cfi import *
from Analysis.MakeSkims.myPhotonSelector_cfi import *
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
    process.selectedPatPhotons = mySelectedPatPhotons
    process.selectedPatJets.cut = 'pt()>10.0'
    process.selectedPatJetsAK5PF.cut = 'pt()>10.0'

    # essentially renaming the filter
    process.selectedVertices = mySelectedVertices

    # remove cleaning
    removeCleaning(process)

    # add vertex selection to sequence
    # assumes that we've kept patElectrons, and that it is still at the beginning
    process.patDefaultSequence.replace(process.patElectrons,process.selectedVertices+process.patElectrons)

    # updated output
    process.out.outputCommands += ['keep *_selectedVertices_*_*',
                                   'keep *_generalTracks_*_*',
                                   'keep *_towerMaker_*_*']
