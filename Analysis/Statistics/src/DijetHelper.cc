#include "Analysis/Statistics/interface/DijetHelper.h"

#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooBinning.h"
#include "RooAbsReal.h"

#include "TH1.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLatex.h"

using namespace RooFit;

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double *binsx)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram",nbins, binsx);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedData(filename, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram", nbins, xlo, xhi);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedData(filename, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, TH1D* hist)
{
  // open a file
  ifstream is;
  is.open(filename.c_str(), ifstream::in);

  // fill the histogram
  while(is.good()) {
    double temp;
    is >> temp;
    if(!is.good()) break;
    hist->Fill(temp);
  }

  // create a new binned data object
  RooDataHist *binnedData = new RooDataHist(objectname.c_str(),"binned data", RooArgSet(*var), Import(*hist, kFALSE));

  return binnedData;
}


RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double *binsx)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram",nbins, binsx);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedPseudoData(pdf, nevents, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram", nbins, xlo, xhi);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedPseudoData(pdf, nevents, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, TH1D* hist)
{
  for(int i=1; i<=hist->GetNbinsX(); i++) {
    double xlo=hist->GetBinLowEdge(i);
    double xhi=hist->GetBinLowEdge(i+1);
    double integral=DijetHelper::calcPDF1DIntegral(pdf, var, xlo, xhi);
    hist->Fill(0.5*(xlo+xhi), integral*nevents);
  }

  // create a new binned data object
  RooDataHist *binnedData = new RooDataHist(objectname.c_str(), "binned pseudo-data", RooArgSet(*var), Import(*hist, kFALSE));

  return binnedData;
}

RooDataSet* DijetHelper::makeUnbinnedData(std::string filename, std::string objectname, RooRealVar* var)
{
  // create the dataset
  RooDataSet* data = new RooDataSet(objectname.c_str(), "unbinned data", RooArgSet(*var));
  
  // fill the dataset
  ifstream is;
  is.open(filename.c_str(), ifstream::in);
  while(is.good()) {
    double temp;
    is >> temp;
    if(!is.good()) break;
    var->setVal(temp);
    data->add(RooArgSet(*var));
  }
  return data;
}

double DijetHelper::calcPDF1DIntegral(RooAbsPdf* pdf, RooRealVar* var, double min, double max)
{
  var->setRange("signal",min,max);
  RooAbsReal* pdfIntegral = pdf->createIntegral(*var, NormSet(*var), Range("signal"));
  return pdfIntegral->getVal();
}


RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double xlo, double xhi)
{
  double *binsx=new double[nbins+1];
  for(int i=0; i<nbins+1; i++) {
    binsx[i]=xlo+i*(xhi-xlo)/nbins;
  }
  RooFitResult* result=DijetHelper::doFit(name, pdf, data, var, nsig, nbkg, nbins, binsx);
  delete[] binsx;
  return result;
}

RooFitResult* DijetHelper::doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double *binsx)
{
  TString label=name.c_str();
  RooFitResult *fit = pdf->fitTo(*data, Save(kTRUE), Extended(kTRUE), Strategy(2), PrintLevel(1));
  fit->Print();

  // plot fit
  TCanvas* cfit = new TCanvas(TString("cfit")+label,"fit",500,500);
  RooPlot* plot = var->frame();
  data->plotOn(plot, Binning(RooBinning(nbins, binsx)));
  pdf->plotOn(plot, LineColor(kBlue+2));
  pdf->paramOn(plot, Layout(0.43, 0.88, 0.92), Format("NEU",AutoPrecision(1)) ); 
  gPad->SetLogy();
  gPad->SetGrid(1,1);
  plot->GetYaxis()->SetRangeUser(0.1,plot->GetMaximum()*2.0);
  plot->GetYaxis()->SetTitleOffset(1.2);
  plot->Draw();
  cfit->SaveAs(label+".gif");
  delete plot;

  // calculate pull and diff
  RooArgSet observables(*var);
  TH1* hist=data->createHistogram(TString("dataHist")+name,*var,Binning(RooBinning(nbins,binsx)));
  TH1D* pull=(TH1D*)hist->Clone(TString("pull")+name);
  TH1D* diff=(TH1D*)hist->Clone(TString("diff")+name);
  double chi2=0;
  for(int i=1; i<=hist->GetNbinsX(); i++) {
    double content = hist->GetBinContent(i);
    double error = content>25 ? sqrt(content) :
      (TMath::ChisquareQuantile(0.95,2*(content+1)-TMath::ChisquareQuantile(0.05,2*content)))/4.0;

    // evaluate the pdf
    var->setVal(hist->GetBinCenter(i));
    double fitval = pdf->getVal(&observables)*(nsig->getVal()+nbkg->getVal())*hist->GetBinWidth(i);

    // calculate the pull and diff
    double pullval=error==0 ? 0 : (content-fitval)/error;
    double diffval=(content-fitval)/fitval;
    double differr=error/fitval;
    chi2+=fabs(pullval);

    pull->SetBinContent(i, pullval);
    pull->SetBinError(i, 1.0);
    diff->SetBinContent(i, diffval);
    diff->SetBinError(i, differr);
  }

  // draw pull
  TCanvas* cpull = new TCanvas(TString("cpull")+label,"pull",500,500);
  pull->SetMarkerStyle(20);
  pull->Draw("E");
  pull->GetYaxis()->SetTitle("Pull");
  char title[100];
  sprintf(title, "#chi^{2} = %2.2f", chi2);
  TLatex tex;
  tex.SetNDC();
  tex.SetTextAlign(12);
  tex.SetTextSize(0.04);
  tex.DrawLatex(0.22,0.88, title);
  cpull->SaveAs(label+"_pull.gif");

  // draw diff
  TCanvas* cdiff = new TCanvas(TString("cdiff")+label,"(data-fit)/fit",500,500);
  diff->SetMarkerStyle(20);
  diff->GetYaxis()->SetTitle("(data-fit)/fit");
  diff->SetMaximum(0.3);
  diff->SetMinimum(-0.3);
  diff->Draw("E");
  tex.DrawLatex(0.22,0.88, title);
  cdiff->SaveAs(label+"_diff.gif");

  return fit;
}


double DijetHelper::getParFinalValue(RooFitResult* fit, std::string parname)
{
  // find a parameter
  RooArgList args=fit->floatParsFinal();
  for(int i=0; i<args.getSize(); i++) {
    TString name(args.at(i)->GetName());
    std::cout << "Found name=" << name << std::endl;
    if(name.CompareTo(parname.c_str())==0) {
      RooRealVar* v=dynamic_cast<RooRealVar*>(args.at(i));
      return (v->getVal());
    }
  }
  std::cout << "Could not find parameter " << parname << std::endl;
  return 0.0;
}
 
void DijetHelper::printVal(const RooRealVar& var)
{
  std::cout << var.GetName() << "=" << var.getVal() << "+/-" << var.getError()
	    << " from " << var.getMin() << " to " << var.getMax() 
	    << "; isConst=" << var.isConstant() << std::endl;
  return;
}

double DijetHelper::calcPoissonError(double mu)
{
  double error=sqrt(mu);
  if(mu<25)
    error=(TMath::ChisquareQuantile(0.95,2*(mu+1)-TMath::ChisquareQuantile(0.05,2*mu)))/4.0;
  return error;
}

double DijetHelper::getMinMaxDeviations(RooAbsPdf* pdf, RooAbsPdf* pdfb, RooAbsPdf* pdfc, RooRealVar* var, double normalization, int nbins, double *binsx)
{
  double result=0.0;
  double maxdiff=0.0;
  for(int i=0; i<nbins; i++) {
    RooArgSet s(*var);
    double val=normalization*DijetHelper::calcPDF1DIntegral(pdf, var, binsx[i], binsx[i+1]);
    double valb=normalization*DijetHelper::calcPDF1DIntegral(pdfb, var, binsx[i], binsx[i+1]);
    double valc=normalization*DijetHelper::calcPDF1DIntegral(pdfc, var, binsx[i], binsx[i+1]);
    double diffb=fabs(val-valb)/val;
    double diffc=fabs(val-valc)/val;
    if(diffb>maxdiff) maxdiff=diffb;
    if(diffc>maxdiff) maxdiff=diffc;
    double relerror=DijetHelper::calcPoissonError(val)/val;

    printf("%3d %7.1f %13.5f %13.5f %13.5f %11.5f %11.5f %11.5f %11.5f\n", i, binsx[i], val, valb, valc, diffb, diffc, maxdiff, relerror);
    if(maxdiff>relerror && maxdiff>result) result=maxdiff;
  }
  return result;
}

