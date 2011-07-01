## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## this is the dataset we're going to run on
#import Analysis.MakeSkims.datasets.dataset_JetMETTauDec22ndReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetMETDec22ndReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetDec22ndReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_JetRun2011APromptRecoV1_cfi as dataset
import Analysis.MakeSkims.datasets.dataset_JetRun2011APromptRecoV2_cfi as dataset

from Analysis.MakeSkims.datasets.datasetTools import *
linkDatasetIntoPatProcess(process, dataset)

## set the global tag
if dataset.isMC:
    process.GlobalTag.globaltag = "START311_V2::All"
else:
    process.GlobalTag.globaltag = "GR_R_311_V2::All"

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
#removeSpecificPATObjects(process, ['Taus'])

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 1000
