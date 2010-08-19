import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_7_0_patch4

dataset = "/EG/Run2010A-Jul15thReReco-v1/RECO"

## global tag
globaltag = cms.string('GR_R_37X_V6D::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/EG/RECO/Jul15thReReco-v1/0000/00201E52-5390-DF11-8D90-003048D3CD92.root')
readFiles.append('/store/data/Run2010A/EG/RECO/Jul15thReReco-v1/0000/003782A8-5490-DF11-BD19-0030487F92A7.root')
readFiles.append('/store/data/Run2010A/EG/RECO/Jul15thReReco-v1/0000/008804D4-4190-DF11-A51E-003048D3C7BC.root')
readFiles.append('/store/data/Run2010A/EG/RECO/Jul15thReReco-v1/0000/00AE7C49-4290-DF11-8ACE-0030487D5EBB.root')

## out files
outfile = cms.untracked.string('EGJuly15thReReco.root')

## good run list
from Analysis.MakeSkims.datasets.json_July15thReReco_cfi import *

## do other stuff
def doOtherStuff(process):
    pass
