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
echo "from FWCore.ParameterSet.VarParsing import VarParsing" > modified.py
echo "options = VarParsing ('analysis')" >> modified.py
echo "options.register('seed',1,VarParsing.multiplicity.singleton,VarParsing.varType.int,'seed')" >> modified.py
echo "options.maxEvents=20" >> modified.py
echo "options.parseArguments()" >> modified.py
echo "process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )" >> modified.py
echo "process.RandomNumberGeneratorService.generator = cms.PSet(initialSeed=cms.untracked.uint32(options.seed), engineName = cms.untracked.string('HepJamesRandom'))" >> modified.py
echo "process.maxEvents.input = cms.untracked.int32(options.maxEvents)" >> modified.py
cat sherpa_${PROCESS}_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_RECO.py modified.py > runSherpa.py

# create runscript
echo '#\!/bin/tcsh -f' > run.tcsh
echo 'set CWD=${PWD}' >> run.tcsh
echo 'set WD=`dirname $0`' >> run.tcsh
echo 'cd $WD' >> run.tcsh
echo 'cmsRun runSherpa.py maxEvents=$1 seed=$2' >> run.tcsh
echo cp output_${PROCESS}.root \$CWD/output_${PROCESS}_\$2.root >> run.tcsh
chmod 777 run.tcsh

# run runscript to process library
./run.tcsh 1 1

# final result
echo "MS, NED, KK, LOWMASS, HIGHMASS:"
echo "MS: " ${MSSCALE}
echo "NED: " ${NED}
echo "KKCONVENTION: " ${KKCONVENTION}
echo "LOWMASSCUT: " ${LOWMASSCUT}
echo "HIGHMASSCUT: " ${HIGHMASSCUT}
