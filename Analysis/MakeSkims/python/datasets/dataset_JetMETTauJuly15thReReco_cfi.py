import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_7_0_patch4

## dataset = /JetMETTau/Run2010A-Jul15thReReco-v1/RECO

## global tag
globaltag = cms.string('GR_R_37X_V6D::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Jul15thReReco-v1/0047/00D0760C-9A90-DF11-9675-002618FDA259.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Jul15thReReco-v1/0047/02BD4F2C-A390-DF11-8685-002618FDA248.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Jul15thReReco-v1/0047/02D974BC-A990-DF11-A3AD-001A92971B78.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Jul15thReReco-v1/0047/04003360-9290-DF11-A3F6-002618943921.root')

## out files
outfile = cms.untracked.string('JetMETTauJuly15thReReco.root')

## good run list
from Analysis.MakeSkims.datasets.json_July15thReReco_cfi import *

