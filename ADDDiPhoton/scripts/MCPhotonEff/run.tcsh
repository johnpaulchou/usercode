#!/bin/tcsh -f

foreach ana (1 2 3)
  setenv ANATYPE $ana
  root -b -q makeplots.C+
end

root drawplots.C
