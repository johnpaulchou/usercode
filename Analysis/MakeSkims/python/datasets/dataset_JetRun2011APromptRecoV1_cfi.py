import FWCore.ParameterSet.Config as cms

## production release = CMSSW_4_1_3_patch2

dataset = '/Jet/Run2011A-PromptReco-v1/AOD'

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2011A/Jet/AOD/PromptReco-v1/000/160/431/44A96589-2D50-E011-861C-003048F118D4.root')
readFiles.append('/store/data/Run2011A/Jet/AOD/PromptReco-v1/000/160/578/92358E70-8D51-E011-9883-0030487A3232.root')

## out files
outfile = cms.untracked.string('JetRun2011AV1.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Run2011APromptReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
