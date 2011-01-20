#ifndef __HIST_ARRAY_HH__
#define __HIST_ARRAY_HH__

#include <TArrayD.h>
#include <TNamed.h>
#include <TAxis.h>
#include <TH1D.h>

class HistArray : public TArrayD, public TNamed
{
 public:

  HistArray(void);
  HistArray(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy);
  HistArray(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy);
  HistArray(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy);

 // helper functions
  virtual Int_t GetBin(Double_t x, Int_t biny);
  virtual Int_t GetNbinsX(void) const;
  virtual Int_t GetNbinsY(void) const;

  // fill stuff here
  virtual Int_t Fill(Double_t x, Int_t biny, Double_t weight=1.0);

  // access content directly
  virtual void AddBinContent(Int_t bin, Double_t content);
  virtual void SetBinContent(Int_t bin, Double_t content);
  virtual Double_t GetBinContent(Int_t bin) const;

  // get histograms (N.B.: it is up to the user to delete them)
  virtual TH1D* GetHist(const char* name, Int_t biny);

 protected:

  virtual Int_t GetBin_(Int_t binx, Int_t biny) const;

  // members
  TAxis fXaxis;
  Int_t fNbinsY;
  Int_t fNcells;

  ClassDef(HistArray, 1)

};


#endif
