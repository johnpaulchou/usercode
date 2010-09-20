import FWCore.ParameterSet.Config as cms

import os;

def setupEventSelection(process):
    tag = 'AnySelection'

    # rename output file
    oldname = os.path.splitext(process.out.fileName.value())[0]
    process.out.fileName = cms.untracked.string(oldname+tag+'.root')

    process.p = cms.Path(process.patDefaultSequence)
