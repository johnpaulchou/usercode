## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## import tools
from PhysicsTools.PatAlgos.tools.coreTools import *

## this is the dataset we're going to run on
import Analysis.MakeSkims.dataset_JetMETTauJuly15thReReco_cfi as dataset
#import Analysis.MakeSkims.dataset_ZtautauM20CTEQ66Summer10_cfi as dataset

## link everything up here
process.GlobalTag.globaltag = dataset.globaltag
process.source.fileNames = dataset.readFiles
process.out.fileName = dataset.outfile
process.source.lumisToProcess = dataset.lumisToProcess

## if we're running on data remove MC matching
if not dataset.isMC:
    removeMCMatching(process)

## Filter out events
process.load('Analysis.MakeSkims.custom_filters_cfi')
process.out.SelectEvents = cms.untracked.PSet ( SelectEvents = cms.vstring('p') )

## set other running options
process.options.wantSummary = True
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents.input = 100
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)

## let it run
process.p = cms.Path(process.customFilterSequence*process.patDefaultSequence)
