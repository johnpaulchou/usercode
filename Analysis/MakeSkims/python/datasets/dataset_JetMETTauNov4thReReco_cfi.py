import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_6

dataset = '/JetMETTau/Run2010A-Nov4ReReco_v1/RECO'

## global tag
globaltag = cms.string('FT_R_38X_V14A::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Nov4ReReco_v1/0151/9A9D89B4-79ED-DF11-A9BF-002618943939.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Nov4ReReco_v1/0151/9AD7194A-79ED-DF11-9E77-001A92971B3A.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Nov4ReReco_v1/0151/9C0AFAB6-79ED-DF11-ADF1-0026189438AC.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Nov4ReReco_v1/0151/9C910BBF-79ED-DF11-9049-0026189437F0.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Nov4ReReco_v1/0151/9C9FCF47-79ED-DF11-BB48-003048D25B68.root')

## out files
outfile = cms.untracked.string('JetMETTauNov4thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Nov4thReReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
