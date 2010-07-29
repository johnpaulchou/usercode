import FWCore.ParameterSet.Config as cms

from Analysis.MakeSkims.myMuonSelector_cfi import *
from Analysis.MakeSkims.myElectronSelector_cfi import *
from Analysis.MakeSkims.myVertexSelector_cfi import *

def ReplaceSequenceWithMySelectors(process):
    # print warning message
    print "Replacing default PAT selectors with my own.  Warning: This function is not particularly smart and assumes some things about the sequence.  Use with caution."
    print "================================================="

    # do some replacing
    process.selectedPatMuons = mySelectedPatMuons
    process.selectedPatElectrons = mySelectedPatElectrons

    # essentially renaming the filter
    process.selectedVertices = mySelectedVertices

    # add vertex selection to sequence
    # assumes that we've kept patElectrons, and that it is still at the beginning
    process.patDefaultSequence.replace(process.patElectrons,process.selectedVertices+process.patElectrons)

    # make sure the cleaner picks up the right collections, now that there are multiple selected objects
    process.cleanPatMuons.src = cms.InputTag('selectedPatMuons','isolated')
    process.cleanPatElectrons.src = cms.InputTag('selectedPatElectrons','isolated')

    # updated output
    process.out.outputCommands += ['keep *_selectedPatPhotons_*_*',
                                   'keep *_selectedPatElectrons_*_*',
                                   'keep *_selectedPatMuons_*_*',
                                   'keep *_selectedPatTaus_*_*',
                                   'keep *_selectedPatJets_*_*',
                                   'keep *_selectedVertices_*_*']
