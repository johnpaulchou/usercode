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
#include <vector>

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

  Double_t GetResolution(void) const;
  Double_t GetTagHcalEt(Int_t ieta) const;
  Double_t GetTagEcalEt(void) const;
  Double_t GetProbeHcalEt(Int_t ieta) const;
  Double_t GetProbeEcalEt(void) const;

  void SetResolution(Double_t);
  void SetTagHcalEt(Double_t, Int_t ieta);
  void SetTagEcalEt(Double_t);
  void SetProbeHcalEt(Double_t, Int_t ieta);
  void SetProbeEcalEt(Double_t);

 private:
  Double_t fResolution;
  TArrayD fTagHcalEt;
  Double_t fTagEcalEt;
  TArrayD fProbeHcalEt;
  Double_t fProbeEcalEt;

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
  TArrayD doFit(const TArrayD& respCorrInit, Int_t maxIetaFixed);
  TArrayD doFit(void); // use default resp corrections

  // fitting parameters
  inline void SetPrintLevel(Int_t p) { fPrintLevel=p; }
  void SetParStep(Double_t p) { fParStep=p; }
  void SetParMin(Double_t p) { fParMin=p; }
  void SetParMax(Double_t p) { fParMax=p; }

 private:
  // actual data
  std::vector<DijetRespCorrDatum> fData;

  // this is where the magic happens
  static void FCN(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t flag);

  // fit parameters
  Int_t fPrintLevel;
  Double_t fParStep;
  Double_t fParMin;
  Double_t fParMax;

  ClassDef(DijetRespCorrData, 1);
};

#endif

