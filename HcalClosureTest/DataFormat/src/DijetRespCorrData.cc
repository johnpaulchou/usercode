#include "HcalClosureTest/DataFormat/interface/DijetRespCorrData.h"

#include "TMinuit.h"
#include "TMath.h"

#include <sstream>
//#include <iostream>
#include <cassert>

ClassImp(DijetRespCorrDatum)
ClassImp(DijetRespCorrData)

DijetRespCorrDatum::DijetRespCorrDatum() {
  fResolution=fTagEcalEt=fProbeEcalEt=0.0;
  fTagHcalEt.Set(NUMTOWERS);
  fProbeHcalEt.Set(NUMTOWERS);
}

DijetRespCorrDatum::~DijetRespCorrDatum() {}

Double_t DijetRespCorrDatum::GetResolution(void) const
{
  return fResolution;
}

Double_t DijetRespCorrDatum::GetTagHcalEt(Int_t ieta) const
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  Int_t index = ieta+MAXIETA;
  return fTagHcalEt.GetAt(index);
}

Double_t DijetRespCorrDatum::GetTagEcalEt(void) const
{
  return fTagEcalEt;
}

Double_t DijetRespCorrDatum::GetProbeHcalEt(Int_t ieta) const
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  Int_t index = ieta+MAXIETA;
  return fProbeHcalEt.GetAt(index);
}

Double_t DijetRespCorrDatum::GetProbeEcalEt(void) const
{
  return fProbeEcalEt;
}

void DijetRespCorrDatum::SetResolution(Double_t v)
{
  fResolution = v;
  return;
}

void DijetRespCorrDatum::SetTagHcalEt(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  Int_t index = ieta+MAXIETA;
  fTagHcalEt.SetAt(v, index);
  return;
}

void DijetRespCorrDatum::SetTagEcalEt(Double_t v)
{
  fTagEcalEt = v;
  return;
}

void DijetRespCorrDatum::SetProbeHcalEt(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  Int_t index = ieta+MAXIETA;
  fProbeHcalEt.SetAt(v, index);
  return;
}

void DijetRespCorrDatum::SetProbeEcalEt(Double_t v)
{
  fProbeEcalEt = v;
  return;
}

DijetRespCorrData::DijetRespCorrData()
{
  fData.clear();
  fPrintLevel=5;
  fParStep=0.1;
  fParMin=0.0;
  fParMax=3.0;
}

DijetRespCorrData::~DijetRespCorrData()
{
}

void DijetRespCorrData::push_back(const DijetRespCorrDatum& d)
{
  fData.push_back(d);
  return;
}

const DijetRespCorrDatum& DijetRespCorrData::GetAt(Int_t i) const
{
  return fData[i];
}

Int_t DijetRespCorrData::GetSize(void) const
{
  return fData.size();
}

Double_t DijetRespCorrData::GetLikelihoodDistance(const TArrayD& respcorr) const
{
  Double_t total=0.0;
  for(std::vector<DijetRespCorrDatum>::const_iterator it=fData.begin(); it!=fData.end(); ++it) {
    Double_t sigma=it->GetResolution();
    Double_t te=it->GetTagEcalEt();
    Double_t pe=it->GetProbeEcalEt();
    Double_t th=0.0, ph=0.0;
    for(int i=0; i<NUMTOWERS; i++) {
      int ieta = -MAXIETA+i;
      if(ieta==0) continue;
      th += respcorr[i]*it->GetTagHcalEt(ieta);
      ph += respcorr[i]*it->GetProbeHcalEt(ieta);
    }
    Double_t x=ph+pe-th-te;
    total += TMath::Log(sigma)-x*x/2/sigma/sigma;
  }
  return total;
}

TArrayD DijetRespCorrData::doFit(void)
{
  Double_t array[NUMTOWERS] =
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601,
     1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756,
     1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321,
     1.21111, 1.175, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
  TArrayD respCorrInit;
  respCorrInit.Set(NUMTOWERS, array);
  return doFit(respCorrInit, 23);
}

TArrayD DijetRespCorrData::doFit(const TArrayD& respCorrInit, Int_t maxIetaFixed)
{
  // set the number of parameters to be the number of towers
  TMinuit *gMinuit=new TMinuit(NUMTOWERS);
  gMinuit->SetPrintLevel(fPrintLevel);
  gMinuit->SetErrorDef(0.5); // for a likelihood
  gMinuit->SetFCN(FCN);
  gMinuit->SetObjectFit(this);

  // define the parameters
  for(int i=0; i<respCorrInit.GetSize(); i++) {
    int ieta=-MAXIETA+i;
    std::ostringstream oss;
    oss << "Tower ieta: " << ieta;
    gMinuit->DefineParameter(i, oss.str().c_str(), respCorrInit[i], fParStep, fParMin, fParMax);
    if(ieta>=-maxIetaFixed && ieta<=maxIetaFixed) gMinuit->FixParameter(i);
  }

  // Minimize
  gMinuit->Migrad();

  // get the results
  TArrayD results(NUMTOWERS);
  for(int i=0; i<results.GetSize(); i++) {
    Double_t val, error;
    gMinuit->GetParameter(i, val, error);
    results[i]=val;
  }
  return results;
}

void DijetRespCorrData::FCN(Int_t &npar, Double_t*, Double_t &f, Double_t *par, Int_t)
{
  // get the relevant data
  const DijetRespCorrData* data=dynamic_cast<const DijetRespCorrData*>(gMinuit->GetObjectFit());
  TArrayD respcorr;
  respcorr.Set(NUMTOWERS, par);
  f = data->GetLikelihoodDistance(respcorr);
  
  return;
}
