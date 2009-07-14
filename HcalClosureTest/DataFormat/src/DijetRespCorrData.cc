#include "HcalClosureTest/DataFormat/interface/DijetRespCorrData.h"

#include <cassert>


ClassImp(DijetRespCorrDatum)
ClassImp(DijetRespCorrData)

DijetRespCorrDatum::DijetRespCorrDatum() {
  fResolution=fTagEcalEt=fProbeEcalEt=0.0;
  fTagHcalEt.Set(GetNumTowers());
  fProbeHcalEt.Set(GetNumTowers());
}

DijetRespCorrDatum::~DijetRespCorrDatum() {}

Double_t DijetRespCorrDatum::GetResolution(void) const
{
  return fResolution;
}

Double_t DijetRespCorrDatum::GetTagHcalEt(Int_t ieta) const
{
  assert(ieta<=GetMaxIeta() && ieta>=-GetMaxIeta() && ieta!=0);
  Int_t index = ieta+GetMaxIeta();
  return fTagHcalEt.GetAt(index);
}

Double_t DijetRespCorrDatum::GetTagEcalEt(void) const
{
  return fTagEcalEt;
}

Double_t DijetRespCorrDatum::GetProbeHcalEt(Int_t ieta) const
{
  assert(ieta<=GetMaxIeta() && ieta>=-GetMaxIeta() && ieta!=0);
  Int_t index = ieta+GetMaxIeta();
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
  assert(ieta<=GetMaxIeta() && ieta>=-GetMaxIeta() && ieta!=0);
  Int_t index = ieta+GetMaxIeta();
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
  assert(ieta<=GetMaxIeta() && ieta>=-GetMaxIeta() && ieta!=0);
  Int_t index = ieta+GetMaxIeta();
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
