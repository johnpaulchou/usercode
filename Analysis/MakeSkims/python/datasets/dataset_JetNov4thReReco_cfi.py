import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_6

dataset = '/Jet/Run2010B-Nov4ReReco_v1/RECO'

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010B/Jet/RECO/Nov4ReReco_v1/0034/08E28E13-CFEA-DF11-914E-003048D43788.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/Nov4ReReco_v1/0034/0E77CA46-E3EA-DF11-8BC0-003048F02CB4.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/Nov4ReReco_v1/0034/10311899-C8EA-DF11-BC37-002481E0DC7C.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/Nov4ReReco_v1/0034/3AD31901-E3EA-DF11-8815-0030487F1717.root')
readFiles.append('/store/data/Run2010B/Jet/RECO/Nov4ReReco_v1/0034/3AE86666-BCEA-DF11-B393-003048D439AC.root')

## out files
outfile = cms.untracked.string('JetNov4thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Nov4thReReco.JSON'

## do other stuff
def doOtherStuff(process):
    pass
