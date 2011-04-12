import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_6

dataset = '/JetMET/Run2010A-Nov4ReReco_v1/RECO'

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/JetMET/RECO/Nov4ReReco_v1/0130/009046A7-CEEA-DF11-9F46-002618943915.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Nov4ReReco_v1/0130/0246D8B2-CEEA-DF11-9E37-002618943862.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Nov4ReReco_v1/0130/02BC4FCF-CEEA-DF11-9C76-002618943861.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Nov4ReReco_v1/0130/1CFF58C8-CEEA-DF11-90BF-001A92971B12.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Nov4ReReco_v1/0130/226871D2-CEEA-DF11-87AD-003048678BB8.root')

## out files
outfile = cms.untracked.string('JetMETNov4thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Nov4thReReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
