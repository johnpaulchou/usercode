#!/bin/bash
#
#  file:        MakeSherpaLibs.sh
#  description: BASH script handling the SHERPA data cards for the
#               library production and cross section calculation
#  uses:        the required SHERPA data cards (+ libraries) [see below]
#
#  author:      Markus Merschmeyer, RWTH Aachen
#  date:        28th April 2011
#  version:     3.3
#  changed: 	Martin Niegel, KIT, 2011/06/07
#		Fix for Sherpa 1.3.0
#



# +-----------------------------------------------------------------------------------------------+
# function definitions
# +-----------------------------------------------------------------------------------------------+

print_help() {
    echo "" && \
    echo "MakeSherpaLibs version 3.0" && echo && \
    echo "options: -d  path       (optional) path to your SHERPA installation (otherwise the SHERPA" && \
    echo "                         package belonging to the release under '\$CMSSW_BASE' is used)" && \
    echo "                         -> ( "${shr}" )" && \
    echo "         -i  path       path to SHERPA datacard (and library, see -o) files" && \
    echo "                         -> ( "${inc}" )" && \
    echo "         -p  process    SHERPA process/dataset name ( "${prc}" )" && \
    echo "         -o  options    library/cross section options ( "${lbo}" )" && \
    echo "                         [ 'LBCR' : generate libraries and cross sections     ]" && \
    echo "                         [ 'LIBS' : generate libraries only                   ]" && \
    echo "                         [ 'CRSS' : generate cross sections, needs libraries! ]" && \
    echo "                         [ 'EVTS' : generate events, needs libs + crss. sec.! ]" && \
    echo "         -f  path       output path for SHERPA library & cross section files" && \
    echo "                         -> ( "${fin}" )" && \
    echo "         -D  filename   (optional) name of data card file ( "${cfdc}" )" && \
    echo "         -L  filename   (optional) name of library file ( "${cflb}" )" && \
    echo "         -C  filename   (optional) name of cross section file ( "${cfcr}" )" && \
    echo "         -h             display this help and exit" && echo
}

check_md5() {
# $1 : what is to be checked ('CRDS','CRDFILE','LIBS''LIBFILE','CRSS','CRSFILE') ?
# $2 : who is requesting the check ('CRDS','LIBS','CRSS','EVTS') ?
# $3 : name of the file containing the checksums
IMD=`which md5sum | grep -c -i "not found"`
if [ $IMD -eq 0 ]; then
  if [ -e $3 ]; then
    echo " <I> performing MD5 check: "$1" "$2" "$3
    rslt=`md5sum --check $3`
    fpatt="OK"
    nok=`echo $rslt | grep -o -i $fpatt | grep -c -i $fpatt`
    fpatt="FAILED"
    nfail=`echo $rslt | grep -o -i $fpatt | grep -c -i $fpatt`
    nline=`cat $3 |wc -l`
    echo " <I> MD5 file has "$nline" entries"
    echo " <I>  -> OK: "$nok", FAILED: "$nfail
    if [ $nfail -gt 0 ]; then
      if [ "$1" = "CRDS" ] && [ "$2" = "CRDS" ]; then
        echo " <E> data cards do not match their own MD5 sums,"
        echo " <E>  stopping..."
        exit 1
      fi
      if [ "$1" = "CRDFILE" ] && [ "$2" = "LIBS" ]; then
        echo " <W> libraries were probably not generated with this data card file"
      fi
      if [ "$1" = "CRDS" ] && [ "$2" = "LIBS" ]; then
        echo " <W> some of the data cards have changed since library generation"
        echo " <W>  please make sure these changes are harmless:"
        cnt=1
        while [ $cnt -le $nline ]; do
          let fidx=$cnt+1
          cfile=`echo $rslt |cut -f $cnt -d ":" | cut -f 3 -d " "`
          cstat=`echo $rslt |cut -f $fidx -d ":" | cut -f 2 -d " "`
          if [ "$cstat" = "FAILED" ]; then
            echo " <W>  -> file '"$cfile"' does not match MD5 sum"
          fi
          let cnt=$cnt+1
        done
      fi
      if [ "$1" = "LIBS" ] && [ "$2" = "LIBS" ]; then
        echo " <E> libraries do not match their own MD5 sums,"
        echo " <E>  stopping..."
        exit 1
      fi
    fi
  else
    echo " <W> file "$3" does not exist, skipping MD5 test"
  fi # check existence od md5sum file
fi   # check availability of 'md5sum'
}

clean_libs() {
  DIRS=`find Process -name P?_?`" "`find Process -name P?_??`
  for J in $DIRS ; do
    echo "."
    echo "======================"
    echo "$J";
    echo "======================"
    cd $J
    make clean
    rm config* Makefile*
#    rm *.tex
    rm aclocal.m4 ChangeLog depcomp install-sh libtool ltmain.sh missing
    rm AUTHORS COPYING INSTALL NEWS README 
    rm -rf autom4te.cache
    find ./ -type f -name 'Makefile*' -exec rm -rf {} \;
    find ./ -type d -name '.deps'     -exec rm -rf {} \;
    find ./ -type f -name '*.C'       -exec rm -rf {} \;
    find ./ -type f -name '*.H'       -exec rm -rf {} \;
    cd ../..
  done
}

fix_makelibs() {
# fix 'makelibs' script for 32-bit compatibility
  echo " <W> setting 32bit flags in 'makelibs' script !!!"

  CNFFLG="CFLAGS=-m32 FFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32"
  MKEFLG="CFLAGS=-m32 FFLAGS=-m32 CXXFLAGS=\"-O2 -m32\" LDFLAGS=-m32"

  if [ -e ${SHERPA_SHARE_PATH} ]; then
    sed -e "s/configure/configure ${CNFFLG}/" < ${SHERPA_SHARE_PATH}/makelibs > ./makelibs.tmp
    sed -e "s/-j2 \"CXXFLAGS=-O2\"/-j2 ${MKEFLG}/" < ./makelibs.tmp > ./makelibs
    rm ./makelibs.tmp
    chmod 755 ./makelibs
  else
    echo " <E> SHERPA_SHARE_PATH does not exist, stopping..."
    exit
  fi

}





# +-----------------------------------------------------------------------------------------------+
# start of the script
# +-----------------------------------------------------------------------------------------------+

# save current path
HDIR=`pwd`

# dummy setup (if all options are missing)
shr=${HDIR}/SHERPA-MC-1.1.2        # path to SHERPA installation
scrloc=`which scramv1 &> tmp.tmp; cat tmp.tmp | cut -f1 -d"/"; rm tmp.tmp`
if [ "${scrloc}" = "" ]; then
  shr=`scramv1 tool info sherpa | grep "SHERPA_BASE" | cut -f2 -d"="`
  shrinit=$shr
fi
pth="TMP"                          # name of SHERPA data card directory
prc="XXX"                          # SHERPA process name
lbo="LBCR"                         # library/cross section option
inc=${HDIR}                        # path to SHERPA datacards (libraries)
cfdc=""                            # custom data card file name
cflb=""                            # custom library file name
cfcr=""                            # custom cross section file name
fin=${HDIR}                        # output path for SHERPA libraries & cross sections

# get & evaluate options
while getopts :d:i:p:o:f:D:L:C:h OPT
do
  case $OPT in
  d) shr=$OPTARG ;;
  i) inc=$OPTARG ;;
  p) prc=$OPTARG ;;
  o) lbo=$OPTARG ;;
  f) fin=$OPTARG ;;
  D) cfdc=$OPTARG ;;
  L) cflb=$OPTARG ;;
  C) cfcr=$OPTARG ;;
  h) print_help && exit 0 ;;
  \?)
    shift `expr $OPTIND - 1`
    if [ "$1" = "--help" ]; then print_help && exit 0;
    else 
      echo -n "MakeSherpaLibs: error: unrecognized option "
      if [ $OPTARG != "-" ]; then echo "'-$OPTARG'. try '-h'"
      else echo "'$1'. try '-h'"
      fi
      print_help && exit 1
    fi
    shift 1
    OPTIND=1
  esac
done


# make sure to use absolute path names...
cd ${shr} && shr=`pwd`; cd ${HDIR}
cd ${inc} && inc=`pwd`; cd ${HDIR}
cd ${fin} && fin=`pwd`; cd ${HDIR}

# test whether to take $shr from CMSSW installation
if [ ! "$CMSSW_BASE" = "" ] && [ "$shr" = "$shrinit" ]; then
  newshr=""
  cd $CMSSW_BASE &&
  newshr=`scramv1 tool info sherpa | grep BASE | cut -f 2 -d "="`
  if [ "${newshr}" = "" ]; then
    echo " <E> no 'sherpa' tool defined in CMSSW, are you sure that"
    echo " <E>  1. the command 'scramv1' is available ?"
    echo " <E>  2. the path to your CMSSW is correct ?"
    echo " <E>  3. there exists a SHERPA package in your CMSSW ?"
    exit 0
  fi
  shr=${newshr}
  cd ${HDIR}
fi
if [ "${SHERPA_SHARE_PATH}"   = "" ]; then export SHERPA_SHARE_PATH=${shr}/share/SHERPA-MC;     fi
if [ "${SHERPA_INCLUDE_PATH}" = "" ]; then export SHERPA_INCLUDE_PATH=${shr}/include/SHERPA-MC; fi
if [ "${SHERPA_LIBRARY_PATH}" = "" ]; then export SHERPA_LIBRARY_PATH=${shr}/lib/SHERPA-MC;     fi


# find 'Run' directory
shrun=${HDIR}/SHERPATMP
mkdir -p ${shrun}
pth=${shrun}/${pth} #SHERPA 1.3.0 needs full path, MN 070611

echo "  -> SHERPA path: '"${shr}"'"
echo "  -> SHERPA run path: '"${shrun}"'"
echo "  -> PROCESS name: '"${prc}"'"
echo "  -> Library & cross section otions: '"${lbo}"'"
echo "  -> include path: '"${inc}"'"
echo "  -> custom data card file name: '"${cfdc}"'"
echo "  -> custom library file name: '"${cflb}"'"
echo "  -> custom cross section file name: '"${cfcr}"'"
echo "  -> output path: '"${fin}"'"


### go to 'Run' subdirectory of SHERPA
cd ${shrun}


### set base name for SHERPA output file(s) and directories
outflbs=sherpa_${prc}
cardfile=${outflbs}_cards.tgz             # input card file (master -> libraries)
if [ "${lbo}" = "CRSS" ]; then
  cardfile=${outflbs}_crdC.tgz
elif [ "${lbo}" = "EVTS" ]; then
  cardfile=${outflbs}_crdE.tgz
fi
crdsmd5s=md5sums_crds.md5                 # MD5 sums -> cards
crdfmd5s=md5sums_crdsfile.md5             # MD5 sum -> cardfile
libsfile=${outflbs}_libs.tgz              # output libs
libsmd5s=md5sums_libs.md5                 # MD5 sums -> libs
libfmd5s=md5sums_libsfile.md5             # MD5 sum -> libfile
crssfile=${outflbs}_crss.tgz              # output cross sections
crssmd5s=md5sums_crss.md5                 # MD5 sums -> cross sections
crsfmd5s=md5sums_crssfile.md5             # MD5 sum -> cross section file
evtsfile=${outflbs}_evts.tgz              # output events
evtsmd5s=md5sums_evts.md5                 # MD5 sums -> events
evtfmd5s=md5sums_evtsfile.md5             # MD5 sum -> eventfile
if [ ! "${cfdc}" = "" ]; then
  cardfile=${cfdc}                        # custom input data card file
  echo " <I> using custom data card file: "${cardfile}
fi
if [ ! "${cflb}" = "" ]; then
  libsfile=${cflb}                        # custom input library file
  echo " <I> using custom library file: "${libsfile}
fi
if [ ! "${cfcr}" = "" ]; then
  crssfile=${cfcr}                        # custom input cross section file
  echo " <I> using custom cross section file: "${crssfile}
fi
crdlfile=${outflbs}_crdL.tgz              # output cardfile (-> from library production)
crdcfile=${outflbs}_crdC.tgz              # output cardfile (-> from cross section calculation)
crdefile=${outflbs}_crdE.tgz              # output cardfile (-> from event generation)
loglfile=${outflbs}_logL.tgz              # output messages (-> from library production)
logcfile=${outflbs}_logC.tgz              # output messages (-> from cross section calculation)
logefile=${outflbs}_logE.tgz              # output messages (-> from event generation)
#
dir1="Process"                            # SHERPA process directory name
dir2="Result"                             # SHERPA results directory name
dir3="Analysis"                           # SHERPA analysis directory name

### clean up existing xsection files
for FILE in `ls xsections_*.dat 2> /dev/null`; do
  echo " <W> deleting existing cross section file: "${FILE}
  rm ${FILE}
done


### test whether PATH exists
if [ -e ${pth} ]; then
  echo " path '"${pth}"' exists,"
  echo "  -> cleaning path '"${pth}"'"
  rm -rf ${pth}/*
else
  echo " path '"${pth}"' does not exist,"
  echo "  -> creating path '"${pth}"'"
  mkdir ${pth}
fi
#MM() pth="./"$pth
cd ${pth}
#MM() pth=`pwd`


### get data card (+ library) tarball(s) from include path
cp ${inc}/${cardfile} .
if [ "${lbo}" = "CRSS" ]; then
  cp ${inc}/${libsfile} .
fi
if [ "${lbo}" = "EVTS" ]; then
  cp ${inc}/${libsfile} .
  cp ${inc}/${crssfile} .
fi


### check existence of data card file
if [ -e ${cardfile} ]; then
  echo " data card file '"${cardfile}"' exists,"
  echo "  -> unpacking data card file"
  tar -xzf ${cardfile}
  check_md5 "CRDS" "CRDS" ${crdsmd5s}
else
  echo " <E> no data card file found"
  echo "  -> stopping..."
  exit
fi


### find out whether COMIX or AMEGIC is being used
runfile="Run.dat"
if [ -e ${runfile} ]; then
  iamegic=`cat ${runfile} | grep -i "ME_SIGNAL_GENERATOR" | grep -i -c "AMEGIC"`
  if [ ${iamegic} -gt 0 ]; then
    FLGCOMIX="FALSE"                   # use AMEGIC
    echo " <I> using AMEGIC ME generator"
  else
    FLGCOMIX="TRUE"                    # use COMIX
    echo " <I> using COMIX ME generator"
    lbo="LBCR"
  fi
fi
###exit


### check required subdirectories
## generate/clean 'Process' subdirectory
if [ ! -e ${dir1} ]; then
  echo " '"${dir1}"' subdirectory does not exist and will be created"
  mkdir ${dir1}
else
  echo " cleaning '"${dir1}"' subdirectory"
  rm -rf ${dir1}/*
fi
## generate/clean 'Result' subdirectory
if [ ! -e ${dir2} ]; then
  echo " '"${dir2}"' subdirectory does not exist and will be created"
  mkdir ${dir2}
else
  echo " cleaning '"${dir2}"' subdirectory"
  rm -rf ${dir2}/*
fi

### check, whether only cross sections have to be calculated
if [ "${lbo}" = "CRSS" ] || [ "${lbo}" = "EVTS" ]; then
  if [ -e ${libsfile} ]; then
    echo " <I> library file '"${libsfile}"' exists,"
    echo "  -> unpacking library file"
    tar -xzf ${libsfile}
#    check_md5 "CRDFILE" "LIBS" ${crdfmd5s}
#    rm ${cardfile}
    check_md5 "CRDS"    "LIBS" ${crdsmd5s}
  else
    echo " <E> no library file found"
    echo "  -> stopping..."
    exit
  fi
fi

### check, whether only events have to be generated
if [ "${lbo}" = "EVTS" ]; then
  if [ -e ${crssfile} ]; then
    echo " <I> cross section file '"${crssfile}"' exists,"
    echo "  -> unpacking cross section file"
    tar -xzf ${crssfile}
    check_md5 "CRDFILE" "CRSS" ${crdfmd5s}
    check_md5 "CRDS"    "CRSS" ${crdsmd5s}
    check_md5 "LIBFILE" "CRSS" ${libfmd5s}
    check_md5 "LIBS"    "CRSS" ${libsmd5s}
  else
    echo " <E> no cross section file found"
    echo "  -> stopping..."
    exit
  fi
fi



### generate process-specific libraries -> redirect output (stdout, stderr) to files
## first pass
sherpaexe=`find ${shr} -type f -name Sherpa`
echo " <I> Sherpa executable is "${sherpaexe}
cd ${shrun}
if [ "${lbo}" = "LIBS" ] || [ "${lbo}" = "LBCR" ]; then
  echo " <I> creating library code..."
  ${sherpaexe} "PATH="${pth} "PATH_PIECE="${pth} "RESULT_DIRECTORY="${dir2} 1>${shrun}/${outflbs}_pass1.out 2>${shrun}/${outflbs}_pass1.err #Sherpa 1.3.0 needs full path MN 070611
  cd ${pth}
  if [ "${FLGCOMIX}" == "FALSE" ]; then
  testfile=`find ${shr} -type f -name libSherpaMain.so.0.0.0`
  echo "testfile: "${testfile}
  testbit=`echo ${testfile} | grep -i -c "ELF 32"`
  echo "testbit: "${testbit}
  if [ ${testbit} -ge 1 ]; then
##  cp ${shr}/share/SHERPA-MC/makelibs .
    fix_makelibs
    fi
    echo " <I> compiling libraries..."
    ./makelibs 1>${shrun}/${outflbs}_mklib.out 2>${shrun}/${outflbs}_mklib.err
    nf=`du -sh | grep -o "\." | grep -c "\."`
    lsize=`du -sh  | cut -f 1-${nf} -d "."`
    echo " <I>  -> raw size: "${lsize}
    echo " <I> cleaning libraries..."
    clean_libs 1>${shrun}/${outflbs}_cllib.out 2>${shrun}/${outflbs}_cllib.err
  fi
  nf=`du -sh | grep -o "\." | grep -c "\."`
  lsize=`du -sh  | cut -f 1-${nf} -d "."`
  echo " <I>  -> clean size: "${lsize}
  cd ${shrun}
fi

if [ "${FLGCOMIX}" == "FALSE" ]; then
## second pass (save integration results)
  if [ "${lbo}" = "LBCR" ] || [ "${lbo}" = "CRSS" ]; then
    echo " <I> calculating cross sections..."
    ${sherpaexe} "PATH="${pth} "PATH_PIECE="${pth} "RESULT_DIRECTORY="${dir2} 1>${shrun}/${outflbs}_pass2.out 2>${shrun}/${outflbs}_pass2.err #Sherpa 1.3.0 needs full path MN 070611
#    ${sherpaexe} -p ${pth} -g -r ${pth}/${dir2} 1>${shrun}/${outflbs}_pass2.out 2>${shrun}/${outflbs}_pass2.err
    if [ -d ${dir3} ]; then
      mv ${dir3} ${pth}/
    else
      mkdir ${pth}/${dir3}
    fi
  fi
## third pass (event generation)
  if [ "${lbo}" = "EVTS" ]; then
    echo " <I> generating events..."
    ${sherpaexe} "PATH="${pth} "PATH_PIECE="${pth} "RESULT_DIRECTORY="${dir2} 1>${shrun}/${outflbs}_pass3.out 2>${shrun}/${outflbs}_pass3.err #Sherpa 1.3.0 needs full path MN 070611
  fi
fi


## generate tar balls with data cards, libraries, cross sections, events
cd ${pth} ##Sherpa 1.3.0 needs full path MN 070611
#MM() cd ${pth}

#if [ "${lbo}" = "LBCR" ] || [ "${lbo}" = "CRSS" ]; then
#  mv ../Weights*.*      ./${dir2}/
#  mv ../xsections_*.dat ./${dir2}/
#fi

## libraries & cross sections
if [ "${lbo}" = "LIBS" ] || [ "${lbo}" = "LBCR" ]; then
  find ./${dir1}/ -type f -name '*' -exec md5sum {} \; > ${libsmd5s}
  touch ${libsfile}.tmp
  find ./         -type f -name '*.md5' > tmp.lst && tar --no-recursion -rf ${libsfile}.tmp -T tmp.lst; rm tmp.lst
##  find ./${dir1}/ -type f -name '*'     > tmp.lst && tar --no-recursion -rf ${libsfile}.tmp -T tmp.lst; rm tmp.lst
  find ./${dir1}/ -name '*'     > tmp.lst && tar --no-recursion -rf ${libsfile}.tmp -T tmp.lst; rm tmp.lst
  gzip -9 ${libsfile}.tmp && mv ${libsfile}.tmp.gz ${libsfile}
  md5sum ${libsfile} > ${libfmd5s}
  mv ${libsfile} ${shrun}/
fi

if [ "${lbo}" = "LBCR" ] || [ "${lbo}" = "CRSS" ]; then
  if [ -e ${libsfile} ]; then
    find ./${dir1}/ -type f -name '*' -exec md5sum {} \; > ${libsmd5s}
    md5sum ${libsfile} > ${libfmd5s}
  fi
  find ./${dir2}/ -type f -name '*' -exec md5sum {} \; > ${crssmd5s}
  touch ${crssfile}.tmp
  find ./         -type f -name '*.md5' > tmp.lst && tar --no-recursion -rf ${crssfile}.tmp -T tmp.lst; rm tmp.lst
##  find ./${dir2}/ -type f -name '*'     > tmp.lst && tar --no-recursion -rf ${crssfile}.tmp -T tmp.lst; rm tmp.lst
  find ./${dir2}/ -name '*'     > tmp.lst && tar --no-recursion -rf ${crssfile}.tmp -T tmp.lst; rm tmp.lst
  if [ -e ${dir3} ]; then
##  find ./${dir3}/ -type f -name '*'     > tmp.lst && tar --no-recursion -rf ${crssfile}.tmp -T tmp.lst; rm tmp.lst
  find ./${dir3}/ -name '*'     > tmp.lst && tar --no-recursion -rf ${crssfile}.tmp -T tmp.lst; rm tmp.lst
  fi
  gzip -9 ${crssfile}.tmp && mv ${crssfile}.tmp.gz ${crssfile}
  md5sum ${crssfile} > ${crsfmd5s}
  mv ${crssfile} ${shrun}/
fi

if [ "${lbo}" = "EVTS" ]; then
  find ./${dir1}/ -type f -name '*' -exec md5sum {} \; > ${libsmd5s}
  md5sum ${libsfile} > ${libfmd5s}
  rm ${libsfile}
  find ./${dir2}/ -type f -name '*' -exec md5sum {} \; > ${crssmd5s}
  md5sum ${crssfile} > ${crsfmd5s}
  rm ${crssfile}
  md5sum *.*         > ${evtsmd5s}
  tar -czf ${evtsfile} *.*
  md5sum ${evtsfile} > ${evtfmd5s}
  mv ${evtsfile} ${shrun}/
fi
#rm -rf ${dir1}/*
#rm -rf ${dir2}/*
#rm -rf ${dir3}/*
#rm *.md5

## data cards
touch ${crdsmd5s}
for FILE in `ls *.dat *slha.out 2> /dev/null`; do
  md5sum ${FILE} >> ${crdsmd5s}
done
FILES=`ls *.md5 *.dat *slha.out 2> /dev/null`
if [ "${lbo}" = "LIBS" ]; then
#  tar -czf ${crdlfile} ${FILES}
#  md5sum ${crdlfile} > ${crdfmd5s}
  tar -czf ${crdcfile} ${FILES}
  md5sum ${crdcfile} > ${crdfmd5s}
  mv ${crdcfile} ${shrun}/
elif [ "${lbo}" = "LBCR" ]; then
#  tar -czf ${crdlfile} ${FILES}
#  md5sum ${crdlfile} > ${crdfmd5s}
#  sed -e 's:MI_HANDLER.*:MI_HANDLER   = Amisic                ! Amisic / None:' < Run.dat > Run.dat.tmp
#  mv Run.dat.tmp Run.dat
  tar -czf ${crdefile} ${FILES}
  md5sum ${crdefile} > ${evtfmd5s}
  mv ${crdefile} ${shrun}/
elif [ "${lbo}" = "CRSS" ]; then
#  tar -czf ${crdcfile} ${FILES}
#  md5sum ${crdcfile} > ${crdfmd5s}
#  sed -e 's:MI_HANDLER.*:MI_HANDLER   = Amisic                ! Amisic / None:' < Run.dat > Run.dat.tmp
#  mv Run.dat.tmp Run.dat
  tar -czf ${crdefile} ${FILES}
  md5sum ${crdefile} > ${evtfmd5s}
  mv ${crdefile} ${shrun}/
elif [ "${lbo}" = "EVTS" ]; then
  echo evts
#  tar -czf ${crdefile} ${FILES}
#  md5sum ${crdefile} > ${evtfmd5s}
fi

## log files
cd ${shrun}
FILES=`ls *.err *.out 2> /dev/null`
if [ "${lbo}" = "LIBS" ]; then
  tar -czf ${loglfile} ${FILES}
elif [ "${lbo}" = "LBCR" ]; then
  tar -czf ${loglfile} ${FILES}
elif [ "${lbo}" = "CRSS" ]; then
  tar -czf ${logcfile} ${FILES}
elif [ "${lbo}" = "EVTS" ]; then
  tar -czf ${logefile} ${FILES}
fi
#rm *.err *.out
mv *.tgz ${fin}/


# go back to original directory
cd ${HDIR}
rm -rf ./SHERPATMP
