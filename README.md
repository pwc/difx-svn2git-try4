# DiFX

[![Build Status](https://travis-ci.com/difx/difx-svn2git-try4.svg?branch=master)](https://travis-ci.com/difx/difx-svn2git-try4) [Branch build status](https://travis-ci.com/github/difx/difx-svn2git-try4/branches)

## A note about file history

This repo has a lot of files which have been renamed over the years. To see
the full history of a file through all of the renames, add `--follow` to the
`git log` command:

`$ git log -p --follow README.md`

The above log also illustrates a cosmetic problem we have with file history
thanks to the svn-to-git conversion in 2020: some svn branch creation events
have caused a large number of spurious file deletions in individual file
histories. No content has been lost.

## Installation

To install the latest trunk version of a clean SVN checkout of DiFX, follow
these simple instructions:

1) Ensure you have a working installation of IPP and MPI
2) cd setup
3) Open setup.bash or setup.csh in your favourite text editor and change the 
DIXFROOT, IPPROOT MPICXX and PGPLOT paths and the perl versions to suit your system
4) source setup/setup.bash (or .csh). You will probably want to add
this to your .bashrc or .cshrc files
5) Run "./install-difx

Thats it. DiFX should compile - if not, check out the troubleshooting
area on http://www.atnf.csiro.au/vlbi/dokuwiki/doku.php/difx/start

## Testing

See http://www.atnf.csiro.au/dokuwiki/doku.php/difx/benchmarks
