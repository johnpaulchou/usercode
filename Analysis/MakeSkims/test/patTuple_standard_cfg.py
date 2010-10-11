## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## this is the dataset we're going to run on
#import Analysis.MakeSkims.datasets.dataset_QCDPt30to50Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt50to80Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt80to120Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt120to170Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt170to300Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt300to470Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt470to600Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt600to800Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt800to1000Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1000to1400Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1400to1800Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_QCDPt1800Pythia8Fall10_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetMETTauSep17thReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetMETSep17thReReco_cfi as dataset
import Analysis.MakeSkims.datasets.dataset_JetRun2010BPromptRecoV2_cfi as dataset

from Analysis.MakeSkims.datasets.datasetTools import *
linkDatasetIntoPatProcess(process, dataset)

## overwrite default PAT selection with my own selection
## this includes adding PFJets/MET, cleaning, etc.
from Analysis.MakeSkims.mySelectorSequence_cfi import *
ReplaceSequenceWithMySelectors(process)

## this is the event selection we're using
from Analysis.MakeSkims.dijetEventSelection_cfi import *
#from Analysis.MakeSkims.dileptonEventSelection_cfi import *
setupEventSelection(process)

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents.input = 1000
