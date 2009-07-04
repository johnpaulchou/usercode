#!/bin/tcsh -f

# root file name to output
set ROOTFILENAME = "plots.root"

# write root script to get a ieta value
set ROOTSCRIPTNAME = "/tmp/`basename $0 .tcsh`.$$.C"
################################################################################################
cat > ${ROOTSCRIPTNAME} <<+EOF

{
  TFile f("${ROOTFILENAME}");
  int ieta=std::atoi(std::getenv("IETAFIND"));
  
  // convert ieta into a bin
  int bin=hRespIeta->GetYaxis()->FindBin(ieta);
  double val = hRespIeta->ProfileY()->GetBinContent(bin);
  std::cout << "Respcorr = " << (2.0+val)/(2.0-val) << std::endl;

}
+EOF
################################################################################################

# set the positive and negative responses
set POSRESP = ( 1.15833 1.14589 1.15 1.14048 1.22407 1.09756 1.07979 1.14484 1.22885 1.20833 1.21161 1.18929 1.17783 1.27585 1.29167 1.25481 1.26563 1.35172 1.2816 1.25988 1.22321 1.21111 1.175 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 )
set NEGRESP = ( 1.11142 1.1412 1.14483 1.13728 1.15791 1.16586 1.11951 1.15064 1.20513 1.14601 1.11458 1.23643 1.29709 1.25045 1.2652 1.451 1.29944 1.27661 1.32585 1.26742 1.252 1.27885 1.26437 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 )

foreach ieta ( 24 26 28 30 32 34 36 38)
    # set the maximum calibrated ieta
    @ MAXCALIBRATEDIETA = ($ieta - 1)

    # these are all of the ietas we are going to extract
    if ($ieta == 24) then
	set ietas = ( 24 25 )
    else if ($ieta == 26) then
	set ietas = ( 26 27 )
    else if ($ieta == 28) then
	set ietas = ( 28 29 )
    else if ($ieta == 30) then
	set ietas = ( 30 31 )
    else if ($ieta == 32) then
	set ietas = ( 32 33 )
    else if ($ieta == 34) then
	set ietas = ( 34 35 )
    else if ($ieta == 36) then
	set ietas = ( 36 37 )
    else if ($ieta == 38) then
	set ietas = ( 38 39 40 41 )
    endif


    # write a python script to overwrite the response corrections
    set PYTHONSCRIPTNAME = "/tmp/`basename $0 .tcsh`.$$.$ieta.py"

################################################################################################
cat > ${PYTHONSCRIPTNAME} <<+EOF

import FWCore.ParameterSet.Config as cms

process = cms.Process('MyRec')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('HcalClosureTest.Analyzers.calcrespcorrdijets_cfi')

process.calcrespcorrdijets.rootHistFilename = cms.string('${ROOTFILENAME}')
process.calcrespcorrdijets.maxDeltaEta = cms.double(1.5)
process.calcrespcorrdijets.minJetEt = cms.double(30)
process.calcrespcorrdijets.maxModifiedEMF = cms.double(0.9)

process.calcrespcorrdijets.maxCalibratedIEta = cms.int32($MAXCALIBRATEDIETA)
process.calcrespcorrdijets.respCorr = cms.vdouble($NEGRESP[41], $NEGRESP[40], $NEGRESP[39], $NEGRESP[38], $NEGRESP[37], $NEGRESP[36], $NEGRESP[35], $NEGRESP[34], $NEGRESP[33], $NEGRESP[32], $NEGRESP[31], $NEGRESP[30], $NEGRESP[29], $NEGRESP[28], $NEGRESP[27], $NEGRESP[26], $NEGRESP[25], $NEGRESP[24], $NEGRESP[23], $NEGRESP[22], $NEGRESP[21], $NEGRESP[20], $NEGRESP[19], $NEGRESP[18], $NEGRESP[17], $NEGRESP[16], $NEGRESP[15], $NEGRESP[14], $NEGRESP[13], $NEGRESP[12], $NEGRESP[11], $NEGRESP[10], $NEGRESP[9], $NEGRESP[8], $NEGRESP[7], $NEGRESP[6], $NEGRESP[5], $NEGRESP[4], $NEGRESP[3], $NEGRESP[2], $NEGRESP[1], 0.0, $POSRESP[1], $POSRESP[2], $POSRESP[3], $POSRESP[4], $POSRESP[5], $POSRESP[6], $POSRESP[7], $POSRESP[8], $POSRESP[9], $POSRESP[10], $POSRESP[11], $POSRESP[12], $POSRESP[13], $POSRESP[14], $POSRESP[15], $POSRESP[16], $POSRESP[17], $POSRESP[18], $POSRESP[19], $POSRESP[20], $POSRESP[21], $POSRESP[22], $POSRESP[23], $POSRESP[24], $POSRESP[25], $POSRESP[26], $POSRESP[27], $POSRESP[28], $POSRESP[29], $POSRESP[30], $POSRESP[31], $POSRESP[32], $POSRESP[33], $POSRESP[34], $POSRESP[35], $POSRESP[36], $POSRESP[37], $POSRESP[38], $POSRESP[39], $POSRESP[40], $POSRESP[41])

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)
readFiles.append('file:/afs/cern.ch/user/j/johnpaul/localscratch/QCDpt80_1.root')
readFiles.append('file:/afs/cern.ch/user/j/johnpaul/localscratch/QCDpt80_2.root')
readFiles.append('file:/afs/cern.ch/user/j/johnpaul/localscratch/QCDpt80_3.root')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(100000)
process.p = cms.Path(process.calcrespcorrdijets)

+EOF
################################################################################################
    cat ${PYTHONSCRIPTNAME}

    # run over the python script
    cmsRun ${PYTHONSCRIPTNAME}

    # get the new responses
    foreach i ($ietas)
	setenv IETAFIND $i
	set POSRESP[$i] = `root -b -q -n ${ROOTSCRIPTNAME} | awk '/Respcorr = / { print $3 }'`
	setenv IETAFIND -$i
	set NEGRESP[$i] = `root -b -q -n ${ROOTSCRIPTNAME} | awk '/Respcorr = / { print $3 }'`
    end

    # remove the script
    rm -f ${PYTHONSCRIPTNAME}
end

echo $POSRESP[-41]
echo $NEGRESP[-41]

# remove the script
rm -f ${ROOTSCRIPTNAME}

