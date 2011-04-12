import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_6_1_patch2

dbs_instance = "cms_dbs_ph_analysis_02"
dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet"
dataset = "/TTbarJets-madgraph/dgele-Reference_TTsampleSpring10-4415580b7493c934abe6bb8c367d31f1/USER"

## is MC
isMC = True

## read files (no files are found locally)
readFiles = cms.untracked.vstring()

## out files
outfile = cms.untracked.string('DileptonTopChallenge.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
from PhysicsTools.PatAlgos.tools.coreTools import *
def doOtherStuff(process):
    removeMCMatching(process) # turn this off, anyway, since the genjets are missing
    removeSpecificPATObjects(process, ['Jets'])
    process.patDefaultSequence.remove(process.cleanPatJets)
