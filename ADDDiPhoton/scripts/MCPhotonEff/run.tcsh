#!/bin/tcsh -f

setenv ANATYPE 1
root -b -q makeplots.C+

setenv ANATYPE 2
root -b -q makeplots.C+

root drawplots.C
