#!/bin/tcsh -f
#usesage ./condor_execute_manual.sh MSSCALE NED KKCONVENTION LOWMASSCUT HIGHMASSCUT 
#===========Input parameter===================
set VERSION="3_9_2"
set VERNAME="3_9_2"
set PRENAME="ADDdiphoton_gen"
set CURRENTDIR="SherpaGenADDphoton"
set MSSCALE=$1
set NED=$2
set KKCONVENTION=$3
set LOWMASSCUT=$4
set HIGHMASSCUT=$5
set PROCESS = "${PRENAME}_MS${MSSCALE}_NED${NED}_KK${KKCONVENTION}_MASSCUT_${LOWMASSCUT}_${HIGHMASSCUT}"
#======environment setup==========
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod
cd /uscms/home/johnpaul/cmssw/CMSSW_${VERSION}/src
pwd
eval `scramv1 runtime -csh`
cd $CURRENTDIR
pwd

#======create library==========
set WORKDIR=$PROCESS
set subWORKDIR="test"
rm -rf $WORKDIR
mkdir $WORKDIR
cd $WORKDIR
rm Run.dat

cat ../Run.dat | sed -e s/EVENTNUM/100/ | sed -e s/NED/$NED/ | sed -e s/MSSCALE/$MSSCALE/ | sed -e s/KKCONVENTION/$KKCONVENTION/ | sed -e s/LOWMASSCUT/$LOWMASSCUT/ | sed -e s/HIGHMASSCUT/$HIGHMASSCUT/ > Run.dat
cp ../Analysis.dat .
tar -czvf sherpa_${PROCESS}_cards.tgz Run.dat

cp ../MakeSherpaLibs.sh .
cp ../PrepareSherpaLibs.sh .
#./MakeSherpaLibs.sh -d $CMSSW_BASE -i ./ -p $PROCESS
./MakeSherpaLibs.sh -i ./ -p $PROCESS
#mv sherpa_${PROCESS}_crdL.tgz sherpa_${PROCESS}_crdE.tgz

#mkdir tmp
#mv sherpa_${PROCESS}_crdE.tgz tmp
#cd tmp
#tar -xzvf sherpa_${PROCESS}_crdE.tgz
#rm sherpa_${PROCESS}_crdE.tgz
#cat ../../Run.dat | sed -e s/EVENTNUM/100000/ | sed -e s/NED/$NED/ | sed -e s/MSSCALE/$MSSCALE/ | sed -e s/KKCONVENTION/$KKCONVENTION/ | sed -e s/LOWMASSCUT/$LOWMASSCUT/ | sed -e s/HIGHMASSCUT/$HIGHMASSCUT/ > Run.dat
#tar -czvf sherpa_${PROCESS}_crdE.tgz *.*
#mv sherpa_${PROCESS}_crdE.tgz ../
#cd ../
#tar -czvf sherpa_${PROCESS}_cards.tgz Run.dat

./PrepareSherpaLibs.sh -i ./ -p $PROCESS -c MC_39Y_V5 -m PROD

tar -xzf sherpa_${PROCESS}_MASTER.tgz
mkdir python
cp sherpa_${PROCESS}_cff.py sherpa_custom.py python
scramv1 b

#./PrepareSherpaLibs.sh -d $CMSSW_BASE -i ./ -p $PROCESS -a SherpaGenADDelectron/$WORKDIR -m LOCAL
#mkdir test
#cp sherpa_${PROCESS}_MASTER.tgz test
#cd test
#tar -xzf sherpa_${PROCESS}_MASTER.tgz
#mkdir ../python
#cp sherpa_custom.py ../python
#cp sherpa_${PROCESS}_cff.py ../python
#cd ../python
#scramv1 b
#cd ../test
#echo $PWD

#cmsDriver.py $CURRENTDIR/$WORKDIR/sherpa_${PROCESS}_cff.py -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT --conditions MC_38Y_V8::All --datatier GEN-SIM-RAW --eventcontent RAWSIM  --customise $CURRENTDIR/$WORKDIR/sherpa_custom.py -n 20 --no_exec --fileout outputGENDIGIHLT_${PROCESS}.root
cmsDriver.py $CURRENTDIR/$WORKDIR/sherpa_${PROCESS}_cff.py -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO --conditions MC_39Y_V5::All --datatier GEN-SIM-RECO --eventcontent RECOSIM  --customise $CURRENTDIR/$WORKDIR/sherpa_custom.py -n 10000 --no_exec --fileout output_${PROCESS}.root

cat sherpa_${PROCESS}_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_RECO.py ../modified.py > runSherpa.py

#cmsRun runSherpa.py > log_$PROCESS

#cp log_$PROCESS ../Log

echo "MS, NED, KK, LOWMASS, HIGHMASS:"
echo "MS: " ${MSSCALE}
echo "NED: " ${NED}
echo "KKCONVENTION: " ${KKCONVENTION}
echo "LOWMASSCUT: " ${LOWMASSCUT}
echo "HIGHMASSCUT: " ${HIGHMASSCUT}

#process.generator.SherpaParameters.Run[1] = 'EVENTS = 100000'
#process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
