#!/bin/tcsh -f

#===========Input parameter===================
set LIBNAME="ADDDiPhoton"
set CONDITIONS="MC_39Y_V5"
set NEVENTS=20

set MSSCALE=$1
set NED=$2
set KKCONVENTION=$3
set LOWMASSCUT=$4
set HIGHMASSCUT=$5
set RANDOMSEED=$6
set PROCESS = "MS${MSSCALE}_NED${NED}_KK${KKCONVENTION}_MASSCUT_${LOWMASSCUT}_${HIGHMASSCUT}"

#======create library==========
set LIBPATH=${CMSSW_BASE}/src/${LIBNAME}
set WORKPATH=${LIBPATH}/$PROCESS
rm -rf $WORKPATH
mkdir -p $WORKPATH

# copy files to the workpath
set DATFILE=ADDDiPhoton.dat
set ANAFILE=${CMSSW_RELEASE_BASE}/src/GeneratorInterface/SherpaInterface/data/Analysis.dat
set MAKELIBSFILE=MakeSherpaLibs.sh
set PREPLIBSFILE=PrepareSherpaLibs.sh

cat ${DATFILE} | sed -e s/EVENTNUM/${NEVENTS}/ | sed -e s/NED/$NED/ | sed -e s/MSSCALE/$MSSCALE/ | sed -e s/KKCONVENTION/$KKCONVENTION/ | sed -e s/LOWMASSCUT/$LOWMASSCUT/ | sed -e s/HIGHMASSCUT/$HIGHMASSCUT/ > ${WORKPATH}/Run.dat
cp ${ANAFILE} ${WORKPATH}/Analysis.dat
cp ${MAKELIBSFILE} ${WORKPATH}/MakeSherpaLibs.sh
cp ${PREPLIBSFILE} ${WORKPATH}/PrepareSherpaLibs.sh

# make the library
cd ${WORKPATH}
tar -czf sherpa_${PROCESS}_cards.tgz Run.dat Analysis.dat
./MakeSherpaLibs.sh -i ./ -p $PROCESS
./PrepareSherpaLibs.sh -i ./ -p $PROCESS -c ${CONDITIONS} -m PROD
tar -xzf sherpa_${PROCESS}_MASTER.tgz
mkdir python
cp sherpa_${PROCESS}_cff.py sherpa_custom.py python
scram b

# create cmsRun config script
cmsDriver.py $LIBNAME/$PROCESS/sherpa_${PROCESS}_cff.py -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --conditions ${CONDITIONS}::All --datatier GEN-SIM-RECO --eventcontent RECOSIM  --customise $LIBNAME/$PROCESS/sherpa_custom.py -n ${NEVENTS} --no_exec --fileout output_${PROCESS}.root

# modify config script
echo "process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )" > modified.py
echo "process.RandomNumberGeneratorService = cms.Service('RandomNumberGeneratorService'," >> modified.py
echo "   moduleSeeds = cms.PSet(generator = cms.untracked.uint32(${RANDOMSEED})))" >> modified.py

cat sherpa_${PROCESS}_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_RECO.py modified.py > runSherpa.py

# final result
echo "MS, NED, KK, LOWMASS, HIGHMASS:"
echo "MS: " ${MSSCALE}
echo "NED: " ${NED}
echo "KKCONVENTION: " ${KKCONVENTION}
echo "LOWMASSCUT: " ${LOWMASSCUT}
echo "HIGHMASSCUT: " ${HIGHMASSCUT}
