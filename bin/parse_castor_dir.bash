#!/bin/bash -f
dir=$1
if [ $# == 0 ]
then
  echo "Usage: parse_castor_dir.bash directory [numfiles]"
elif [ $# == 1 ]
then
  rfdir ${dir} | awk "{printf \"rfio:%s%s \", \"${dir}\", \$9}"
  printf "\n"
else
  count=$2
  rfdir ${dir} | awk "{if(++cnt<=${count}) printf \"rfio:%s%s \", \"${dir}\", \$9}"
  printf "\n"
fi

