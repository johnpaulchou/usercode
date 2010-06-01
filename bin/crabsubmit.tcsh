#!/bin/tcsh -f

#============== Input parameters =================
set NUMARGS = $#argv
set PYTHONFILENAME = $1
set NUMBER_OF_EVENTS = $2
set EVENTS_PER_JOB = $3
set DATASETPATH = $4
set OUTPUTDIR = $5
set MINRUN = $6
set MAXRUN = $7

if ($NUMARGS<5) then
    echo "Usage: `basename $0` pythonfile nevents events_per_job dataset_path output_dir minrun maxrun"
    exit
endif

#============== Setup environment ================

# do this first
source /uscmst1/prod/grid/gLite_SL5_CRAB_27x.csh

# setup cms production
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod
cmsenv

# setup crab
#unsetenv CRABDIR
source /uscmst1/prod/grid/CRAB/crab.csh

# get the output filename from the configuration file
set OUTPUTFILENAME = `getoutputfilename.py ${PYTHONFILENAME}`
if ($? != 0) then
    echo "Could not determine output filename in cfg file ${PYTHONFILENAME}."
    echo "Assuming that there is no output file for this job."
    echo "Press a key to continue..."
    set OLDSTTYCONFIG=`stty -g`
    stty -icanon -echo min 1 time 0
    dd count=1 >& /dev/null
    stty $OLDSTTYCONFIG
    set OUTPUTFILENAME = "ttbarmc.root"
endif
set USERNAME = "johnpaul"
set WORKINGDIR = /uscms_data/d1/${USERNAME}/crab/${OUTPUTDIR}/
set RESILIENTDIR = /${OUTPUTDIR}/
set RUNSELECTION = ""
if ($NUMARGS>6) then
    if ($MINRUN == $MAXRUN) then
	set RUNSELECTION = "runselection            = ${MINRUN}"
    else
	set RUNSELECTION = "runselection            = ${MINRUN}-${MAXRUN}"
    endif
endif


#============== Create crab config file ==========

# condor = local, condor_g = global

set CFGFILENAME = "/tmp/`basename $0 .tcsh`.$$.cfg"
cat > $CFGFILENAME <<+EOF

[CRAB]
jobtype		= cmssw
scheduler	= condor_g

[CMSSW]
datasetpath		= ${DATASETPATH}
pset			= ${PYTHONFILENAME}
output_file             = ${OUTPUTFILENAME}
total_number_of_events	= ${NUMBER_OF_EVENTS}
events_per_job		= ${EVENTS_PER_JOB}
get_edm_output          = 1
${RUNSELECTION}

[USER]
return_data		= 0
copy_data		= 1
storage_element		= cmssrm.fnal.gov
storage_path		= /srm/managerv2?SFN=/11/store/user/${USERNAME}/
user_remote_dir		= ${RESILIENTDIR}
ui_working_dir          = ${WORKINGDIR}
srm_version		= 2
use_central_bossDB	= 0
use_boss_rt		= 0
check_user_remote_dir   = 0
thresholdLevel		= 100
eMail			= johnpaul@fnal.gov

[GRID]
rb			= CERN
proxy_server		= myproxy.cern.ch
virtual_organization	= cms
retry_count		= 2
lcg_catalog_type	= lfc
lfc_host		= lfc-cms-test.cern.ch
lfc_home		= /grid/cms

+EOF

echo "Using the following crab config file:"
echo "=========================================================================="
cat $CFGFILENAME
echo "=========================================================================="

echo "Working Directory is ${WORKINGDIR}"
echo "Resilient Directory is /pnfs/cms/WAX/11/store/user/${USERNAME}/${RESILIENTDIR}"


#============== Submit jobs ======================
crab -create -cfg ${CFGFILENAME}
crab -submit -c ${WORKINGDIR}


# remove temporary configuration file
rm $CFGFILENAME
