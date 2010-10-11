import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_3

dataset = '/JetMETTau/Run2010A-Sep17ReReco_v2/RECO'

## global tag
globaltag = cms.string('GR_R_38X_V13A::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Sep17ReReco_v2/0046/0E0FF527-5AC6-DF11-996D-0026189438E3.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Sep17ReReco_v2/0046/10B7A1AC-60C6-DF11-A5D9-003048679162.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Sep17ReReco_v2/0046/12540C4D-61C6-DF11-848F-00261894396D.root')
readFiles.append('/store/data/Run2010A/JetMETTau/RECO/Sep17ReReco_v2/0046/126C36F1-57C6-DF11-9B6E-002354EF3BDA.root')

## out files
outfile = cms.untracked.string('JetMETTauSep17thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Sep17thReReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
