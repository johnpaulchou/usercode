from PhysicsTools.PatAlgos.tools.coreTools import *

def linkDatasetIntoStandardProcess(process, dataset, setGlobalTag=True):
    ## link everything up here
    if setGlobalTag:
        process.GlobalTag.globaltag = dataset.globaltag
    process.source.fileNames = dataset.readFiles
    for module in process._Process__outputmodules.values():
        module.fileName = dataset.outfile

def linkDatasetIntoPatProcess(process, dataset):
    ## do the basics
    linkDatasetIntoStandardProcess(process, dataset)

    ## if we're running on data remove MC matching
    if not dataset.isMC:
        removeMCMatching(process)

    ## do other things
    dataset.doOtherStuff(process)
