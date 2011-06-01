/* -*- mode: c++ -*- */

#include "RooWorkspace.h"
#include "RooHist.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooProdPdf.h"
#include "RooUniform.h"
#include "RooBinning.h"
#include "RooLognormal.h"
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooPlot.h"
#include "RooMinuit.h"

#include "RooStats/BayesianCalculator.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"

#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "RooFunctor.h"
#include "RooFunctor1DBinding.h"

#include "TStopwatch.h"
#include "TFile.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TUnixSystem.h"

#include <iostream>
#include <fstream>

#include "Analysis/Statistics/interface/DijetHelper.h"
#include "Analysis/Statistics/interface/LineShapeDensityPdf.h"
#include "Analysis/Statistics/interface/JPMCCalculator.h"
#include "Analysis/Statistics/interface/Qstar_qg_3.h"

using namespace RooFit;
using namespace RooStats;
using namespace DijetHelper;

int main(int argc, char* argv[])
{
  // this is needed to setup the auto-compile for the workspace
  gSystem->SetIncludePath("-I$ROOFITSYS/include");

  // Declare magic numbers here
  double MININVMASS=220.;
  double MAXINVMASS=3147.;
  std::string DATASETFN="/uscms/physics_grp/lpcjj/Dijet/7TeV/Sep3rd2010_2p875pbm1/dijet_mass_2p875pbm1.txt";
  double LUMI=2.875;     // total integrated luminosity
  double LUMIERROR=0.11; // relative error on luminosity
  double JES=1.0;        // JES "value"
  double JER=1.0;        // JER "value"
  double JESERROR=0.1;   // relative error on JES
  double JERERROR=0.1;   // relative error on JER
  double PD_NBKG=414131; // background normalization for pseudo-experiments
  double PD_P1=7.0655;   // parameter #1 for PEs
  double PD_P2=6.4766;   // parameter #2 for PEs
  double PD_P3=1.9936;   // parameter #3 for PEs
  double NSIGCUTOFF=3.0; // number of +/- "sigma" to cutoff integration of nuisance parameters

  bool USELOGNORM=true;  // use lognormal or gaussian nuisance prior pdfs
  
  // histogram binning (for display only)
  const int NBINS=45;
  Double_t BOUNDARIES[NBINS] = { 220, 244, 270, 296, 325, 354, 386, 419, 453,
				 489, 526, 565, 606, 649, 693, 740, 788, 838,
				 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383,
				 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132,
				 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147 };


  // start the timer
  TStopwatch t;
  t.Start();


  if(argc<2) {
    std::cout << "Usage: dijetStatsNewBackground signalMass statlevel [usePseudoData=false]" << std::endl;
    return -1;
  }

  double signalMass = atof(argv[1]);
  int statlevel = atoi(argv[2]);
  bool usePseudodata = argc>=4 ? atoi(argv[3]) : false;

  // setup label
  char label[100];
  sprintf(label, "%2.0f_%d", signalMass, statlevel);

  // setup workspace
  RooWorkspace *ws = new RooWorkspace("ws");

  // observable
  ws->factory("invmass[0]");
  RooRealVar* invmass=ws->var("invmass");
  invmass->setRange(MININVMASS, MAXINVMASS);

  // background
  ws->factory("EXPR::background('pow(1.0-invmass/7000.0,p1)/pow(invmass/7000.0,p2+p3*log(invmass/7000.0))', p1[10,-40,40], p2[7,-20,20], p3[0.1,-20,20],invmass)");
  ws->factory("EXPR::backgroundb('pow(1-invmass/7000.0+pb3*(invmass/7000.0)*(invmass/7000.0),pb1)/pow(invmass/7000,pb2)',pb1[20,0,100],pb2[5,-100,100], pb3[0.8,0,2],invmass)");
  ws->factory("EXPR::backgroundc('pow(1-invmass/7000.0,pc1)/pow(invmass/7000,pc2)',pc1[13,-100,100],pc2[5,-100,100],invmass)");
  // set the background to the seed (this will later get set by the fit)
  ws->var("p1")->setVal(PD_P1);
  ws->var("p2")->setVal(PD_P2);
  ws->var("p3")->setVal(PD_P3);

  // data
  RooDataHist* binnedData;
  RooDataSet* unbinnedData;
  unbinnedData=makeUnbinnedData(DATASETFN, "unbinnedData", invmass);
  if(usePseudodata)
    binnedData=makeBinnedPseudoData(ws->pdf("background"), PD_NBKG, "binnedPseudoData", invmass, MAXINVMASS-MININVMASS, MININVMASS, MAXINVMASS);
  else
    binnedData=makeBinnedData(DATASETFN, "binnedData", invmass, (MAXINVMASS-MININVMASS), MININVMASS, MAXINVMASS);
  ws->import(*binnedData);
  
  // signal rate
  if(USELOGNORM) ws->factory("RooLognormal::lumi_prior(lumi[0], lumiM0[0], lumiK0[1])");
  else           ws->factory("RooGaussian::lumi_prior(lumi[0], lumiM0[0], lumiK0[1])");
  ws->var("lumi")->setVal(LUMI);
  ws->var("lumi")->setRange(std::max(0.0, LUMI-NSIGCUTOFF*LUMI*LUMIERROR), LUMI+NSIGCUTOFF*LUMI*LUMIERROR);
  ws->var("lumi")->setConstant();
  ws->var("lumiM0")->setVal(LUMI);
  if(USELOGNORM) ws->var("lumiK0")->setVal(LUMIERROR+1);
  else           ws->var("lumiK0")->setVal(LUMI*LUMIERROR);
  ws->factory("RooUniform::xs_prior(xs[0])");
  ws->factory("prod::nsig(xs, lumi)");

  // signal nuisance parameters
  ws->factory("sigMass[1000]");
  ws->var("sigMass")->setVal(signalMass);
  if(USELOGNORM) ws->factory("RooLognormal::sigMassDelta_prior(sigMassDelta[0], sigMassDeltaM0[0], sigMassDeltaK0[1])");
  else           ws->factory("RooGaussian::sigMassDelta_prior(sigMassDelta[0], sigMassDeltaM0[0], sigMassDeltaK0[1])");
  if(USELOGNORM) ws->factory("RooLognormal::sigWidthDelta_prior(sigWidthDelta[0], sigWidthDeltaM0[0], sigWidthDeltaK0[1])");
  else           ws->factory("RooGaussian::sigWidthDelta_prior(sigWidthDelta[0], sigWidthDeltaM0[0], sigWidthDeltaK0[1])");
  ws->var("sigMassDelta")->setVal(JES);
  ws->var("sigWidthDelta")->setVal(JER);
  ws->var("sigMassDelta")->setRange(std::max(0.0, JES-NSIGCUTOFF*JESERROR), JES+NSIGCUTOFF*JESERROR);
  ws->var("sigWidthDelta")->setRange(std::max(0.0, JER-NSIGCUTOFF*JERERROR), JER+NSIGCUTOFF*JERERROR);
  ws->var("sigMassDeltaM0")->setVal(JES);
  ws->var("sigWidthDeltaM0")->setVal(JER);
  if(USELOGNORM) ws->var("sigMassDeltaK0")->setVal(JESERROR+1);
  else           ws->var("sigMassDeltaK0")->setVal(JESERROR);
  if(USELOGNORM) ws->var("sigWidthDeltaK0")->setVal(JERERROR+1);
  else           ws->var("sigWidthDeltaK0")->setVal(JERERROR);
  ws->var("sigMassDelta")->setConstant(true);
  ws->var("sigWidthDelta")->setConstant(true);

  // signal shape
  ws->factory("Qstar_qg_3::signal(invmass, sigMassDelta, sigWidthDelta, sigMass)");

  // model
  ws->factory("SUM::modela(nsig*signal, nbkg[1000,0,1E6]*background)");
  ws->var("nbkg")->setVal(PD_NBKG);
  ws->factory("SUM::modelb(nsig*signal, nbkg*backgroundb)");
  ws->factory("SUM::modelc(nsig*signal, nbkg*backgroundc)");
  ws->defineSet("observables","invmass");
  ws->defineSet("POI","xs");
  if(statlevel==0) ws->factory("PROD::prior(xs_prior)");
  if(statlevel==1) ws->factory("PROD::prior(xs_prior)");
  if(statlevel==2) ws->factory("PROD::prior(xs_prior,lumi_prior)");
  if(statlevel==3) ws->factory("PROD::prior(xs_prior,sigMassDelta_prior)");
  if(statlevel==4) ws->factory("PROD::prior(xs_prior,sigWidthDelta_prior)");
  if(statlevel==5) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior)");
  if(statlevel==6) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior,sigWidthDelta_prior)");
  if(statlevel==7) ws->factory("PROD::prior(xs_prior,sigMassDelta_prior)");
  if(statlevel==8) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior)");
  if(statlevel==9) ws->factory("PROD::prior(xs_prior,lumi_prior)");
  if(statlevel==0) ws->defineSet("nuisSet","");
  if(statlevel==1) ws->defineSet("nuisSet","");
  if(statlevel==2) ws->defineSet("nuisSet","lumi");
  if(statlevel==3) ws->defineSet("nuisSet","sigMassDelta");
  if(statlevel==4) ws->defineSet("nuisSet","sigWidthDelta");
  if(statlevel==5) ws->defineSet("nuisSet","lumi,sigMassDelta");
  if(statlevel==6) ws->defineSet("nuisSet","lumi,sigMassDelta,sigWidthDelta");
  if(statlevel==7) ws->defineSet("nuisSet","sigMassDelta");
  if(statlevel==8) ws->defineSet("nuisSet","lumi,sigMassDelta");
  if(statlevel==9) ws->defineSet("nuisSet","lumi");

  // do a background-only fit first
  if(usePseudodata) {
    ws->var("p1")->setConstant(true);
    ws->var("p2")->setConstant(true);
    ws->var("p3")->setConstant(true);
    ws->var("pb1")->setConstant(true);
    ws->var("pb2")->setConstant(true);
    ws->var("pb3")->setConstant(true);
    ws->var("pc1")->setConstant(true);
    ws->var("pc2")->setConstant(true);
  }
  RooFitResult* fit=doFit(std::string("bfita")+label, ws->pdf("modela"), binnedData, invmass, ws->var("nbkg")->getVal(), NBINS-1, BOUNDARIES);
  fit=doFit(std::string("bfitb")+label, ws->pdf("modelb"), binnedData, invmass, ws->var("nbkg")->getVal(), NBINS-1, BOUNDARIES);
  fit=doFit(std::string("bfitc")+label, ws->pdf("modelc"), binnedData, invmass, ws->var("nbkg")->getVal(), NBINS-1, BOUNDARIES);

  // integrate over model to get xs estimate as input to the B+S fit
  double pdfIntegral=PD_NBKG*calcPDF1DIntegral(ws->pdf("modela"), invmass, signalMass*0.9, signalMass*1.1);
  double maxXS = sqrt(pdfIntegral)*5/ws->var("lumi")->getVal();
  RooRealVar* xs=ws->var("xs");
  xs->setRange(0,maxXS);
  xs->setVal(maxXS/5.0);

  // do the background+signal fit
  xs->setConstant(false);
  if(!usePseudodata) {
    fit=doFit(std::string("bsfita")+label, ws->pdf("modela"), binnedData, invmass, ws->var("nbkg")->getVal()+ws->function("nsig")->getVal(), NBINS-1, BOUNDARIES);
    fit=doFit(std::string("bsfitb")+label, ws->pdf("modelb"), binnedData, invmass, ws->var("nbkg")->getVal()+ws->function("nsig")->getVal(), NBINS-1, BOUNDARIES);
    fit=doFit(std::string("bsfitc")+label, ws->pdf("modelc"), binnedData, invmass, ws->var("nbkg")->getVal()+ws->function("nsig")->getVal(), NBINS-1, BOUNDARIES);
  }

  // set parameters for limit calculation
  ws->var("p1")->setConstant(true);
  ws->var("p2")->setConstant(true);
  ws->var("p3")->setConstant(true);
  ws->var("pb1")->setConstant(true);
  ws->var("pb2")->setConstant(true);
  ws->var("pb3")->setConstant(true);
  ws->var("pc1")->setConstant(true);
  ws->var("pc2")->setConstant(true);
  ws->var("nbkg")->setConstant(true);
  if(statlevel==1) {
  } else if(statlevel==2) {
    ws->var("lumi")->setConstant(false);
  } else if(statlevel==3) {
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==4) {
    ws->var("sigWidthDelta")->setConstant(false);
  } else if(statlevel==5) {
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==6) {
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
    ws->var("sigWidthDelta")->setConstant(false);
  } else if(statlevel==7) {
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==8) {
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==9) {
    ws->var("lumi")->setConstant(false);
  }

  // setup model config
  ModelConfig *modelConfigA = new ModelConfig("modelConfigA");
  modelConfigA->SetWorkspace(*ws);
  modelConfigA->SetPdf(*ws->pdf("modela"));
  modelConfigA->SetPriorPdf(*ws->pdf("prior"));
  modelConfigA->SetParametersOfInterest(*ws->set("POI"));
  modelConfigA->SetNuisanceParameters(*ws->set("nuisSet"));
  ModelConfig *modelConfigB = new ModelConfig("modelConfigB");
  modelConfigB->SetWorkspace(*ws);
  modelConfigB->SetPdf(*ws->pdf("modelb"));
  modelConfigB->SetPriorPdf(*ws->pdf("prior"));
  modelConfigB->SetParametersOfInterest(*ws->set("POI"));
  modelConfigB->SetNuisanceParameters(*ws->set("nuisSet"));
  ModelConfig *modelConfigC = new ModelConfig("modelConfigC");
  modelConfigC->SetWorkspace(*ws);
  modelConfigC->SetPdf(*ws->pdf("modelc"));
  modelConfigC->SetPriorPdf(*ws->pdf("prior"));
  modelConfigC->SetParametersOfInterest(*ws->set("POI"));
  modelConfigC->SetNuisanceParameters(*ws->set("nuisSet"));

  ws->import(*modelConfigA);
  ws->import(*modelConfigB);
  ws->import(*modelConfigC);
  ws->Print();
  printVal(*ws->var("xs"));
  printVal(*ws->var("nbkg"));
  printVal(*ws->var("lumi"));
  printVal(*ws->var("lumiM0"));
  printVal(*ws->var("lumiK0"));
  printVal(*ws->var("sigMassDelta"));
  printVal(*ws->var("sigMassDeltaM0"));
  printVal(*ws->var("sigMassDeltaK0"));
  printVal(*ws->var("sigWidthDelta"));
  printVal(*ws->var("sigWidthDeltaM0"));
  printVal(*ws->var("sigWidthDeltaK0"));
  printVal(*ws->var("p1"));
  printVal(*ws->var("p2"));
  printVal(*ws->var("p3"));
  printVal(*ws->var("pb1"));
  printVal(*ws->var("pb2"));
  printVal(*ws->var("pb3"));
  printVal(*ws->var("pc1"));
  printVal(*ws->var("pc2"));
  ws->writeToFile("ws.root");

  double lower=-1, upper=-1;
  int niters=1000;
  double alpha=0.05;
  double lstail=0.0;
  JPMCCalculator mcA(*binnedData, *modelConfigA);
  mcA.SetNumIterations(niters);
  mcA.SetConfidenceLevel(1-alpha);
  mcA.SetLeftSideTailFraction(lstail);
  JPMCCalculator mcB(*binnedData, *modelConfigB);
  mcB.SetNumIterations(niters);
  mcB.SetConfidenceLevel(1-alpha);
  mcB.SetLeftSideTailFraction(lstail);
  JPMCCalculator mcC(*binnedData, *modelConfigC);
  mcC.SetNumIterations(niters);
  mcC.SetConfidenceLevel(1-alpha);
  mcC.SetLeftSideTailFraction(lstail); 

  double p1val=ws->var("p1")->getVal(); double p1err=ws->var("p1")->getError();
  double p2val=ws->var("p2")->getVal(); double p2err=ws->var("p2")->getError();
  double p3val=ws->var("p3")->getVal(); double p3err=ws->var("p3")->getError();

  TH1D* histA=dynamic_cast<TH1D*>(mcA.GetPosteriorHist()->Clone("histA"));
  if(statlevel==1 || statlevel==5 || statlevel==6 || statlevel==7 || statlevel==9) {
    ws->var("p1")->setVal(std::max(0.0,p1val-p1err));
    ws->var("p2")->setVal(std::max(0.0,p2val+p2err));
    ws->var("p3")->setVal(std::max(0.0,p3val+p3err));
    TH1D* histAHi=dynamic_cast<TH1D*>(mcA.GetPosteriorHistForce()->Clone("histAHi"));
    ws->var("p1")->setVal(std::max(0.0,p1val+p1err));
    ws->var("p2")->setVal(std::max(0.0,p2val-p2err));
    ws->var("p3")->setVal(std::max(0.0,p3val-p3err));
    TH1D* histALo=dynamic_cast<TH1D*>(mcA.GetPosteriorHistForce()->Clone("histALo"));
    TH1D* histB=dynamic_cast<TH1D*>(mcB.GetPosteriorHist()->Clone("histB"));
    TH1D* histC=dynamic_cast<TH1D*>(mcC.GetPosteriorHist()->Clone("histC"));

    histA->Add(histAHi);
    histA->Add(histALo);
    histA->Add(histB);
    histA->Add(histC);
  }
  TH1* hPosteriorHist=histA;
  TCanvas* canvas = new TCanvas("mcPost","Posterior Distribution",500,500);
  hPosteriorHist->Draw();
  canvas->SaveAs(TString("MCpost")+label+".gif");

  // Calculate the interval by hand
  double lowerCutOff = lstail*alpha;
  double upperCutOff = 1.-(1.-lstail)*alpha;
  TH1D* hCdf = (TH1D*)hPosteriorHist->Clone("hCdf");
  for(int bin=1; bin<=hCdf->GetNbinsX()+1; ++bin) {
    double last=hCdf->GetBinContent(bin-1);
    double next=hPosteriorHist->GetBinContent(bin);
    hCdf->SetBinContent(bin, last+next);
  }
  hCdf->Scale(1./hPosteriorHist->GetSumOfWeights());

  for(int bin=1; bin<=hCdf->GetNbinsX(); bin++) {
    if(lowerCutOff<=hCdf->GetBinContent(bin)) {
      lower=hCdf->GetBinLowEdge(bin);
      break;
    }
  }

  for(int bin=hCdf->GetNbinsX(); bin>=1; --bin) {
    if(upperCutOff>=hCdf->GetBinContent(bin)) {
      upper=hCdf->GetBinLowEdge(bin+1);
      break;
    }
  }
  delete hCdf;

  //print the results
  cout << "m0 = " << signalMass << " ; "
       << "statlevel = " << statlevel << " ; "
       << "lowerlimit = " << lower << " ; "
       << "upperlimit = " << upper << endl;

  t.Print();

}
