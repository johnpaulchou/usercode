import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

process.load('FWCore.MessageService.MessageLogger_cfi')

#load the response corrections calculator
process.load('HcalClosureTest.Analyzers.calcrespcorrdijets_cfi')

process.calcrespcorrdijets.rootHistFilename = cms.string('plots.root')
# all towers (particle gun, and |eta|<2.0)
process.calcrespcorrdijets.maxCalibratedIEta = cms.int32(23)
process.calcrespcorrdijets.respCorr = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)
readFiles.extend( [
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/QCDDiJetPt800to1000/QCDDiJetPt800to1000_1.root'
    ] );

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(1000)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# timing
#process.Timing = cms.Service('Timing')

process.p = cms.Path(process.calcrespcorrdijets)
