#ifndef __DIJET_RESP_CORR_DATA_H__
#define __DIJET_RESP_CORR_DATA_H__

//
// DijetRespCorrData.h
//
//    description: Object which contains data relevant to dijet response corrections
//
//

#include "TObject.h"
#include "Rtypes.h"
#include "TArrayD.h"

#include <map>
#include <vector>

//
// forward declarations
//

class TH1D;
class TH2D;

//
// numerical constants
//

const int MAXIETA = 41;
const int NUMTOWERS = 83;
  

//
// class definitions
//

// container class for a dijet event
// includes the resolution, tag/probe hcal et, and tag/probe ecal et
class DijetRespCorrDatum : public TObject
{
 public:
  DijetRespCorrDatum();
  ~DijetRespCorrDatum();

  Double_t GetTagEta(void) const;
  Double_t GetTagPhi(void) const;
  Double_t GetTagHcalE(Int_t ieta);
  void     GetTagHcalE(std::map<Int_t, Double_t>&) const;
  Double_t GetTagEcalE(void) const;
  Double_t GetProbeEta(void) const;
  Double_t GetProbePhi(void) const;
  Double_t GetProbeHcalE(Int_t ieta);
  void     GetProbeHcalE(std::map<Int_t, Double_t>&) const;
  Double_t GetProbeEcalE(void) const;
  Double_t GetThirdJetPx(void) const;
  Double_t GetThirdJetPy(void) const;
  
  void SetTagEta(Double_t);
  void SetTagPhi(Double_t);
  void SetTagHcalE(Double_t, Int_t ieta);
  void AddTagHcalE(Double_t, Int_t ieta);
  void SetTagEcalE(Double_t);
  void SetProbeEta(Double_t);
  void SetProbePhi(Double_t);
  void SetProbeHcalE(Double_t, Int_t ieta);
  void AddProbeHcalE(Double_t, Int_t ieta);
  void SetProbeEcalE(Double_t);
  void SetThirdJetPx(Double_t);
  void SetThirdJetPy(Double_t);

  // calculate the ecal, hcal, and HF energy in the tag and probe jets
  // scale the hcal and hf energy by the response corrections
  void GetTagEnergies(const TArrayD& respcorr, Double_t& ecal, Double_t& hcal, Double_t& hf) const;
  void GetProbeEnergies(const TArrayD& respcorr, Double_t& ecal, Double_t& hcal, Double_t& hf) const;

 private:
  // tag jet info
  Double_t fTagEta, fTagPhi;
  std::map<Int_t, Double_t> fTagHcalE;
  Double_t fTagEcalE;

  // probe jet info
  Double_t fProbeEta, fProbePhi;
  std::map<Int_t, Double_t> fProbeHcalE;
  Double_t fProbeEcalE;

  // third jet info
  Double_t fThirdJetPx, fThirdJetPy;
  
  ClassDef(DijetRespCorrDatum, 1);
};

// vector of DijetRespCorrDatum
// uses ROOT style accessors, since ROOT doesn't like the STD
// provide a fitting tool to determine the response corrections
class DijetRespCorrData : public TObject
{
 public:
  DijetRespCorrData();
  ~DijetRespCorrData();

  void push_back(const DijetRespCorrDatum&);
  const DijetRespCorrDatum& GetAt(Int_t) const;
  Int_t GetSize(void) const;

  // calculate the total distance between the tag and probe jets
  // given a set of response corrections
  Double_t GetLikelihoodDistance(const TArrayD& respcorr) const;

  // fit for the response corrections
  //  TArrayD doFit(const TArrayD& respCorrInit, Int_t maxIetaFixed);
  void doFit(TArrayD& respcorr, TArrayD& respcorre); // use default resp corrections
  TH1D* doFit(const char* name, const char* title); // use default resp corrections

  // fitting parameters
  inline void SetPrintLevel(Int_t p) { fPrintLevel=p; }
  inline void SetParStep(Double_t p) { fParStep=p; }
  inline void SetParMin(Double_t p) { fParMin=p; }
  inline void SetParMax(Double_t p) { fParMax=p; }
  inline void SetEcalRes(Double_t p=0.07) { fEcalRes=p; }
  inline void SetHcalRes(Double_t p=1.15) { fHcalRes=p; }
  inline void SetHfRes(Double_t p=1.35) { fHfRes=p; }

 private:
  // calculate the balance parameter and its resolution for a given dijet pair
  void GetBalance(const DijetRespCorrDatum& datum, const TArrayD& respcorr, Double_t& balance_, Double_t& resolution_) const;

  // actual data
  std::vector<DijetRespCorrDatum> fData;

  // this is where the magic happens
  static void FCN(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t flag);

  // fit parameters
  Int_t fPrintLevel;
  Double_t fParStep;
  Double_t fParMin;
  Double_t fParMax;
  Double_t fEcalRes, fHcalRes, fHfRes;

  ClassDef(DijetRespCorrData, 1);
};

#endif

