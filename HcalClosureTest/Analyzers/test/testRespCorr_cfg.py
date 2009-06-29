import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

process.load('FWCore.MessageService.MessageLogger_cfi')

#load the response corrections calculator
process.load('HcalClosureTest.Analyzers.calcrespcorr_cfi')

process.calcrespcorr.rootHistFilename = cms.string('plots.root')
# max tower (particle gun)
#process.calcrespcorr.respCorr = cms.vdouble(2.925, 2.61176, 2.48571, 2.3541, 1.95956, 1.87222, 2.07798, 2.05658, 1.78958, 2.1125, 1.85441, 2.05, 2.25053, 1.48722, 1.393, 1.32155, 1.28167, 1.22845, 1.26111, 1.29444, 1.23333, 1.25417, 1.30714, 1.21591, 1.415, 1.29167, 1.30833, 1.125, 1.35278, 1.325, 1.09583, 1.10938, 1.18125, 1.195, 1.1175, 1.18333, 1.15, 1.09318, 1.20714, 1.12206, 1.11818, 0, 1.12885, 1.16848, 1.10625, 1.14643, 1.29022, 1.1075, 1.09167, 1.12917, 1.21346, 1.19318, 1.27955, 1.13056, 1.14167, 1.25625, 1.425, 1.21346, 1.225, 1.37857, 1.24063, 1.25227, 1.21618, 1.21389, 1.1125, 1.28409, 1.30776, 1.31154, 1.36, 1.556, 2.02143, 1.825, 1.84474, 1.74729, 1.94574, 1.89022, 1.96416, 1.90329, 2.27672, 2.21987, 2.44864, 2.41316, 2.535 )
# all towers (particle gun)
#process.calcrespcorr.respCorr = cms.vdouble(3.06014, 2.56641, 2.31755, 2.08264, 1.96195, 1.89597, 1.94803, 1.87687, 1.83382, 1.89167, 1.85824, 1.9247, 1.96712, 1.74818, 1.43026, 1.3049, 1.28855, 1.25, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.23098, 1.3175, 1.30595, 1.35515, 1.53153, 1.75728, 1.78285, 1.71609, 1.74652, 1.82479, 1.77465, 1.80768, 1.9039, 2.01467, 2.10768, 2.25695, 2.38147, 2.55 )
# all towers (particle gun, and |eta|<2.0)
process.calcrespcorr.respCorr = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )


# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",
                             fileNames = readFiles,
                             duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                             )

readFiles.extend( [
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_1.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_2.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_3.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_4.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_5.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_6.root',
    'dcap://pnfs/cms/WAX/resilient/johnpaul/DiJetCalibration/Single211E50_HcalRespCorrs200mc/Single211E50_HcalRespCorrs200mc_7.root'
    ] );

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(1000)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# timing
#process.Timing = cms.Service('Timing')

process.p = cms.Path(process.calcrespcorr)
