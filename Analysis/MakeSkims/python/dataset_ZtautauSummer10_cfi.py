import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_7_0_patch3

## dataset = /Ztautau/Summer10-START37_V5_S09-v1/GEN-SIM-RECO

## global tag
globaltag = cms.string('START37_V5::All')

## is MC
isMC = True

## read files (no files are found locally)
readFiles = cms.untracked.vstring()

## out files
outfile = cms.untracked.string('ZtautauSummer10.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()
