#!/bin/tcsh -f

#===========Input parameter===================
set LIBNAME="ADDDiPhoton.dat"
set NEVENTS=$1
set MSSCALE=$2
set NED=$3
set KKCONVENTION=$4
set PROCESS="MS${MSSCALE}_NED${NED}_KK${KKCONVENTION}"
set GLOBALTAG="START42_V12"

set LIBPATH=${CMSSW_BASE}/src/ADDDiPhoton/${PROCESS}/

# make the paths
rm -rf ${LIBPATH}
mkdir -p ${LIBPATH}
mkdir -p ${LIBPATH}/python
mkdir -p ${LIBPATH}/test

# copy files to the library path
cp *SherpaLibs.sh ${LIBPATH}/test
cat ${LIBNAME} | sed -e s/NEVENTS/${NEVENTS}/ | sed -e s/NED/$NED/ | sed -e s/MSSCALE/$MSSCALE/ | sed -e s/KKCONVENTION/$KKCONVENTION/ > ${LIBPATH}/test/Run.dat

# make and prepare the libraries
cd ${LIBPATH}/test
tar -czf sherpa_${PROCESS}_cards.tgz Run.dat 
./MakeSherpaLibs.sh  -i ./ -p ${PROCESS}
./PrepareSherpaLibs.sh -i ./ -p ${PROCESS} -a ADDDiPhoton/${PROCESS} -m PROD

# move to the libpath and compile the library
tar -xzf sherpa_${PROCESS}_MASTER.tgz
mv sherpa_${PROCESS}_cff.py ${LIBPATH}/python
cd ${LIBPATH}
scram b -j 4

# create the cmsRun config script
cd ${LIBPATH}/test
cmsDriver.py ADDDiPhoton/${PROCESS}/python/sherpa_${PROCESS}_cff.py -s GEN --conditions ${GLOBALTAG}::All --datatier GEN-SIM-RAW --eventcontent RAWSIM -n ${NEVENTS} --no_exec --fileout Sherpa_GEN_${PROCESS}.root

