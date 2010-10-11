import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_3

dataset = '/JetMET/Run2010A-Sep17ReReco_v2/RECO'

## global tag
globaltag = cms.string('GR_R_38X_V13A::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/JetMET/RECO/Sep17ReReco_v2/0024/00A4FB8B-17C7-DF11-9692-00261894385A.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Sep17ReReco_v2/0024/023C1F31-14C7-DF11-BEDD-00261894384A.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Sep17ReReco_v2/0024/063F7439-19C7-DF11-8166-001A928116E0.root')
readFiles.append('/store/data/Run2010A/JetMET/RECO/Sep17ReReco_v2/0024/0650C931-15C7-DF11-BC2B-0018F3D09630.root')

## out files
outfile = cms.untracked.string('JetMETSep17thReReco.root')

## JSON file
JSON = 'Analysis/MakeSkims/python/datasets/json_Sep17thReReco.JSON'

## do other stuff
def doOtherStuff(process):
        pass
