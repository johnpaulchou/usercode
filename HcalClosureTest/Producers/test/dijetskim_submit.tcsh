#!/bin/tcsh -f

# these are the basic parameters
set BASE = $1
set TEST = false
set MAXEVENTS = -1
set EVENTS_PER_JOB = 100000 # we expect ~3 kB per event that we run over

# set the location of where we're going to store the output
# this is appended to the pnfs or ~/nobackup directory
set STORAGEDIRNAME = DiJetCalibration/${BASE}

# the datahandling config file name is determined from the base
set DATAHANDLINGCFGNAME = DataHandling.ConfigFiles.${BASE}_cff

# get the data pathname from the python file
# this is a cute hack...
set DATASETPATH = `python -c 'import '${DATAHANDLINGCFGNAME}' as data; print data.path.value()'`

# the python filename we're about to create
set PYTHONNAME = "${BASE}.py"

# the name of the file that cmsRun will output
set OUTPUTNAME = "${BASE}.root"

# setup cms production
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod

cmsenv

############################################################################

# create the python file here
cat > ${PYTHONNAME} <<+EOF

import FWCore.ParameterSet.Config as cms

process = cms.Process('MyReco')

# import basic services
process.load("FWCore.MessageService.MessageLogger_cfi")

# import the data information
import ${DATAHANDLINGCFGNAME} as data

# setup a JetFilter
process.load('HcalClosureTest.Filters.jetfilter_cfi')

# set up a PtHatFilter
process.pthatfilter = cms.EDFilter(
    'MCProcessFilter',
    ProcessID = cms.untracked.vint32(0),
    MinPthat  = cms.untracked.vdouble(data.minPthat.value()),
    MaxPthat  = cms.untracked.vdouble(data.maxPthat.value())
    )


# set some input/output information
process.source = cms.Source('PoolSource', fileNames = data.readFiles)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(${MAXEVENTS}) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(100)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('file:${OUTPUTNAME}'),
			       SelectEvents = cms.untracked.PSet(
			          SelectEvents = cms.vstring('p')
				  ),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep CaloTowersSorted_*_*_*',
                                                                      'keep recoCaloJets_iterativeCone5CaloJets_*_*',
                                                                      'keep recoCaloJets_sisCone5CaloJets_*_*')
                               )

#set the path
process.p = cms.Path(process.pthatfilter*process.jetfilter)
process.e = cms.EndPath(process.out)


+EOF

############################################################################

echo "=========================================================================="
echo "Using the following python script:"
cat ${PYTHONNAME}
echo "=========================================================================="
# finished creating the python script



# run it either locally or with crab
if($TEST == false) then
    crabsubmit.tcsh ${PYTHONNAME} ${MAXEVENTS} ${EVENTS_PER_JOB} ${DATASETPATH} ${STORAGEDIRNAME}
else
    cmsRun ${PYTHONNAME}
endif

# remove the temporary script
rm ${PYTHONNAME}
