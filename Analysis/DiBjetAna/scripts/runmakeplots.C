{
  TString base=getenv("CMSSW_BASE");
  TString arch=getenv("SCRAM_ARCH");
  gROOT->ProcessLine(TString(".L ")+base+"/lib/"+arch+"/libAnalysisDataFormats.so");
  gROOT->ProcessLine(".x makeplots.C+");
}
