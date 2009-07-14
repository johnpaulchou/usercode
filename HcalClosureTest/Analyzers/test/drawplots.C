#include "HcalClosureTest/DataFormat/interface/DijetRespCorrData.h"
#include <iostream>
#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TCanvas.h"

TFile* rootfile=0;
TCanvas *c=0;

class fitter
{
  static void doFit(DijetRespCorrData* data, int maxFixedIeta);

private:
  fitter() {}
  virtual ~fitter() {}
  
  static void FCN(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t flag);
  static DijetRespCorrData* data_;
  static int maxFixedIeta_;
};

DijetRespCorrData* fitter::data_;
int fitter::maxFixedIeta_=23;

void fitter::FCN(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t flag)
{
  double total=0;
  for(int i=0; i<respcorrdata->GetSize(); i++) {
    DijetRespCorrDatum d=respcorrdata->GetAt(i);
    total -= TMath::Log(d.GetResolution()) - 
  }

}

void fitter::doFit(DijetRespCorrData* data, int maxFixedIeta)
{
  data_ = data;
  maxFixedIeta_ = maxFixedIeta;

  TMinuit *gMinuit=new TMinuit(83);
  gMinuit->SetPrintLevel(-1);
  gMinuit->SetErrorDef(0.5); // for a likelihood
  gMinuit->SetFCN(FCN);

  // define the background parameters
  for(int i=0; i<83; i++) {
    int ieta=i-41;
    std::ostringstream oss;
    oss << "ieta=" << ieta;
    gMinuit->DefineParameter(i, oss.str().c_str(), 1.0, .01, 0.0, 0.0);
    if(abs(ieta)<=maxFixedIeta_) gMinuit->FixParameter(i);
  }

  // Minimize
  gMinuit->Migrad();

  // get fitted values (assume there are less than 100 backgrounds...)
  for(int i=0; i<83; i++) {
    double scale, err;
    gMinuit->GetParameter(i, scale, err);
    std::cout << i << " " << scale << " " << err << std::endl;
  }
  return;
}


void drawplots(void)
{
  rootfile = new TFile("plots.root");
  c=new TCanvas();

  TH1D* hResolution=new TH1D("Resolution","Resolution",100,0,100);

  DijetRespCorrData* respcorrdata = dynamic_cast<DijetRespCorrData*>(gROOT->FindObject("respcorrdata"));


  hResolution->Draw();
}
