#!/bin/tcsh -f

#============== Input parameters =================
set NUMARGS = $#argv
set PYTHONFILENAME = $1
set OUTPUTFILENAME = $2
set NUMBER_OF_EVENTS = $3
set EVENTS_PER_JOB = $4
set DATASETPATH = $5
set OUTPUTDIR = $6

if ($NUMARGS<6) then
	echo "Usage: `basename $0` pythonfile outputfile nevents events_per_job dataset_path output_dir"
	exit
endif

set WORKINGDIR = /uscms_data/d1/johnpaul/crab/${OUTPUTDIR}/
set RESILIENTDIR = /${OUTPUTDIR}/

#============== Setup environment ================

# setup cms production
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod

# setup crab
source /uscmst1/prod/grid/CRAB_2_5_1/crab.csh

#============== Create crab config file ==========

set CFGFILENAME = "/tmp/`basename $0 .tcsh`.$$.cfg"
cat > $CFGFILENAME <<+EOF

[CRAB]
jobtype		= cmssw
#scheduler       = condor
scheduler	= glite
#server_name     = bari

[CMSSW]
datasetpath		= ${DATASETPATH}
pset			= ${PYTHONFILENAME}
total_number_of_events	= ${NUMBER_OF_EVENTS}
events_per_job		= ${EVENTS_PER_JOB}
output_file		= ${OUTPUTFILENAME}

[USER]
return_data		= 0
copy_data		= 1
storage_element		= cmssrm.fnal.gov
storage_path		= /srm/managerv2?SFN=/resilient/johnpaul/
user_remote_dir		= ${RESILIENTDIR}
ui_working_dir          = ${WORKINGDIR}
srm_version		= 2
use_central_bossDB	= 0
use_boss_rt		= 0
thresholdLevel		= 100
eMail			= johnpaul@fnal.gov

[EDG]
rb			= CERN
proxy_server		= myproxy.cern.ch
virtual_organization	= cms
retry_count		= 0
lcg_catalog_type	= lfc
lfc_host		= lfc-cms-test.cern.ch
lfc_home		= /grid/cms

+EOF

echo "=========================================================================="
echo "Using the following crab config file:"
cat $CFGFILENAME
echo "=========================================================================="

echo "Working Directory is ${WORKINGDIR}"
echo "Resilient Directory is /pnfs/cms/WAX/resilient/johnpaul/${RESILIENTDIR}"


#============== Submit jobs ======================
crab -create -cfg $CFGFILENAME
crab -submit -c ${WORKINGDIR}


# remove temporary configuration file
rm $CFGFILENAME
