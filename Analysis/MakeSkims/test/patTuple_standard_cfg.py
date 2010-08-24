## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## add basic trigger stuff to the event (do this before setting the dataset)
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process, True)

## this is the dataset we're going to run on
#import Analysis.MakeSkims.datasets.dataset_JetMETTauJuly15thReReco_cfi as dataset
#import Analysis.MakeSkims.datasets.dataset_ZtautauM20CTEQ66Summer10_cfi as dataset
import Analysis.MakeSkims.datasets.dataset_DileptonTopChallenge_cfi as dataset
from Analysis.MakeSkims.datasets.datasetTools import *
linkDatasetIntoProcess(process, dataset)

## overwrite default PAT selection with my own selection
## this includes adding PFJets/MET, cleaning, etc.
from Analysis.MakeSkims.mySelectorSequence_cfi import *
ReplaceSequenceWithMySelectors(process)

## this is the event selection we're using
from Analysis.MakeSkims.dileptonEventSelection_cfi import *
setupEventSelection(process)

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 500
process.out.outputCommands += ['keep *_towerMaker_*_*',
                               'keep *_generalTracks_*_*']
