#!/usr/bin/python
# take care of a few logistics before launching the correlator
# Cormac Reynolds: June 2010

import subprocess, optparse, re, shutil, os, sys, time, fileinput, pprint

def run_vex2difx(v2dfilename, vex2difx_options):
    # run vex2difx, and wait for completion
    command = "vex2difx " + vex2difx_options + ' ' + v2dfilename
    print command
    subprocess.check_call( command, stdout=sys.stdout, shell=True)

def change_path(inputfilename, changeparm, oldpath, newpath):
    # modify paths in the input file
    for line in fileinput.FileInput(inputfilename, inplace=1, backup='.org'):
        if re.match(changeparm, line):
            line = line.replace(oldpath, newpath)
        print line,

    fileinput.close()

def run_calcif2(jobname, calcfilename):
    # tidy up old calcif2 files and run calcif2 again
    calcoutputfiles = ['.uvw', '.rate', '.im', '.delay']
    for file in calcoutputfiles:
        # clear up old calc files so we are sure it completed
        file = jobname + file
        if os.path.exists(file):
            os.remove(file)
    command = 'calcif2 ' + calcfilename
    print command
    subprocess.check_call(command, stdout=sys.stdout, shell=True)

def backup_oldrun(jobname, outdir):
    # back up previous correlator job to subdirectory named with current time
    backupdir = outdir + time.strftime('%Y-%m-%d-%H-%M-%S') + os.sep
    if os.path.exists(outdir):
        print "\nwill move old jobs to", backupdir, '\n'
        dirlist = os.listdir(outdir)
        for file in dirlist:
            if jobname in file and not re.match('\.', file):
                os.renames(outdir + file, backupdir + file)

def copy_models(jobname, indir, outdir):
    # copy all files with the jobname in their name to the output directory.
    # This will include all the files needed to run difx2fits, and other
    # interesting logs.
    dirlist = os.listdir(indir)
    for file in dirlist:
        if jobname + '.' in file and not re.match('\.', file):
            if os.path.isfile(file):
                #print "copy", file, outdir
                shutil.copy2(indir + file, outdir)

def copy_jobcontrol(expname, jobname, indir, outdir):
    # copy the vex, v2d and joblist files to the output directory for archiving
    # purposes. Rename to match the jobname.
    extensions = ['.vex', '.v2d', '.joblist']
    for extension in extensions:
        infile = indir + expname + extension
        outfile = outdir + jobname + extension
        if os.path.isfile(infile):
            shutil.copy2(infile, outfile)
        else:
            sys.stderr.write(infile + ' not found!')

def make_new_runfiles(jobname):
    # make copies of the prototype run and .thread files
    runfile = 'run_' + jobname
    machinesfilename = jobname + '.machines'
    shutil.copy('run', runfile)
    for line in fileinput.FileInput(runfile, inplace=1):
        if '_1.input' in line:
            line = re.sub(r'\S*_1.input', jobname + '.input', line)
            line = re.sub(r'machines.list', machinesfilename, line)
        print line,
    fileinput.close()
    os.chmod(runfile, 0775)

    threadfilename = jobname + '.threads'
    shutil.copy(expname + '.threads', threadfilename)

    shutil.copy(expname + '.machines', machinesfilename)

def parse_joblistfile(joblistfilename):
    # get the full list of jobs from the joblist file
    # Return a dictionary. Keys are the job names. Values are the stations in
    # the job
    joblistfile = open(joblistfilename).readlines()

    joblistfile.pop(0)
    joblist = dict()
    for line in joblistfile:
        jobname = line.split()[0]
        stations = re.search(r'#\s+(.*)', line).group(1)
        joblist[jobname] = stations

    return joblist

def run_lbafilecheck(datafilename, stations, computehead):
    # run lbafilecheck creating machines and .threads files for this job
    stations = stations.strip()
    stations = re.sub(r'\s+', ',', stations)
    stations = "'" + stations + "'"
    options = ''
    if computehead:
        options += ' -H '
    command = "lbafilecheck.py -F -s " + options + stations + " " + datafilename
    print command
    subprocess.check_call( command, stdout=sys.stdout, shell=True)

def fill_operator_log(logfile):
    # Fire up an editor for operator comments
    editor = os.environ.get('EDITOR')
    if not editor:
        editor = 'vim'
    command = editor + ' ' + logfile
    subprocess.check_call( command, stdout=sys.stdout, shell=True)


# Main program start.
#parse the options
usage = '''%prog <jobname>
    will:
    run vex2difx 
    correct the output file name 
    run calcif2
    move previous correlator job to backup directory
    copy model information to correlator data area
    start errormon2
    start the correlator!
    quit errormon2 and copy the log file to the output directory
    accept an operator comment for storing with the output
    
<jobname> may be a space separated list.
<jobname> may also include a python regular expression after the '_' in the job
name to match multiple jobs. The job name up to the '_' must be given
explicitly)'''


parser = optparse.OptionParser(usage=usage, version='%prog ' + '1.0')
parser.add_option( "--clock", "-c",
        dest="clockjob", action="store_true", default=False,
        help='Store output in a clock subdirectory. Also passes -f to vex2difx' )
parser.add_option( "--nocalc", "-C",
        dest="nocalc", action="store_true", default=False,
        help='Do not re-run calc' )
parser.add_option( "--novex", "-n",
        dest="novex", action="store_true", default=False,
        help='Do not re-run vex2difx' )
parser.add_option( "--nopause", "-p",
        dest="nopause", action="store_true", default=False,
        help='Do not pause after running calc - proceed straight to correlation' )
parser.add_option( "--alljobs", "-a",
        type='str', dest="expt_all", default=None,
        help='Correlate all jobs produced by vex2difx for the experiment specified (no other arguments required)')
parser.add_option( "--computehead", "-H",
        dest="computehead", action="store_true", default=False,
        help='Use head and datastream nodes as compute nodes' )

(options, args) = parser.parse_args()


if len(args) < 1 and not options.expt_all:
    parser.print_help()
    parser.error("Give job name(s)")

# preliminaries: clear old input files out of the way
if not options.novex:
    if options.expt_all:
        for filename in os.listdir(os.getcwd()):
            if re.match(options.expt_all +  r'_\d+\.input$', filename):
                os.rename(filename, filename + '.bak')

    for jobname in args:
        inputfilename = jobname + '.input'
        if os.path.exists(inputfilename):
            # clear old input file out of the way first
            # That way later parts will crash if vex2difx fails
            os.rename(inputfilename, inputfilename + '.bak')


vex2difx_options = ''
if options.clockjob:
    vex2difx_options = ' -f '

# Determine the experiment name from the first jobname or the -a switch
if options.expt_all:
    expname = options.expt_all
else:
    expname = re.match(r'(.*)_', args[0]).group(1)

# run vex2difx. 
v2dfilename = expname + '.v2d'

if not options.novex:
    run_vex2difx(v2dfilename, vex2difx_options)

joblistfilename = expname + '.joblist'
(fulljoblist) = parse_joblistfile(joblistfilename) 

# figure out the list of jobs to run this time
corrjoblist = dict()
# if the -a switch was used, then do all jobs
if options.expt_all:
    corrjoblist = fulljoblist
else:
    # if no -a, then match any patterns given on the command line
    for jobpattern in args:
        for jobname in fulljoblist.keys():
            if re.search(jobpattern + '$', jobname):
                corrjoblist[jobname] = fulljoblist[jobname]

print "job list to correlate = ", pprint.pformat(corrjoblist), "\n";
#print "stations = ", pprint.pformat(stations), "\n";

# create the mpi files for each job
for jobname in sorted(corrjoblist.keys()):
    # run lbafilecheck to get the new machines and .threads files
    datafilename = expname + '.datafiles'
    run_lbafilecheck(datafilename, corrjoblist[jobname], options.computehead)

    # duplicate the run and thread and machines files for the full number of
    # jobs
    print "\nduplicating the run file, machines file and .threads files for ", jobname, "\n"
    make_new_runfiles(jobname)


for jobname in sorted(corrjoblist.keys()):
    # figure out filenames, directories, etc. using normal difx/cuppa conventions
    indir = os.getcwd() + os.sep
    try:
        outdirbase = os.environ.get('CORR_DATA') + os.sep
    except:
        raise Exception('You must set $CORR_DATA to an output data directory!')


    outdir = outdirbase + expname + os.sep
    if options.clockjob:
        outdir += 'clocks/'

    inputfilename = jobname + '.input'
    calcfilename = jobname + '.calc' 

    if not os.path.exists(outdir):
        print "making the output directory", outdir
        os.makedirs(outdir)

    # fix the output filename to point at the cuppa data disk
    if not options.novex:
        print "\nrenaming the 'OUTPUT FILENAME' in", inputfilename, "from", indir, "to", outdir, "\n"
        change_path(inputfilename, 'OUTPUT FILENAME:', indir, outdir)

    # run calcif2
    if not options.nocalc:
        run_calcif2(jobname, calcfilename)

    # copy any old jobs to a backupdir
    backup_oldrun(jobname, outdir)

    # copy the model files to the output directory
    print "copying the model files", jobname + '.*', "to", outdir, "\n"
    copy_models(jobname, indir, outdir)

    # change the path names in the copied .input and .calc to relative paths
    print "changing absolute paths to relative paths in the copied .input and .calc files\n"
    copy_inputfilename = outdir + os.sep + inputfilename
    copy_calcfilename = outdir + os.sep + calcfilename
    change_path(copy_inputfilename, 'CALC FILENAME:', indir, './')
    change_path(copy_calcfilename, 'IM FILENAME:', indir, './')
    change_path(copy_calcfilename, 'FLAG FILENAME:', indir, './')

    # copy job control files to output directory, and rename
    print "copying the job control files", expname + '.[joblist|v2d|vex]', "to", outdir, "\n"
    copy_jobcontrol(expname, jobname, indir, outdir)

if not options.nopause:
    raw_input('Press return to initiate the correlator job or ^C to quit ')

try:
    for jobname in sorted(corrjoblist.keys()):
        # start the correlator log
        print "starting errormon2 in the background"
        errormon2 = subprocess.Popen('errormon2')

        try:
            runfile = jobname
            runfile = './run_' + runfile
            print "starting the correlator running", runfile
            subprocess.check_call(runfile, shell=True, stdout=sys.stdout)
        finally:
            # we're finished with the log...
            os.kill(errormon2.pid, 9)
            logfile = outdir + jobname + '.log'
            print "renaming the log file to", logfile
            shutil.copy2('log', logfile)
finally:
    # and enter an operator comment
    raw_input('\nHit return, then enter an operator comment, minimally: PROD/CLOCK/TEST/FAIL')
    operator_log = 'comment.txt'
    fill_operator_log( operator_log )
    for jobname in args:
        operator_joblog = outdir + jobname + '.comment.txt'
        shutil.copy2(operator_log, operator_joblog)
