import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_6_2

dataset = "/Ztautau_M20_CTEQ66-powheg/Summer10-START36_V9_S09-v1/GEN-SIM-RECO"

## global tag
globaltag = cms.string('START36_V9::All')

## is MC
isMC = True

## read files
readFiles = cms.untracked.vstring('/store/mc/Summer10/Ztautau_M20_CTEQ66-powheg/GEN-SIM-RECO/START36_V9_S09-v1/0026/02579C1B-E276-DF11-949B-00261894380D.root',
                                  '/store/mc/Summer10/Ztautau_M20_CTEQ66-powheg/GEN-SIM-RECO/START36_V9_S09-v1/0026/064E0F28-E576-DF11-8DA1-002618943910.root',
                                  '/store/mc/Summer10/Ztautau_M20_CTEQ66-powheg/GEN-SIM-RECO/START36_V9_S09-v1/0026/0C8B783B-E976-DF11-8DD3-003048678FDE.root',
                                  '/store/mc/Summer10/Ztautau_M20_CTEQ66-powheg/GEN-SIM-RECO/START36_V9_S09-v1/0026/16F267BB-E576-DF11-9AE4-002618943920.root')

## out files
outfile = cms.untracked.string('ZtautauM20CTEQ66Summer10.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
def doOtherStuff(process):
    process.patTrigger.processName = "REDIGI36X"
    process.patTriggerEvent.processName = "REDIGI36X"
