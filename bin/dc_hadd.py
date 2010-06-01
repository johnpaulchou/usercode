#!/bin/env python

import sys, os, fnmatch

# need the directory
if len(sys.argv)<=2:
    print 'Usage: %s outfile files' % os.path.basename(sys.argv[0])
    sys.exit(1)

outfile = sys.argv[1]
path = os.path.realpath(sys.argv[2])
newpath = path
splitout = os.path.splitext(outfile)

# fix the path
if path.find('/pnfs/cms/WAX/11/store/')==0:
    newpath = path.replace('/pnfs/cms/WAX/11/store/', 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/')

# get the files
cntr = 0
command = 'hadd %s ' % outfile
filelist = ''
for file in os.listdir(path):
    if fnmatch.fnmatch(file, '*.root'):
        filelist = filelist + newpath + '/' + file + ' '
    if cntr%50==0 and cntr!=0:
        command = 'hadd ' + splitout[0] + '_' + str(cntr/50) + splitout[1] + ' ' + filelist
        os.system(command)
        filelist =''
    cntr = cntr+1
if filelist != '':
    command = 'hadd ' + splitout[0] + '_' + str(cntr/50) + splitout[1] + ' ' + filelist
    os.system(command)
