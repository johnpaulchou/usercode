#!/bin/env python

import sys, os, getopt

class parseopt:
    def __init__(self, argv=None):
        self.basename = os.path.basename(sys.argv[0])
        self.status = True
        self.dirmode = False
        self.filemode = False
        self.numfiles = -1
        self.files=[]
        self.linebreaks = False

        # use the system arguments if no argument is passed
        if argv is None:
            argv = sys.argv[1:]
            
        # parse command line arguments
        try:
            opts, args = getopt.getopt(argv, 'hdfn:l',['help','dirmode','filemode','numfiles=','linebreaks'])
                
        except getopt.GetoptError, err:
            print "%s: %s" % (self.basename, str(err))
            print "Try `%s --help' for more information" % self.basename
            self.status=False
            return
        
        # loop over options
        for o, a in opts:
            if o=="-h" or o=="--help":
                self.usage()
                self.status=False
                return
            elif o=="-d" or o=="--dirmode":
                self.dirmode=True
            elif o=="-f" or o=="--filemode":
                self.filemode=True
            elif o=="-n" or o=="--numfiles":
                try:
                    self.numfiles = int(a)
                except:
                    print "%s: `%s\' is an invalid value for option %s: must be an integer" % (self.basename, a, o)
                    print "Try `%s --help' for more information" % self.basename
                    self.status=False
                    return
            elif o=="-l" or o=="--linebreaks":
                self.linebreaks=True
                
        # make sure arguments are consistent
        if self.dirmode==self.filemode:
            print "%s: Must choose either dirmode or filemode, but not both" % self.basename
            print "Try `%s --help' for more information" % self.basename
            self.status=False
            return

        # we need at least one argument
        if len(args)<1:
            print "%s: We need at least one directory or file to parse" % self.basename
            print "Try `%s --help' for more information" % self.basename
            self.status=False
            return

        # ok, we're ready
        self.files = args

    def usage(self):
        print "Usage: %s <mode> [options] files/directory" %self.basename
        print ""
        print "Description"
        print "    Converts files in a pnfs directory into a root readable list"
        print ""
        print "Operations"
        print "    -d, --dirmode"
        print "        Directory mode.  The second argument must be a directory."
        print ""
        print "    -f, --filemode"
        print "        File mode.  The second argument(s) must be file(s)."
        print ""
        print "Options"
        print "    -h, --help"
        print "        Prints this message"
        print ""
        print "    -n NUM, --numfiles=NUM"
        print "        Specifies the maximum number of files to convert.  A value <0 is equivalent"
        print "        to writing all files."
        print ""
        print "    -l, --linebreaks"
        print "        Prints each file one line at a time, rather than space deliminated"
        print ""


if __name__=="__main__":
    options = parseopt(sys.argv[1:])

    # we're in directory mode
    if options.status and options.dirmode:
        path = os.path.realpath(options.files[0])
        if path.find('/pnfs/cms/WAX/11/store/')==0:
            newpath = path.replace('/pnfs/cms/WAX/11/store/', 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/')
        else:
            print '%s: `%s\' is not a valid pnfs directory' % (options.basename, path)
            sys.exit(1)
        
        # get all of the files in that directory
        try:
            files=os.listdir(path)
        except:
            print '%s: `%s\' is not a valid pnfs directory' % (options.basename, path)
            sys.exit(1)
        files=[filename for filename in files if filename[0] != '.' and os.path.isfile(path+'/'+filename)]
        cnt=0
        for file in files:
            if options.numfiles>=0 and cnt>=options.numfiles:
                break
            cnt=cnt+1
            if options.linebreaks:
                print '%s' % (newpath+'/'+file)
            else:
                print '%s' % (newpath+'/'+file),

    # we're in file mode
    elif options.status and options.filemode:
        cnt=0
        for file in options.files:
            if options.numfiles>=0 and cnt>=options.numfiles:
                break
            cnt=cnt+1
            path = os.path.realpath(file)
            if path.find('/pnfs/cms/WAX/11/store/')==0:
                newfile = path.replace('/pnfs/cms/WAX/11/store/', 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11/store/')
            else:
                print '%s: `%s\' is not in a valid pnfs directory' % (options.basename, path)
                sys.exit(1)
            if options.linebreaks:
                print '%s' % newfile
            else:
                print '%s' % newfile,
