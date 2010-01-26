import FWCore.ParameterSet.Config as cms

process = cms.Process('ANALYSIS')

process.load('FWCore.MessageService.MessageLogger_cfi')

#load the response corrections calculator
process.load('HcalClosureTest.Analyzers.calcrespcorrdijets_cfi')
process.load('JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff')

# run over files

import FWCore.Python.FileUtils as FileUtils
readFiles = cms.untracked.vstring( FileUtils.loadListFromFile ('QCD_Pt30.list') )
process.source = cms.Source ("PoolSource",fileNames = readFiles)
#print readFiles

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(1000)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# timing
#process.Timing = cms.Service('Timing')

process.p = cms.Path(process.calcrespcorrdijets)
