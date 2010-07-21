## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *



## ------------------------------------------------------
#  NOTE: you can use a bunch of core tools of PAT to
#  taylor your PAT configuration; for a few examples
#  uncomment the lines below
## ------------------------------------------------------
#from PhysicsTools.PatAlgos.tools.coreTools import *

## remove MC matching from the default sequence
# removeMCMatching(process, ['Muons'])

## remove certain objects from the default sequence
# removeAllPATObjectsBut(process, ['Muons'])
# removeSpecificPATObjects(process, ['Electrons', 'Muons', 'Taus'])

## let it run
process.p = cms.Path(
    process.patDefaultSequence
    )


# this is the dataset we're going to run on
#import Analysis.MakeSkims.dataset_JetMETTauJuly15thReReco_cfi as dataset
import Analysis.MakeSkims.dataset_ZtautauSummer10_cfi as dataset

# link everything up here
process.GlobalTag.globaltag = dataset.globaltag
process.source.fileNames = dataset.readFiles
process.out.fileName = dataset.outfile
process.source.lumisToProcess = dataset.lumisToProcess

# if we're running on data remove MC matching
if not dataset.isMC:
    from PhysicsTools.PatAlgos.tools.coreTools import *
    removeMCMatching(process)

# turn off JECs (for now)
#process.allLayer1Jets.addJetCorrFactors = False

# reset other options
process.options.wantSummary = True
process.maxEvents.input = 100
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
