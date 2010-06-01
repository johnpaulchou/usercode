#!/bin/env python

import sys, os

# need two arguments
if len(sys.argv)<2:
    print 'Usage: %s pythonfile' % os.path.basename(sys.argv[0])
    print 'Returns status=1 on bad exit'
    sys.exit(1)

# check that the library is found
libfile = sys.argv[1]
if not os.path.isfile(libfile):
    print 'File %s could not be found' % libfile
    sys.exit(1)

# add library directory to the path
sys.path.append(os.path.dirname(libfile))

# load library
libsplit = os.path.splitext(os.path.basename(libfile))
lib=__import__(libsplit[0])

# print out the output filename
if ('process' in dir(lib) and 'output' in dir(lib.process) and 'fileName' in dir(lib.process.output)):
    print lib.process.output.fileName.value()
    sys.exit(0)
else:
    print "Could not find process.output.fileName in %s" %libfile
    sys.exit(1)
