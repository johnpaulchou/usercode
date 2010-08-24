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
    addCleaning(process)
    process.patDefaultSequence.remove(process.cleanPatJets)
    
#    finalPatElectronsSrc=cms.InputTag('selectedPatElectrons','isolated')
#    finalPatMuonsSrc=cms.InputTag('selectedPatMuons','isolated')
#    finalPatTausSrc=cms.InputTag('selectedPatTaus')
#    process.countPatElectrons.src = finalPatElectronsSrc
#    process.countPatMuons.src = finalPatMuonsSrc
#    process.countPatLeptons.electronSource = finalPatElectronsSrc
#    process.countPatLeptons.muonSource = finalPatMuonsSrc
#    process.electronTriggerMatchHLTEle15LWL1R.src = finalPatElectronsSrc
#    process.electronTriggerMatchHLTDoubleEle5SWL1R.src = finalPatElectronsSrc
#    process.muonTriggerMatchL1Muon.src = finalPatMuonsSrc
#    process.muonTriggerMatchHLTMu3.src = finalPatMuonsSrc
#    process.muonTriggerMatchHLTIsoMu3.src = finalPatMuonsSrc
#    process.muonTriggerMatchHLTDoubleMu3.src = finalPatMuonsSrc
#    process.tauTriggerMatchHLTDoubleLooseIsoTau15.src = finalPatTausSrc

