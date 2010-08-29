#!/bin/env python

import sys, os

if len(sys.argv)<=1:
    print 'Usage: %s directory numfiles' % os.path.basename(sys.argv[0])
    sys.exit(1)

# fix the path
path = os.path.realpath(sys.argv[1])
if path.find('/pnfs/cms/WAX/11/store/')==0:
    newpath = path.replace('/pnfs/cms/WAX/11/store/', 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/')
else:
    print '`%s\' is not a valid pnfs directory' % path
    sys.exit(1)


# get all of the files in that directory

files=os.listdir(path)
files=[filename for filename in files if filename[0] != '.' and os.path.isfile(path+'/'+filename)]

cnt=0
for file in files:
    if len(sys.argv)>=3 and cnt>=int(sys.argv[2]):
        break
    cnt=cnt+1
    print '%s' % (newpath+'/'+file),
