#!/bin/tcsh -f

# these are the basic parameters
set RUNNUMBER = 68288
set BASE = CaloCommissioning08_R${RUNNUMBER}
set DATASETPATH = /Calo/Commissioning08-v1/RAW
set TEST = false
set EVENTS_PER_JOB = 50000

# set the # of events to run over
if($TEST == true) then
    set MAXEVENTS = 1000
else
    set MAXEVENTS = -1
endif

# set the location we're going to store the output
# this is appended to the pnfs or ~/nobackup directory
set STORAGEDIRNAME = AnomalousNoise/${BASE}

# the python filename we're about to create
set PYTHONNAME = "${BASE}.py"

# the name of the file that cmsRun will output
set OUTPUTNAME = "${BASE}.root"

# setup cms production on cmslpc
#unsetenv CMS_PATH
#source /uscmst1/prod/sw/cms/cshrc prod
#cmsenv

############################################################################

# create the python file here
cat > ${PYTHONNAME} <<+EOF

import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

# Conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
process.prefer("GlobalTag")

# Magnetic field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

#raw-to-digi
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.L1GlobalTriggerRawToDigi.l1GtRecord_cfi')
process.RawToDigi = cms.Sequence(process.gctDigis+process.gtDigis+process.gtEvmDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis)

#reco for cosmics
process.load('RecoLocalCalo.Configuration.RecoLocalCalo_Cosmics_cff')
process.load('RecoJets.Configuration.RecoCaloTowersGR_cff')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.reconstructionCosmics = cms.Sequence(process.l1GtRecord*process.calolocalreco*process.recoCaloTowersGR*process.hcalnoise)

process.hcalnoise.dropRefVectors = cms.bool(False)
process.hcalnoise.minRBXEnergy = cms.double(20)
process.hcalnoise.minHPDEnergy = cms.double(20)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.fillJets = cms.bool(False)

# run over files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FE92951A-3CA8-DD11-8981-001D09F23A4D.root')
readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FCEA0CB3-3FA8-DD11-82E6-001D09F28D4A.root')
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(${MAXEVENTS}), run = cms.untracked.int32(${RUNNUMBER}) )

# Tone down the logging messages, MessageLogger!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# output
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
#                                           'keep HBHEDataFramesSorted_*_*_*',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep CaloTowersSorted_*_*_*',
                                           'keep recoHcalNoiseRBXs_*_*_*',
                                           'keep HcalNoiseSummary_*_*_*',
                                           'keep L1GlobalTriggerRecord_*_*_*'
                                           ),
    fileName = cms.untracked.string('${OUTPUTNAME}')
    )


# schedule
process.digi = cms.Path(process.RawToDigi)
process.reco = cms.Path(process.reconstructionCosmics)
process.e = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.digi,process.reco,process.e)

+EOF

############################################################################

echo "Using the following python script:"
echo "=========================================================================="
cat ${PYTHONNAME}
echo "=========================================================================="
# finished creating the python script



# run it either locally or with crab
if($TEST == false) then
    crabsubmit.tcsh ${PYTHONNAME} ${MAXEVENTS} ${EVENTS_PER_JOB} ${DATASETPATH} ${STORAGEDIRNAME} ${RUNNUMBER} ${RUNNUMBER}
else
    cmsRun ${PYTHONNAME}
endif

# remove the temporary script
rm ${PYTHONNAME}
