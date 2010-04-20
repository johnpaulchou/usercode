# Auto generated configuration file
# using: 
# Revision: 1.151 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: myreco -s RAW2DIGI,RECO --data --filein file:myfile.root --eventcontent RECO --datatier RECO --conditions CRAFT09_R2_V2::All -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
process.load('Configuration/StandardSequences/L1Reco_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# special running for hcalnoise filter
process.hcalnoise.writeAllRBXs = cms.bool(True)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('myreco nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

# Tone down the logging messages, MessageLogger!
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

# Input source
readFiles = cms.untracked.vstring()
import FWCore.Utilities.FileUtils as FileUtils
readFiles = cms.untracked.vstring( FileUtils.loadListFromFile ('filelists/R112237_raw.list') )
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

# Output definition
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep CaloTowersSorted_*_*_*',
                                           'keep recoHcalNoiseRBXs_*_*_*',
                                           'keep HcalNoiseSummary_*_*_*',
                                           'keep recoCaloMETs_*_*_*', 
                                           'keep recoCaloJets_*_*_*',
                                           'keep recoTracks_generalTracks_*_*',
                                           'keep L1GlobalTrigger*_*_*_*'
                                           ),
    fileName = cms.untracked.string('out.root')
    )


# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'CRAFT09_R_V10::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.out_step)
