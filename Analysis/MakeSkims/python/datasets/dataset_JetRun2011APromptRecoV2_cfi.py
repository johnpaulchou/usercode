import FWCore.ParameterSet.Config as cms

## production release = CMSSW_4_1_3_patch2

dataset = '/Jet/Run2011A-PromptReco-v2/AOD'

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2011A/Jet/AOD/PromptReco-v2/000/162/808/1A6FDD43-8769-E011-BAA9-0030487CD17C.root')
readFiles.append('/store/data/Run2011A/Jet/AOD/PromptReco-v2/000/162/808/684C972F-9D69-E011-813A-0030487CD184.root')

## out files
outfile = cms.untracked.string('JetRun2011AV2.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Run2011APromptReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
