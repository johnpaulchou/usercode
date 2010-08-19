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

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 10
process.out.outputCommands += ['keep *_towerMaker_*_*',
                               'keep *_generalTracks_*_*']
#                               'keep *_patPhotons_*_*',
#                               'keep *_patElectrons_*_*',
#                               'keep *_patMuons_*_*',
#                               'keep *_patTaus_*_*',
#                               'keep *_patJets_*_*']

## Filter out events
process.load('Analysis.MakeSkims.custom_filters_cfi')
process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )

## add electron correction stuff
#process.load("RecoEgamma.EgammaTools.correctedElectronsProducer_cfi")
#process.patElectrons.electronSource = cms.InputTag('gsfElectrons','','PAT')

## let it run
process.p = cms.Path(process.customFilterSequence*process.patDefaultSequence)
