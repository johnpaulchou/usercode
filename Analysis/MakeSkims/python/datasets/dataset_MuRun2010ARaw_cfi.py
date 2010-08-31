import FWCore.ParameterSet.Config as cms

## production release = none

dataset = "/Mu/Run2010A-v1/RAW"

## global tag
globaltag = cms.string('GR_R_38X_V9::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/144/089/049F5E2B-43B3-DF11-9CAC-001D09F2924F.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/144/089/08B9F46A-22B3-DF11-9D41-0030487A1FEC.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/144/089/0C20821C-32B3-DF11-A09A-0030487C7392.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/144/089/0E312193-18B3-DF11-BFC7-0030487CD6B4.root')

## out files
outfile = cms.untracked.string('MuRun2010ARaw.root')

## good run list
from Analysis.MakeSkims.datasets.json_Run2010APromptRecoV4_cfi import *

## do other stuff
def doOtherStuff(process):
    pass


