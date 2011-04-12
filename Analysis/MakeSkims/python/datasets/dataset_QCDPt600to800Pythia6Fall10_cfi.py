import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_8_4_patch2

dataset = '/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO'

## is MC
isMC = True

## read files
readFiles = cms.untracked.vstring()

## out files
outfile = cms.untracked.string('QCDPt600to800Pythia6Fall10.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
def doOtherStuff(process):
    pass
