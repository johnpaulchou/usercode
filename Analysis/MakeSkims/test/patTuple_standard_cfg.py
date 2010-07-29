## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## import tools
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *

## this is the dataset we're going to run on
#import Analysis.MakeSkims.datasets.dataset_JetMETTauJuly15thReReco_cfi as dataset
import Analysis.MakeSkims.datasets.dataset_ZtautauM20CTEQ66Summer10_cfi as dataset

## link everything up here
process.GlobalTag.globaltag = dataset.globaltag
process.source.fileNames = dataset.readFiles
process.out.fileName = dataset.outfile
process.source.lumisToProcess = dataset.lumisToProcess

## if we're running on data remove MC matching, but add PAT trigger info
if not dataset.isMC:
    removeMCMatching(process)


switchOnTrigger(process, True)

## add PF jets
addJetCollection(process,cms.InputTag('ak5PFJets'),
                                  'AK5', 'PF',
                                  doJTA        = True,
                                  doBTagging   = True,
                                  jetCorrLabel = ('AK5', 'PF'),
                                  doType1MET   = False, ## there is no use to apply residual jet corrections on CaloMET
                                  doL1Cleaning = True,
                                  doL1Counters = False,
                                  genJetCollection=cms.InputTag("ak5GenJets"),
                                  doJetID      = False  ## there is no use of jetID for Pflow jets
                                  )

## add extra MET categories
addPfMET(process, 'PF')

## overwrite default PAT selection with my own selection
from Analysis.MakeSkims.mySelectorSequence_cfi import *
ReplaceSequenceWithMySelectors(process)

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 100
process.out.outputCommands += ['keep *_towerMaker_*_*',
                               'keep *_patPhotons_*_*',
                               'keep *_patElectrons_*_*',
                               'keep *_patMuons_*_*',
                               'keep *_patTaus_*_*',
                               'keep *_patJets_*_*']

## Filter out events
process.load('Analysis.MakeSkims.custom_filters_cfi')
process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )

## let it run
process.p = cms.Path(process.customFilterSequence*process.patDefaultSequence)
