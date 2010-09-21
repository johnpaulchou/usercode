import FWCore.ParameterSet.Config as cms

## production release = 370p4

dataset = "/MinimumBias/Commissioning10-Jun9thReReco_v1/RECO"

## global tag
globaltag = cms.string('GR_R_38X_V11::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Jun9thReReco_v1/0005/0ADC0720-4C74-DF11-8375-0026189438E8.root')
readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Jun9thReReco_v1/0005/0C995AD6-4C74-DF11-8DDA-003048678FE4.root')
readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Jun9thReReco_v1/0005/0E6EA4FC-4B74-DF11-A2F2-0018F34D0D62.root')
readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Jun9thReReco_v1/0005/38B087D9-4C74-DF11-930C-0018F3D096C8.root')
readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Jun9thReReco_v1/0005/48F381F1-4B74-DF11-BA07-002618943865.root')

## out files
outfile = cms.untracked.string('Commissioning10Jun9thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_June9thReReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
