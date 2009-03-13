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

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.77.2.1 $'),
    annotation = cms.untracked.string('PGunSource'),
    name = cms.untracked.string('PyReleaseValidation')
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
    )

# Input source
process.source = cms.Source(
    "FlatRandomEGunSource",
    PGunParameters = cms.untracked.PSet(
    PartID = cms.untracked.vint32(211),
    MaxEta = cms.untracked.double(5.0),
    MaxPhi = cms.untracked.double(3.14159265359),
    MinEta = cms.untracked.double(-5.0),
    MinE = cms.untracked.double(49.99),
    MinPhi = cms.untracked.double(-3.14159265359),
    MaxE = cms.untracked.double(50.01)
    ),
    Verbosity = cms.untracked.int32(1),
    psethack = cms.string('single pi E 50 HCAL'),
    AddAntiParticle = cms.untracked.bool(False),
    firstRun = cms.untracked.uint32(1)
    )

# Output definition
process.output = cms.OutputModule(
    "PoolOutputModule",
#    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep recoCaloJets_iterativeCone5CaloJets_*_*',
                                           'keep CaloTowersSorted_towerMaker_*_*',
                                           'keep recoMET*_*_*_*',
                                           'keep Pgen*_*_*_*',
                                           'keep singleparticlecluster_*_*_*'),
    fileName = cms.untracked.string('PGunSource.root'),
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
process.reconstruction_step = cms.Path(process.reconstruction)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.out_step)


process.MessageLogger.cerr.FwkReport.reportEvery = 100

