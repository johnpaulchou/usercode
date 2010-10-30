import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import *

from Analysis.MakeSkims.myMuonSelector_cfi import *
from Analysis.MakeSkims.myElectronSelector_cfi import *
from Analysis.MakeSkims.myPhotonSelector_cfi import *

def ReplaceSequenceWithMySelectors(process):
    # print warning message
    print "Replacing default PAT selectors with my own."
    print "================================================="

    # do some replacing
    process.selectedPatMuons = mySelectedPatMuons
    process.selectedPatElectrons = mySelectedPatElectrons
    process.selectedPatPhotons = mySelectedPatPhotons
    process.selectedPatJets.cut = 'pt()>30.0'

    # remove cleaning
    removeCleaning(process)

    # updated output
    process.out.outputCommands += ['keep *_offlinePrimaryVertices_*_*']
