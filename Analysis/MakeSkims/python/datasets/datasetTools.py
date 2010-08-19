from PhysicsTools.PatAlgos.tools.coreTools import *

def linkDatasetIntoProcess(process, dataset):
    ## link everything up here
    process.GlobalTag.globaltag = dataset.globaltag
    process.source.fileNames = dataset.readFiles
    process.out.fileName = dataset.outfile
    process.source.lumisToProcess = dataset.lumisToProcess

    ## if we're running on data remove MC matching
    if not dataset.isMC:
        removeMCMatching(process)

    ## do other things
    dataset.doOtherStuff(process)
