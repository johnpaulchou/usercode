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
  double NBKGERROR=0.01; // relative error on background normalization
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


  if(argc<3) {
    std::cout << "Usage: dijet signalMass statlevel statmethod [usePseudoData=false]" << std::endl;
    return -1;
  }

  double signalMass = atof(argv[1]);
  int statlevel = atoi(argv[2]);
  int method = atoi(argv[3]);
  bool usePseudodata = argc>=5 ? atoi(argv[4]) : false;

  // setup label
  char label[100];
  sprintf(label, "%2.0f_%d_%d", signalMass, statlevel, method);

  // setup workspace
  RooWorkspace *ws = new RooWorkspace("ws");

  // observable
  ws->factory("invmass[0]");
  RooRealVar* invmass=ws->var("invmass");
  invmass->setRange(MININVMASS, MAXINVMASS);

  // background
  ws->factory("CEXPR::background('pow(1.0-invmass/7000.0,p1)/pow(invmass/7000.0,p2+p3*log(invmass/7000.0))', p1[10,-40,40], p2[7,-20,20], p3[0.1,-20,20],invmass)");
  ws->factory("CEXPR::backgroundb('pow(1-invmass/7000.0+pb3*(invmass/7000.0)*(invmass/7000.0),pb1)/pow(invmass/7000,pb2)',pb1[20,0,100],pb2[5,-100,100], pb3[0.8,0,2],invmass)");
  ws->factory("CEXPR::backgroundc('pow(1-invmass/7000.0,pc1)/pow(invmass/7000,pc2)',pc1[13,-100,100],pc2[5,-100,100],invmass)");
  if(USELOGNORM) ws->factory("RooLognormal::nbkg_prior(nbkg[0], nbkgM0[0], nbkgK0[1])");
  else           ws->factory("RooGaussian::nbkg_prior(nbkg[0], nbkgM0[0], nbkgK0[1])");
  // set the background to the seed (this will later get set by the fit)
  ws->var("nbkg")->setVal(PD_NBKG);
  ws->var("nbkg")->setRange(std::max(0.0, PD_NBKG-NSIGCUTOFF*sqrt(PD_NBKG)), PD_NBKG+NSIGCUTOFF*sqrt(PD_NBKG));
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
  //  ws->factory("cexpr::invmassprime('sigMassDelta*(sigWidthDelta*(invmass-sigMass)+sigMass)',sigMassDelta,sigWidthDelta,invmass,sigMass)");
  ws->var("sigMassDelta")->setConstant(true);
  ws->var("sigWidthDelta")->setConstant(true);

  // signal shape
  //  Qstar_qg_3 *signal=new Qstar_qg_3("signal","asdf",*ws->var("invmass"), *ws->var("sigMassDelta"), *ws->var("sigWidthDelta"), *ws->var("sigMass"));
  //  ws->import(*signal);
  ws->factory("Qstar_qg_3::signal(invmass, sigMassDelta, sigWidthDelta, sigMass)");
  //  ws->factory("RooGaussian::signal(invmassprime, sigMass, 100)");

  // model
  ws->factory("SUM::model(nsig*signal, nbkg*background)");
  ws->factory("SUM::modelb(nsig*signal, nbkg*backgroundb)");
  ws->factory("SUM::modelc(nsig*signal, nbkg*backgroundc)");
  ws->defineSet("observables","invmass");
  ws->defineSet("POI","xs");
  if(statlevel==0) ws->factory("PROD::prior(xs_prior)");
  if(statlevel==1) ws->factory("PROD::prior(xs_prior,nbkg_prior)");
  if(statlevel==2) ws->factory("PROD::prior(xs_prior,lumi_prior)");
  if(statlevel==3) ws->factory("PROD::prior(xs_prior,sigMassDelta_prior)");
  if(statlevel==4) ws->factory("PROD::prior(xs_prior,sigWidthDelta_prior)");
  if(statlevel==5) ws->factory("PROD::prior(xs_prior,nbkg_prior,lumi_prior,sigMassDelta_prior)");
  if(statlevel==6) ws->factory("PROD::prior(xs_prior,nbkg_prior,lumi_prior,sigMassDelta_prior,sigWidthDelta_prior)");
  if(statlevel==7) ws->factory("PROD::prior(xs_prior,nbkg_prior,sigMassDelta_prior)");
  if(statlevel==8) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior)");
  if(statlevel==9) ws->factory("PROD::prior(xs_prior,nbkg_prior,lumi_prior)");
  if(statlevel==0) ws->defineSet("nuisSet","");
  if(statlevel==1) ws->defineSet("nuisSet","nbkg");
  if(statlevel==2) ws->defineSet("nuisSet","lumi");
  if(statlevel==3) ws->defineSet("nuisSet","sigMassDelta");
  if(statlevel==4) ws->defineSet("nuisSet","sigWidthDelta");
  if(statlevel==5) ws->defineSet("nuisSet","nbkg,lumi,sigMassDelta");
  if(statlevel==6) ws->defineSet("nuisSet","nbkg,lumi,sigMassDelta,sigWidthDelta");
  if(statlevel==7) ws->defineSet("nuisSet","nbkg,sigMassDelta");
  if(statlevel==8) ws->defineSet("nuisSet","lumi,sigMassDelta");
  if(statlevel==9) ws->defineSet("nuisSet","nbkg,lumi");

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
  double mininvmass=ws->var("invmass")->getMin();
  double maxinvmass=ws->var("invmass")->getMax();
  ws->var("invmass")->setRange("FULL", mininvmass,maxinvmass);
  RooFitResult* fit=doFit(std::string("b1fit")+label, ws->pdf("model"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL");

  // integrate over model to get xs estimate as input to the B+S fit
  double pdfIntegral=ws->var("nbkg")->getVal()*calcPDF1DIntegral(ws->pdf("model"), invmass, signalMass*0.9, signalMass*1.1);
  double maxXS = sqrt(pdfIntegral)*5/ws->var("lumi")->getVal();
  RooRealVar* xs=ws->var("xs");
  xs->setRange(0,maxXS);
  xs->setVal(maxXS/5.0);

  // do the background+signal fit
  xs->setConstant(false);
  if(!usePseudodata) {
    fit=doFit(std::string("bsfit")+label, ws->pdf("model"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL");
    std::cout << "B+S STATUS = " << fit->status() << std::endl;
  }

  // set the background values to the fit
  double nbkgVal=ws->var("nbkg")->getVal();
  ws->var("nbkg")->setRange(std::max(0.0, nbkgVal-NSIGCUTOFF*nbkgVal*NBKGERROR), nbkgVal+NSIGCUTOFF*nbkgVal*NBKGERROR);
  ws->var("nbkgM0")->setVal(nbkgVal);
  if(USELOGNORM) ws->var("nbkgK0")->setVal(NBKGERROR+1);
  else           ws->var("nbkgK0")->setVal(nbkgVal*NBKGERROR);

  // set parameters for limit calculation
  ws->var("p1")->setConstant(true);
  ws->var("p2")->setConstant(true);
  ws->var("p3")->setConstant(true);
  ws->var("nbkg")->setConstant(true);
  if(statlevel==1) {
    ws->var("nbkg")->setConstant(false);
  } else if(statlevel==2) {
    ws->var("lumi")->setConstant(false);
  } else if(statlevel==3) {
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==4) {
    ws->var("sigWidthDelta")->setConstant(false);
  } else if(statlevel==5) {
    ws->var("nbkg")->setConstant(false);
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==6) {
    ws->var("nbkg")->setConstant(false);
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
    ws->var("sigWidthDelta")->setConstant(false);
  } else if(statlevel==7) {
    ws->var("nbkg")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==8) {
    ws->var("lumi")->setConstant(false);
    ws->var("sigMassDelta")->setConstant(false);
  } else if(statlevel==9) {
    ws->var("nbkg")->setConstant(false);
    ws->var("lumi")->setConstant(false);
  }

  // setup model config
  ModelConfig *modelConfig = new ModelConfig("modelConfig");
  modelConfig->SetWorkspace(*ws);
  modelConfig->SetPdf(*ws->pdf("model"));
  modelConfig->SetPriorPdf(*ws->pdf("prior"));
  modelConfig->SetParametersOfInterest(*ws->set("POI"));
  modelConfig->SetNuisanceParameters(*ws->set("nuisSet"));

  ws->import(*modelConfig);
  ws->Print();
  printVal(*ws->var("xs"));
  printVal(*ws->var("nbkg"));
  printVal(*ws->var("nbkgM0"));
  printVal(*ws->var("nbkgK0"));
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

  // do a final fit (with a fixed lumi nuisance parameter, since it is always anti-correlated to the POI)
  bool isLumiFixed = ws->var("lumi")->isConstant();
  ws->var("lumi")->setConstant(true);
  fit=doFit(std::string("finalfit")+label, ws->pdf("model"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL");
  ws->var("lumi")->setConstant(isLumiFixed);
  ws->writeToFile("ws.root");

  double lower=-1, upper=-1;
  if(method==0) {
    BayesianCalculator bc(*binnedData, *modelConfig);
    bc.SetConfidenceLevel(0.95);
    bc.SetLeftSideTailFraction(0.0);
    SimpleInterval* bInt = bc.GetInterval();
    TCanvas* canvas = new TCanvas("bcPlost","Posterior Distribution",500,500);
    RooPlot* plot = bc.GetPosteriorPlot();
    plot->Draw();
    canvas->SaveAs(TString("BCpost")+label+".gif");
    delete plot;
    lower=bInt->LowerLimit();
    upper=bInt->UpperLimit();

  } else if(method==1) {
    ProposalHelper ph;
    ph.SetVariables((RooArgSet&)fit->floatParsFinal());
    ph.SetCovMatrix(fit->covarianceMatrix());
    ph.SetUpdateProposalParameters(kTRUE);
    ph.SetCacheSize(100);
    ProposalFunction* pdfProp = ph.GetProposalFunction();
    
    MCMCCalculator mc(*binnedData, *modelConfig);
    mc.SetNumBins(100);
    mc.SetConfidenceLevel(0.95);
    mc.SetLeftSideTailFraction(0.0);
    mc.SetNumIters(300000);
    mc.SetNumBurnInSteps(5000);
    mc.SetProposalFunction(*pdfProp);
    MCMCInterval* interval = mc.GetInterval();
    RooArgList* poiList = interval->GetAxes();
    RooRealVar* poi=(RooRealVar*)poiList->at(0);

    // draw posterior plot
    TCanvas * c1 = new TCanvas("MCMCpost", "MCMCpost", 500, 500);
    MCMCIntervalPlot *plot = new MCMCIntervalPlot(*interval);
    plot->Draw();
    c1->SaveAs(TString("mcmcPost")+label+".gif");
    delete plot;
    lower=interval->LowerLimit(*poi);
    upper=interval->UpperLimit(*poi);
    
  } else if(method==2) {
    RooArgSet* nullParams = new RooArgSet("nullParams");
    nullParams->addClone(*ws->var("xs"));
    nullParams->setRealValue("xs",0.0);

    ProfileLikelihoodCalculator pl(*binnedData, *modelConfig);
    pl.SetNullParameters(*nullParams);
    pl.SetConfidenceLevel(0.95);
    LikelihoodInterval* interval=pl.GetInterval();

    // draw posterior plot
    TCanvas * c1 = new TCanvas("PLikePost", "PLikePost", 500, 500);
    LikelihoodIntervalPlot plot(interval);
    plot.Draw();
    plot.SetTitle("Profile Likelihood Ratio and Posterior for S");
    c1->SaveAs(TString("PLikePost")+label+".gif");

    RooRealVar* firstPOI = (RooRealVar*)modelConfig->GetParametersOfInterest()->first();
    lower = interval->LowerLimit(*firstPOI);
    upper = interval->UpperLimit(*firstPOI);
  } else if(method==3) {
    std::cout << "************************************************************" << std::endl;

    RooAbsReal* nll = ws->pdf("model")->createNLL(*binnedData,NumCPU(2));
    RooMinuit(*nll).migrad();
    
    RooPlot* frame = ws->var("xs")->frame(Bins(10),Range(0.0,500),Title("LL and profileLL in xs"));
    nll->plotOn(frame, ShiftToZero());

    RooAbsReal* pll_xs = nll->createProfile(*ws->var("xs"));
    pll_xs->plotOn(frame,LineColor(kRed));
    frame->SetMinimum(0);
    frame->SetMaximum(3);

    TCanvas *c=new TCanvas("c","c");
    frame->Draw();
    c->SaveAs(TString("NLL")+label+".gif");

    delete pll_xs;
    delete nll;

  } else if(method==4) {
    JPMCCalculator mc(*binnedData, *modelConfig);
    mc.SetNumIterations(1000);
    mc.SetConfidenceLevel(0.95);
    mc.SetLeftSideTailFraction(0.0);
    TCanvas* canvas = new TCanvas("mcPost","Posterior Distribution",500,500);
    //    TH1D* h=mc.GetPosteriorHistTestUpper(200);
    TH1D* h=mc.GetPosteriorHist();
    h->Draw();
    canvas->SaveAs(TString("MCpost")+label+".gif");

    SimpleInterval* bInt = mc.GetInterval();
    lower=bInt->LowerLimit();
    upper=bInt->UpperLimit();
  }


  //print the results
  cout << "m0 = " << signalMass << " ; "
       << "statlevel = " << statlevel << " ; "
       << "lowerlimit = " << lower << " ; "
       << "upperlimit = " << upper << endl;

  t.Print();

}
