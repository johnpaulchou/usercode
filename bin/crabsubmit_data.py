#!/bin/env python

""" This script is a wrapper to submit crab jobs.
    The script takes the name of a pythonfile which is normally called by cmsRun,
    and submits a crab job.
    """

import sys, os, getopt, subprocess, tempfile

class parseopt:
    def __init__(self, argv=None):
        self.basename = os.path.basename(sys.argv[0])
        self.status = True
        self.nlumis=-1
        self.minrun=0
        self.maxrun=99999999
        self.dataset=""
        self.outputfile=""
        self.pythonfile=""
        self.jsonfile=""
        self.lumisperjob=10000
        self.outputdirectory=""
        self.username="johnpaul"
        self.email="johnpaul@fnal.gov"
        self.crabdirbase="/uscms_data/d1/"+self.username+"/crab/"
        self.storageelement="cmssrm.fnal.gov"
        self.storagepath="/srm/managerv2?SFN=/11/store/user/"+self.username+"/"
        self.dbsurl=""
        self.scheduler="condor"

        if argv is None:
            argv = sys.argv[1:]

        # parse command line options
        try:
            opts, args = getopt.getopt(argv, 'hn:m:M:f:d:j:s:u:',['help','nlumis=','minrun=','maxrun=','outputfile=','dataset=','jsonfile=',"scheduler=","dbsurl="])

        # find exceptions
        except getopt.GetoptError, err:
            # print help information and exit:
            print str(err) # will print something like "option -a not recognized"
            print ""
            self.usage()
            self.status=False
            return

        # loop over options
        for o, a in opts:
            if o=="-h" or o=="--help":
                self.usagefull()
                self.status=False
                return
            elif o=="-n" or o=="--nlumis":
                try:
                    self.nlumis=int(a)
                except:
                    print "`%s\' is an invalid value for option %s: must be an integer" % (a, o)
                    self.status=False
                    return
            elif o=="-m" or o=="--minrun":
                try:
                    self.minrun=int(a)
                except:
                    print "`%s\' is an invalid value for option %s: must be an integer" % (a, o)
                    self.status=False                    
                    return
            elif o=="-M" or o=="--maxrun":
                try:
                    self.maxrun=int(a)
                except:
                    print "`%s\' is an invalid value for option %s: must be an integer" % (a, o)
                    self.status=False                    
                    return
            elif o=="-f" or o=="--outputfile":
                self.outputfile=a
            elif o=="-d" or o=="--dataset":
                self.dataset=a
            elif o=='-j' or o=='--jsonfile':
                self.jsonfile=a
            elif o=="-s" or o=="--scheduler":
                self.scheduler=a
            elif o=="-u" or o=="--dbsurl":
                self.dbsurl=a

        # make sure minrun and maxrun are consistent
        if self.minrun>self.maxrun:
            print "minrun (%d) cannot be more than maxrun (%d)" % (self.minrun, self.maxrun)
            self.usage()
            self.status=False
            return
                
        # we need at least 3 arguments
        if len(args)<3:
            self.usage()
            self.status=False
            return

        self.pythonfile = args[0]
        try:
            self.lumisperjob = int(args[1])
        except:
            print "`%s\' is an invalid value for lumis_per_job argument: must be an integer" % args[1]
            self.status=False
            return
        self.outputdirectory = args[2]

    def usage(self):
        print "Usage: %s [options] pythonfile lumis_per_job output_dir" % self.basename
        print "Try `%s --help\' for more information" % self.basename

    def usagefull(self):
        print "Usage: %s [options] pythonfile lumis_per_job output_dir" % self.basename
        print """
ARGUMENTS
       pythonfile
              The file to run over.  This is the file typically called with cmsRun.

       lumis_per_job
              The number of luminosity sections to run over per job.

       output_dir
              Output directory.

OPTIONS
       -h, --help
              Prints this usage message.
     
       -n NUM, --nlumis=NUM
              Number of luminosity sections to run over.  By default, this is set to -1 (i.e. run over all available events).
    
       -m NUM, --minrun=NUM
              Minimum run number to traverse over.  By default, this is set to 0.

       -M NUM, --maxrun=NUM
              Maximum run number to traverse over.  By default, this is set to 99999999.

       -d ARG, --dataset=ARG
              Dataset path name, as specified in DBS.  The script  attempts to parse the pythonfile to determine this automa-
              tically, looking for dataset.dataset.  If it cannot find this, it needs to be overwritten here.

       -f ARG, --outputfile=ARG
              Output filename.  Normally, the script attempts to parse the pythonfile to determine this automatically, but it
              can be overwritten here.  Use `none' if no output is given.

       -j ARG, --jsonfile=ARG
              JSON filename.  In normal use, the script attempts to parse the pythonfile to determine this automatically, but
              it can be overwritten here.  Use `none' if no JSON is given.

       -s ARG, --scheduler=ARG
              Scheduler used to submit  jobs (e.g., condor is a local  scheduler; glite is a global  scheduler).  By default,
              condor is used.

       -u ARG, --dbsurl=ARG
              DBS URL for the dataset.  The script attempts  to parse the pythonfile to determine this automatically, looking
              for dataset.dbs_url.  If it cannot find this, the default global dbsurl is used.
                     
"""

    def dump(self):
        print "basename=%s" % self.basename
        print "status=%d" % self.status
        print "nlumis=%d" % self.nlumis
        print "minrun=%d" % self.minrun
        print "maxrun=%d" % self.maxrun
        print "dataset=%s" % self.dataset
        print "outputfile=%s" % self.outputfile
        print "jsonfile=%s" % self.jsonfile
        print "pythonfile=%s" % self.pythonfile
        print "lumisperjob=%d" % self.lumisperjob
        print "outputdirectory=%s" % self.outputdirectory
        print "username=%s" % self.username
        print "email=%s" % self.email
        print "crabdirbase=%s" % self.crabdirbase
        print "storageelement=%s" % self.storageelement
        print "storagepath=%s" % self.storagepath
        print "schedulers=%s" % self.scheduler
        print "dbsurl=%s" % self.dbsurl

class parsecfg:
    def __init__(self, pythonfile):
        self.pythonfile = pythonfile
        self.outputfile = ""
        self.dataset = ""
        self.jsonfile = ""
        self.dbsurl = ""
        self.status = True

        # check that the pythonfile file exists
        if not os.path.isfile(pythonfile):
            print "File %s could not be found" % self.pythonfile
            self.status = False
            return

        # add python file directory to the path
        sys.path.append(os.path.dirname(self.pythonfile))

        # load the python file as a library
        self.libsplit = os.path.splitext(os.path.basename(self.pythonfile))
        try:
            self.library=__import__(self.libsplit[0])
        except:
            print "Could not load `%s\' as a library.  Did you run cmsenv?" % self.pythonfile
            self.status = False
            return

        # get the output filename
        try:
            for module in self.library.process._Process__outputmodules.values():
                self.outputfile = module.fileName.value()
        except:
            return

        # get the dataset name
        try:
            self.dataset=self.library.dataset.dataset
        except:
            self.dataset=""
            return

        # get the JSON file name
        try:
            self.jsonfile=self.library.dataset.JSON
        except:
            self.jsonfile=""
            return

        # get the dbs url name
        try:
            self.dbsurl=self.library.dataset.dbs_url
        except:
            self.dbsurl=""
            return



def makecrabconfig(options):
    configfileoutput = "[CRAB]\n"+ \
                       "jobtype                = cmssw\n" + \
                       "scheduler              = "+options.scheduler+ \
                       "\n\n[CMSSW]\n"

    if options.dbsurl!="":
        configfileoutput = configfileoutput+"dbs_url                = """+options.dbsurl+"\n"
    configfileoutput = configfileoutput \
    +"datasetpath            = "+options.dataset+"\n" \
    +"pset                   = "+options.pythonfile+"\n" \
    +"output_file            = "+options.outputfile+"\n" \
    +"total_number_of_lumis  = " + str(options.nlumis)+"\n" \
    +"lumis_per_job          = " + str(options.lumisperjob)+"\n" \
    +"get_edm_output         = 1\n"

    if options.minrun==options.maxrun:
        configfileoutput = configfileoutput + "runselection           = " + str(options.minrun) + "\n"
    elif options.minrun>0 or options.maxrun<99999999:
        configfileoutput = configfileoutput + "runselection           = " + str(options.minrun) + "-" + str(options.maxrun) + "\n"        

    if options.jsonfile!="":
        configfileoutput = configfileoutput + "lumi_mask              = " + options.jsonfile + "\n"

    configfileoutput = configfileoutput \
    + "\n[USER]\n" \
    + "return_data            = 0\n" \
    + "copy_data              = 1\n" \
    + "storage_element        = "+options.storageelement+"\n" \
    + "storage_path           = "+options.storagepath+"\n" \
    + "user_remote_dir        = "+options.outputdirectory+"\n" \
    + "ui_working_dir         = "+options.crabdirbase+options.outputdirectory+"\n" \
    + "srm_version            = 2\n" \
    + " use_central_bossDB    = 0\n" \
    + "use_boss_rt            = 0\n" \
    + "check_user_remote_dir  = 0\n" \
    + "thresholdLevel         = 100\n" \
    + "eMail                  = "+options.email+"\n" \
    + "\n[GRID]\n" \
    + "rb                     = CERN\n" \
    + "proxy_server           = myproxy.cern.ch\n" \
    + "virtual_organization   = cms\n" \
    + "retry_count            = 2\n" \
    + "lcg_catalog_type       = lfc\n" \
    + "lfc_host               = lfc-cms-test.cern.ch\n" \
    + "lfc_home               = /grid/cms\n"

    # create the configfile
    tmpconfigfile = tempfile.NamedTemporaryFile('wt')
    tmpconfigfile.write(configfileoutput)
    tmpconfigfile.flush()
                       
    scriptfileoutput = """#!/bin/bash

source /uscmst1/prod/grid/gLite_SL5_CRAB_27x.sh

# setup crab
source /uscmst1/prod/grid/CRAB/crab.sh

crab -create -cfg """+tmpconfigfile.name+"""
crab -submit -c """+options.crabdirbase+options.outputdirectory

    tmpscriptfile = tempfile.NamedTemporaryFile('wt')
    tmpscriptfile.write(scriptfileoutput)
    tmpscriptfile.flush()

    print "******************************************************"
    print "This is the config file we are going to run:"
    print configfileoutput
    print "******************************************************"
    print "This is the script we are going to run:"
    print scriptfileoutput
    print "******************************************************"

#    proc = subprocess.call(['cat %s' % tmpconfigfile.name], shell=True, close_fds=True)
    proc = subprocess.call(['bash %s' % tmpscriptfile.name], shell=True, close_fds=True)


if __name__ == "__main__":
    options = parseopt(sys.argv[1:])
    
    if options.status:
        config = parsecfg(options.pythonfile)
        if config.status:
            if config.outputfile!="" and options.outputfile=="":
                options.outputfile=config.outputfile
                print "Using the outputfile `%s\' found in the config file `%s\'" % (options.outputfile, options.pythonfile)
            elif options.outputfile=="":
                print "No outputfile was specified."

            if config.dataset!="" and options.dataset=="":
                options.dataset=config.dataset
                print "using the dataset `%s\' found in the config file `%s'\'" % (options.dataset, options.pythonfile)
            elif options.dataset=="":
                print "No dataset to run over was specified."

            if config.dbsurl!="" and options.dbsurl=="":
                options.dbsurl=config.dbsurl
                print "using the dbsurl `%s\' found in the config file `%s'\'" % (options.dbsurl, options.pythonfile)
            elif options.dbsurl=="":
                print "No dbsurl was specified."

            if config.jsonfile!="" and options.jsonfile=="":
                options.jsonfile=config.jsonfile
                print "using the JSON file `%s\' found in the config file `%s'\'" % (options.jsonfile, options.pythonfile)
            elif options.jsonfile=="":
                print "No JSON file was specified."

            makecrabconfig(options)
        
