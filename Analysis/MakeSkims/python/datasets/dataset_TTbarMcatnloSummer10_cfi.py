import FWCore.ParameterSet.Config as cms

## production release = CMSSW_3_6_2

dataset = "/TTbar-mcatnlo/Summer10-START37_V5_S09-v1/GEN-SIM-RECO"

## is MC
isMC = True

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/mc/Summer10/TTbar-mcatnlo/GEN-SIM-RECO/START37_V5_S09-v1/0000/0268EFE9-DC85-DF11-ABC0-002618943902.root')
readFiles.append('/store/mc/Summer10/TTbar-mcatnlo/GEN-SIM-RECO/START37_V5_S09-v1/0000/02C33E82-EA85-DF11-A055-0026189437FE.root')
readFiles.append('/store/mc/Summer10/TTbar-mcatnlo/GEN-SIM-RECO/START37_V5_S09-v1/0000/02CCE997-E985-DF11-971D-00261894389E.root')
readFiles.append('/store/mc/Summer10/TTbar-mcatnlo/GEN-SIM-RECO/START37_V5_S09-v1/0000/0425AE78-E985-DF11-9510-003048678A80.root')
readFiles.append('/store/mc/Summer10/TTbar-mcatnlo/GEN-SIM-RECO/START37_V5_S09-v1/0000/06226586-DA85-DF11-8F5B-0026189438B5.root')

## out files
outfile = cms.untracked.string('TTbarMcatnloSummer10.root')

## good run list
lumisToProcess = cms.untracked.VLuminosityBlockRange()

## do other stuff
def doOtherStuff(process):
    pass

