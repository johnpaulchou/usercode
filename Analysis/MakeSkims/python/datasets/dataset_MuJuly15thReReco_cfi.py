import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_7_0_patch4

## dataset = /Mu/Run2010A-Jul15thReReco-v1/RECO

## global tag
globaltag = cms.string('GR_R_37X_V6D::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/Mu/RECO/Jul15thReReco-v1/0047/0017F83A-A390-DF11-8260-002618943922.root')
readFiles.append('/store/data/Run2010A/Mu/RECO/Jul15thReReco-v1/0047/005EBF62-8690-DF11-BF3A-00261894393C.root')
readFiles.append('/store/data/Run2010A/Mu/RECO/Jul15thReReco-v1/0047/008405A5-9B90-DF11-92A1-0026189438B8.root')
readFiles.append('/store/data/Run2010A/Mu/RECO/Jul15thReReco-v1/0047/0232A353-6390-DF11-AE4B-002618FDA204.root')

# out files
outfile = cms.untracked.string('MuJuly15thReReco.root')

# good run list
from Analysis.MakeSkims.datasets.json_July15thReReco_cfi import *
