import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_4_patch2
dataset = '/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO'

## global tag
globaltag = cms.string('START38_V12::All')

## is MC
isMC = True

## read files
readFiles = cms.untracked.vstring()

## out files
outfile = cms.untracked.string('QCDPt80to120Pythia6Fall10.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
def doOtherStuff(process):
    pass
