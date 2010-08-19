import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_6_1_patch2

dbs_instance = "cms_dbs_ph_analysis_02"
dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet"
dataset = "/TTbarJets-madgraph/dgele-Reference_TTsampleSpring10-4415580b7493c934abe6bb8c367d31f1/USER"

## global tag
globaltag = cms.string('START36_V10::All')

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
    process.patTrigger.processName = "REDIGI"
    process.patTriggerEvent.processName = "REDIGI"
    removeSpecificPATObjects(process, ['Jets'])
    process.countPatElectrons.src = cms.InputTag('selectedPatElectrons','isolated')
    process.countPatMuons.src = cms.InputTag('selectedPatMuons','isolated')
    process.countPatLeptons.electronSource = cms.InputTag('selectedPatElectrons','isolated')
    process.countPatLeptons.muonSource = cms.InputTag('selectedPatMuons','isolated')
    process.electronTriggerMatchHLTEle15LWL1R.src = cms.InputTag('selectedPatElectrons','isolated')
    process.electronTriggerMatchHLTDoubleEle5SWL1R.src = cms.InputTag('selectedPatElectrons','isolated')
    process.muonTriggerMatchL1Muon.src = cms.InputTag('selectedPatMuons','isolated')
    process.muonTriggerMatchHLTMu3.src = cms.InputTag('selectedPatMuons','isolated')
    process.muonTriggerMatchHLTIsoMu3.src = cms.InputTag('selectedPatMuons','isolated')
    process.muonTriggerMatchHLTDoubleMu3.src = cms.InputTag('selectedPatMuons','isolated')
    process.tauTriggerMatchHLTDoubleLooseIsoTau15.src = cms.InputTag('selectedPatTaus')
