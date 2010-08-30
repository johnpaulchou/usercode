import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_7_0_patch3

dataset = "/ppMuX/Summer10-START37_V5_S09-v1/GEN-SIM-RAW"

## global tag
globaltag = cms.string('START37_V5::All')

## is MC
isMC = True

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/mc/Summer10/ppMuX/GEN-SIM-RAW/START37_V5_S09-v1/0000/000ADD84-FF85-DF11-ACFD-0026189438C1.root')
readFiles.append('/store/mc/Summer10/ppMuX/GEN-SIM-RAW/START37_V5_S09-v1/0000/000D74A4-F585-DF11-96B5-0026189438DC.root')
readFiles.append('/store/mc/Summer10/ppMuX/GEN-SIM-RAW/START37_V5_S09-v1/0000/004F801F-F085-DF11-9B07-0018F3D0962E.root')
readFiles.append('/store/mc/Summer10/ppMuX/GEN-SIM-RAW/START37_V5_S09-v1/0000/0065DEF2-F885-DF11-8376-001A92971B12.root')

## out files
outfile = cms.untracked.string('ppMuXSTART37V5Summer10RAW.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
def doOtherStuff(process):
    pass
