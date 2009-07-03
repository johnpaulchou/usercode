#!/bin/tcsh -f

# these are the basic parameters
set PDGID = 211
set ENERGY = 50
set BASE = Single${PDGID}E${ENERGY}
set TEST = false
set MAXEVENTS = 100000
set EVENTS_PER_JOB = 5000

# set the location we're going to store the output
# this is appended to the pnfs or ~/nobackup directory
set STORAGEDIRNAME = DiJetCalibration/${BASE}

# the python filename we're about to create
set PYTHONNAME = "${BASE}.py"

# the name of the file that cmsRun will output
set OUTPUTNAME = "${BASE}.root"

# setup cms production
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod

cmsenv

############################################################################

# create the python file here
cat > ${PYTHONNAME} <<+EOF

import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import basic services
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')

# various conditions
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/StandardSequences/VtxSmearedEarly10TeVCollision_cff')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')

# generator
process.load('Configuration/StandardSequences/Generator_cff')

# standard Simulation
process.load('Configuration/StandardSequences/Sim_cff')

# L1
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_Unprescaled_cff')

# reconstruction
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/EventContent/EventContent_cff')

# my clustering
process.load('HcalClosureTest/Producers/singleparticleclusterproducer_cfi')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('\$Revision: 1.1 $'),
    annotation = cms.untracked.string('PGunSource'),
    name = cms.untracked.string('PyReleaseValidation')
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(${MAXEVENTS})
    )
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
    )

# Input source
process.source = cms.Source(
    "FlatRandomEGunSource",
    PGunParameters = cms.untracked.PSet(
    PartID = cms.untracked.vint32(${PDGID}),
    MaxEta = cms.untracked.double(5.0),
    MaxPhi = cms.untracked.double(3.14159265359),
    MinEta = cms.untracked.double(-5.0),
    MinE = cms.untracked.double(${ENERGY}-0.001),
    MinPhi = cms.untracked.double(-3.14159265359),
    MaxE = cms.untracked.double(${ENERGY}+0.001)
    ),
    Verbosity = cms.untracked.int32(1),
    psethack = cms.string('single pi E 50 HCAL'),
    AddAntiParticle = cms.untracked.bool(False),
    firstRun = cms.untracked.uint32(1)
    )

# Output definition
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep recoCaloJets_iterativeCone5CaloJets_*_*',
                                           'keep CaloTowersSorted_towerMaker_*_*',
                                           'keep recoGenParticles_*_*_*',
                                           'keep SingleParticleClusters_*_*_*'),
    fileName = cms.untracked.string('${OUTPUTNAME}'),
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string(''),
    filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('generation_step')
    )
    )


# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction*process.ParticleClustering)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.out_step)


process.MessageLogger.cerr.FwkReport.reportEvery = 100


+EOF

############################################################################

echo "=========================================================================="
echo "Using the following python script:"
cat ${PYTHONNAME}
echo "=========================================================================="
# finished creating the python script



# run it either locally or with crab
if($TEST == false) then
    crabsubmit.tcsh ${PYTHONNAME} ${MAXEVENTS} ${EVENTS_PER_JOB} None ${STORAGEDIRNAME}
else
    cmsRun ${PYTHONNAME}
endif

# remove the temporary script
rm ${PYTHONNAME}
