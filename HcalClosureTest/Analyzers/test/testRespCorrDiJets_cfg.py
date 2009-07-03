import FWCore.ParameterSet.Config as cms

process = cms.Process('MyRec')

process.load('FWCore.MessageService.MessageLogger_cfi')

#load the response corrections calculator
process.load('HcalClosureTest.Analyzers.calcrespcorrdijets_cfi')

process.calcrespcorrdijets.rootHistFilename = cms.string('plots.root')
# all towers (particle gun, and |eta|<2.0)
process.calcrespcorrdijets.maxCalibratedIEta = cms.int32(23)
process.calcrespcorrdijets.maxDeltaEta = cms.double(0.5)
process.calcrespcorrdijets.minJetEt = cms.double(120)
process.calcrespcorrdijets.maxModifiedEMF = cms.double(0.8)
process.calcrespcorrdijets.respCorr = cms.vdouble(3.06014, 2.56641, 2.31755, 2.08264, 1.96195, 1.89597, 1.94803, 1.87687, 1.83382, 1.89167, 1.85824, 1.9247, 1.96712, 1.74818, 1.43026, 1.3049, 1.28855, 1.25, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.23098, 1.3175, 1.30595, 1.35515, 1.53153, 1.75728, 1.78285, 1.71609, 1.74652, 1.82479, 1.77465, 1.80768, 1.9039, 2.01467, 2.10768, 2.25695, 2.38147, 2.55 )
#process.calcrespcorrdijets.respCorr = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)
for i in range (1, 60):
    if i==28:
        continue
    readFiles.append('dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/QCDpt80/QCDpt80_'+str(i)+'.root')
print readFiles

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(1000)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# timing
#process.Timing = cms.Service('Timing')

process.p = cms.Path(process.calcrespcorrdijets)
