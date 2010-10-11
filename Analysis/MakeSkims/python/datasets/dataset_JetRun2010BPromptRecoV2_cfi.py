import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_4_patch3

dataset = '/Jet/Run2010B-PromptReco-v2/RECO'

## global tag
globaltag = cms.string('GR_R_38X_V13A::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/146/644/003498F1-3CC9-DF11-B3A2-001D09F25208.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/146/644/06845ADE-4BC9-DF11-80D2-001D09F276CF.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/146/644/06DC1CA1-50C9-DF11-8DF3-001D09F24F1F.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/146/644/0A86F061-6DC9-DF11-B884-0030487C778E.root')

## out files
outfile = cms.untracked.string('JetRun2010BPromptRecoV2.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Run2010BPromptRecoV2.JSON'

## do other stuff
def doOtherStuff(process):
    pass
