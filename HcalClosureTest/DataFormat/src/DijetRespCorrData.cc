#include "HcalClosureTest/DataFormat/interface/DijetRespCorrData.h"

#include "TMinuit.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>

ClassImp(DijetRespCorrDatum)
ClassImp(DijetRespCorrData)

DijetRespCorrDatum::DijetRespCorrDatum() {
  fTagEta=fProbeEta=fTagEcalE=fProbeEcalE=0.0;
}

DijetRespCorrDatum::~DijetRespCorrDatum() {}

Double_t DijetRespCorrDatum::GetTagEta(void) const
{
  return fTagEta;
}

Double_t DijetRespCorrDatum::GetTagHcalE(Int_t ieta)
{
  Double_t v=fTagHcalE[ieta];
  return v;
}

void DijetRespCorrDatum::GetTagHcalE(std::map<Int_t, Double_t>& m) const
{
  m=fTagHcalE;
  return;
}

Double_t DijetRespCorrDatum::GetTagEcalE(void) const
{
  return fTagEcalE;
}

Double_t DijetRespCorrDatum::GetProbeEta(void) const
{
  return fProbeEta;
}

Double_t DijetRespCorrDatum::GetProbeHcalE(Int_t ieta)
{
  Double_t v=fProbeHcalE[ieta];
  return v;
}

void DijetRespCorrDatum::GetProbeHcalE(std::map<Int_t, Double_t>& m) const
{
  m=fProbeHcalE;
  return;
}

Double_t DijetRespCorrDatum::GetProbeEcalE(void) const
{
  return fProbeEcalE;
}

void DijetRespCorrDatum::SetTagEta(Double_t v)
{
  fTagEta = v;
  return;
}

void DijetRespCorrDatum::SetTagHcalE(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  fTagHcalE[ieta] = v;
  return;
}

void DijetRespCorrDatum::AddTagHcalE(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  fTagHcalE[ieta] += v;
  return;
}

void DijetRespCorrDatum::SetTagEcalE(Double_t v)
{
  fTagEcalE = v;
  return;
}

void DijetRespCorrDatum::SetProbeEta(Double_t v)
{
  fProbeEta = v;
  return;
}

void DijetRespCorrDatum::SetProbeHcalE(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  fProbeHcalE[ieta] = v;
  return;
}

void DijetRespCorrDatum::AddProbeHcalE(Double_t v, Int_t ieta)
{
  assert(ieta<=MAXIETA && ieta>=-MAXIETA && ieta!=0);
  fProbeHcalE[ieta] += v;
  return;
}

void DijetRespCorrDatum::SetProbeEcalE(Double_t v)
{
  fProbeEcalE = v;
  return;
}

void DijetRespCorrDatum::GetTagEnergies(const TArrayD& respcorr, Double_t& ecal, Double_t& hcal, Double_t& hf) const
{
  ecal=GetTagEcalE();
  hcal=0.0;
  hf=0.0;

  std::map<Int_t,Double_t> tagmap;
  GetTagHcalE(tagmap);
  for(std::map<Int_t, Double_t>::const_iterator mapit=tagmap.begin(); mapit!=tagmap.end(); ++mapit) {
    int ieta=mapit->first;
    double energy=mapit->second;
    int index=ieta+MAXIETA;
    if(std::abs(ieta)>29)
      hf += respcorr[index]*energy;
    else
      hcal += respcorr[index]*energy;
  }
  return;
}

void DijetRespCorrDatum::GetProbeEnergies(const TArrayD& respcorr, Double_t& ecal, Double_t& hcal, Double_t& hf) const
{
  // calculate the ecal, hcal, and HF energy
  // scale the hcal and hf energy by the response corrections
  ecal=GetProbeEcalE();
  hcal=0.0;
  hf=0.0;

  std::map<Int_t,Double_t> probemap;
  GetProbeHcalE(probemap);
  for(std::map<Int_t, Double_t>::const_iterator mapit=probemap.begin(); mapit!=probemap.end(); ++mapit) {
    int ieta=mapit->first;
    double energy=mapit->second;
    int index=ieta+MAXIETA;
    if(std::abs(ieta)>29)
      hf += respcorr[index]*energy;
    else
      hcal += respcorr[index]*energy;
  }
  return;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

DijetRespCorrData::DijetRespCorrData()
{
  fData.clear();
  fPrintLevel=5;
  fParStep=0.10;
  fParMin=0.0;
  fParMax=0.0;
  fEcalRes=0.07;
  fHcalRes=1.15;
  fHfRes=1.35;
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

  // loop over each jet pair
  for(std::vector<DijetRespCorrDatum>::const_iterator it=fData.begin(); it!=fData.end(); ++it) {

    // calculate the balance and resolution for each jet pair
    Double_t B, dB;
    GetBalance(*it, respcorr, B, dB);

    // this is the total likelihood
    total += 0.5*(std::log(dB*dB)+B*B/dB/dB);
  }
  return total;
}

TH2D* DijetRespCorrData::DrawBalance(const char* name, const char* title) const
{
  Double_t array[NUMTOWERS] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
				1.151630, 1.148890, 1.144870, 1.161240, 1.195630, 1.190690, 1.162400, 1.130190, 1.128810, 1.114070, 1.109850, 1.098620, 1.099950, 1.096140, 1.087350, 1.076980, 1.095540, 1.092960, 1.091500, 1.087100, 1.0, 1.085640, 1.094110, 1.089310, 1.089260, 1.079420, 1.089310, 1.093900, 1.104690, 1.098890, 1.109600, 1.115400, 1.144240, 1.125160, 1.148010, 1.204180, 1.197330, 1.150100, 1.154010, 1.143610, 1.159000,
				1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
  TArrayD respcorr;
  respcorr.Set(NUMTOWERS, array);

  TH2D* hB=new TH2D(name, title, 100,-2,2,10,0,5);

  // loop over each jet pair
  for(std::vector<DijetRespCorrDatum>::const_iterator it=fData.begin(); it!=fData.end(); ++it) {

    // calculate the balance and resolution for each jet pair
    Double_t B, dB;
    GetBalance(*it, respcorr, B, dB);
    Double_t te, th, thf;
    Double_t pe, ph, phf;
    it->GetTagEnergies(respcorr, te, th, thf);
    it->GetProbeEnergies(respcorr, pe, ph, phf);
    hB->Fill(B, fabs(it->GetProbeEta()));
  }
  return hB;
}

TH1D* DijetRespCorrData::doFit(const char* name, const char* title)
{
  TArrayD respcorr, respcorre;
  doFit(respcorr, respcorre);
  TH1D* h=new TH1D(name,title,NUMTOWERS,-MAXIETA-0.5,MAXIETA+0.5);
  for(int i=1; i<=NUMTOWERS; i++) {
    h->SetBinContent(i, respcorr[i-1]);
    h->SetBinError(i, respcorre[i-1]);
  }

  return h;
}

void DijetRespCorrData::doFit(TArrayD& respcorr, TArrayD& respcorre)
{
  // setup the initial response corrections
  const int maxIetaFixed=20;
  //  Double_t array[NUMTOWERS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.26437, 1.27885, 1.252, 1.26742, 1.32585, 1.27661, 1.29944, 1.451, 1.2652, 1.25045, 1.29709, 1.23643, 1.11458, 1.14601, 1.20513, 1.15064, 1.11951, 1.16586, 1.15791, 1.13728, 1.14483, 1.1412, 1.11142, 0, 1.15833, 1.14589, 1.15, 1.14048, 1.22407, 1.09756, 1.07979, 1.14484, 1.22885, 1.20833, 1.21161, 1.18929, 1.17783, 1.27585, 1.29167, 1.25481, 1.26563, 1.35172, 1.2816, 1.25988, 1.22321, 1.21111, 1.175, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
  Double_t array[NUMTOWERS] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
				1.151630, 1.148890, 1.144870, 1.161240, 1.195630, 1.190690, 1.162400, 1.130190, 1.128810, 1.114070, 1.109850, 1.098620, 1.099950, 1.096140, 1.087350, 1.076980, 1.095540, 1.092960, 1.091500, 1.087100, 1.0, 1.085640, 1.094110, 1.089310, 1.089260, 1.079420, 1.089310, 1.093900, 1.104690, 1.098890, 1.109600, 1.115400, 1.144240, 1.125160, 1.148010, 1.204180, 1.197330, 1.150100, 1.154010, 1.143610, 1.159000,
				1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

  TArrayD respCorrInit;
  respCorrInit.Set(NUMTOWERS, array);

  // set the number of parameters to be the number of towers
  TMinuit *gMinuit=new TMinuit(NUMTOWERS);
  gMinuit->SetPrintLevel(fPrintLevel);
  gMinuit->SetErrorDef(0.5); // for a log likelihood
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
  TArrayD errors(NUMTOWERS);
  for(int i=0; i<results.GetSize(); i++) {
    Double_t val, error;
    gMinuit->GetParameter(i, val, error);
    results[i]=val;
    errors[i]=error;
  }
  respcorr=results;
  respcorre=errors;

  return;
}

void DijetRespCorrData::GetBalance(const DijetRespCorrDatum& datum, const TArrayD& respcorr, Double_t& balance_, Double_t& resolution_) const
{
  Double_t te, th, thf;
  Double_t pe, ph, phf;
  datum.GetTagEnergies(respcorr, te, th, thf);
  datum.GetProbeEnergies(respcorr, pe, ph, phf);
  
  // calculate the resolution and balance in E_T, not E
  Double_t t=(te+th+thf)/std::cosh(datum.GetTagEta());
  Double_t p=(pe+ph+phf)/std::cosh(datum.GetProbeEta());
  //  Double_t dt=fEcalRes*fEcalRes*te+fHcalRes*fHcalRes*th+fHfRes*fHfRes*thf;
  //  Double_t dp=fEcalRes*fEcalRes*pe+fHcalRes*fHcalRes*ph+fHfRes*fHfRes*phf;
  Double_t dt = te+th+thf;
  Double_t dp = pe+ph+phf;
  
  balance_ = 2*(t-p)/(t+p);
  resolution_ = 4*t*p/(t+p)/(t+p)*sqrt(dt/t/t+dp/p/p);
  //  resolution_ = 1.0;
  return;
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
