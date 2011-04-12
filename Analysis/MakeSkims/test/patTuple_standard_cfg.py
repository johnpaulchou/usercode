## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## this is the dataset we're going to run on
#import Analysis.MakeSkims.datasets.dataset_QCDPt80to120Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt120to170Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt170to300Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt300to470Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt470to600Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt600to800Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt800to1000Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1000to1400Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1400to1800Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1800Pythia6Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetMETTauNov4thReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetMETNov4thReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetNov4thReReco_cfi as dataset
import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ500_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ750_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ1000_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ1250_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ1500_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZPrimeSSMToJJ1750_cfi as dataset

from Analysis.MakeSkims.datasets.datasetTools import *
linkDatasetIntoPatProcess(process, dataset)

## set the global tag
if dataset.isMC:
    process.GlobalTag.globaltag = "START38_V14::All"
else:
    process.GlobalTag.globaltag = "GR_R_38X_V15::All"

## overwrite default PAT selection with my own selection
from Analysis.MakeSkims.myMuonSelector_cfi import *
from Analysis.MakeSkims.myElectronSelector_cfi import *
from Analysis.MakeSkims.myPhotonSelector_cfi import *
process.selectedPatMuons = mySelectedPatMuons
process.selectedPatElectrons = mySelectedPatElectrons
process.selectedPatPhotons = mySelectedPatPhotons

## this is the event selection we're using
from Analysis.MakeSkims.dijetEventSelection_cfi import *
setupEventSelection(process, dataset)

## updated output
process.out.outputCommands += ['keep *_offlinePrimaryVertices_*_*']

## remove cleaning
from PhysicsTools.PatAlgos.tools.coreTools import *
removeCleaning(process)
removeSpecificPATObjects(process, ['Taus'])

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 1000

