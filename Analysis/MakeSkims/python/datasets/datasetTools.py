from PhysicsTools.PatAlgos.tools.coreTools import *

def linkDatasetIntoStandardProcess(process, dataset):
    ## link everything up here
    process.GlobalTag.globaltag = dataset.globaltag
    process.source.fileNames = dataset.readFiles
    process.source.lumisToProcess = dataset.lumisToProcess
    for module in process._Process__outputmodules.values():
        module.outfile = dataset.outfile

def linkDatasetIntoPatProcess(process, dataset):
    ## do the basics
    linkDatasetIntoStandardProcess(process, dataset)

    ## if we're running on data remove MC matching
    if not dataset.isMC:
        removeMCMatching(process)

    ## do other things
    dataset.doOtherStuff(process)
