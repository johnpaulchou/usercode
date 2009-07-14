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
// class definitions
//

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

  inline Int_t GetMaxIeta(void) const { return MAXIETA; }
  inline Int_t GetNumTowers(void) const { return MAXIETA*2+1; }

 private:
  static const Int_t MAXIETA=41;

  Double_t fResolution;
  TArrayD fTagHcalEt;
  Double_t fTagEcalEt;
  TArrayD fProbeHcalEt;
  Double_t fProbeEcalEt;

  ClassDef(DijetRespCorrDatum, 1);
};

class DijetRespCorrData : public TObject
{
 public:
  DijetRespCorrData();
  ~DijetRespCorrData();

  void push_back(const DijetRespCorrDatum&);
  const DijetRespCorrDatum& GetAt(Int_t) const;
  Int_t GetSize(void) const;

 private:
  std::vector<DijetRespCorrDatum> fData;

  ClassDef(DijetRespCorrData, 1);
};

#endif

