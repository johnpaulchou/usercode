#ifndef __TH1DEFF_HH__
#define __TH1DEFF_HH__

#include "TArrayI.h"
#include "TH1D.h"
#include "TH3D.h"


// TH1DEff is a 1-dimension efficiency histogram
class TH1DEff : public TH1D {

 public:

  TH1DEff();
  TH1DEff(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup);
  TH1DEff(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins);
  TH1DEff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins);
  virtual ~TH1DEff();

  // use this fill function exclusively where w==1 means tagged, and w==0 means not tagged
  // SBS==1 means signal, SBS==-1 means Side-band, and SBS==0 means nothing
  virtual Int_t Fill(Axis_t x, bool w);
  virtual Int_t Fill(Axis_t x, bool w, Int_t SBS);

  virtual void Add(TF1* h1, Double_t c1 = 1, Option_t* option="");
  virtual void Add(const TH1* h1, Double_t c1 = 1);
  virtual void Add(const TH1* h, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1);

  virtual Int_t GetBinNumerator(Int_t bin) const;
  virtual Int_t GetBinDenominator(Int_t bin) const;
  virtual Int_t GetBinNumeratorSB(Int_t bin) const;
  virtual Int_t GetBinDenominatorSB(Int_t bin) const;

  // user is required to delete object
  TH1D* GetDenominatorHist(TString newname);

  virtual Int_t SetBinNumerator(Int_t bin, Int_t val);
  virtual Int_t SetBinDenominator(Int_t bin, Int_t val);

 protected:
  Int_t CheckBin(Int_t &bin) const;
  void CalcBinContent(Int_t bin);

  // disable these fill functions
  Int_t Fill(Axis_t) { return -1; }
  Int_t Fill(Axis_t, Stat_t) { return -1; }
  Int_t Fill(const char*, Stat_t) { return -1; }


 private:

  TArrayI numerator;
  TArrayI denominator;
  TArrayI numeratorSB;
  TArrayI denominatorSB;

  ClassDef(TH1DEff,1)

};


class TH3DEff : public TH3D {

 public:

  TH3DEff();
  TH3DEff(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup, Int_t nbinsy, Axis_t ylow, Axis_t yup, Int_t nbinsz, Axis_t zlow, Axis_t zup);
  TH3DEff(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins);
  TH3DEff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins);
  virtual ~TH3DEff();


  // use this fill function exclusively where w==true means tagged, and w==false means not tagged
  virtual Int_t Fill(Axis_t x, Axis_t y, Axis_t z, bool w);

  virtual void Add(TF1* h1, Double_t c1 = 1, Option_t* option="");
  virtual void Add(const TH1* h1, Double_t c1 = 1);
  virtual void Add(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1);
  
  virtual Int_t GetBinNumerator(Int_t binx, Int_t biny, Int_t binz) const;
  virtual Int_t GetBinDenominator(Int_t binx, Int_t biny, Int_t binz) const;

  virtual Int_t SetBinNumerator(Int_t binx, Int_t biny, Int_t binz, Int_t val);
  virtual Int_t SetBinDenominator(Int_t binx, Int_t biny, Int_t binz, Int_t val);

 protected:
  Int_t CheckBin(Int_t &binx, Int_t &biny, Int_t &binz) const;
  void CalcBinContent(Int_t binx, Int_t biny, Int_t binz);


 private:

  TArrayI numerator;
  TArrayI denominator;

  // disable these functions
  Int_t Fill(Axis_t) {return -1;}                              // MayNotUse
  Int_t Fill(Axis_t, Stat_t) {return -1;}                      // MayNotUse
  Int_t Fill(const char*, Stat_t) {return -1;}                 // MayNotUse
  Int_t Fill(Axis_t, const char*, Stat_t) {return -1;}         // MayNotUse
  Int_t Fill(const char*, Axis_t, Stat_t) {return -1;}         // MayNotUse
  Int_t Fill(const char*, const char*, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(Axis_t, Axis_t, Axis_t) {return -1;}        // MayNotUse
  Int_t Fill(Axis_t, Axis_t, Axis_t, Stat_t) {return -1;}     // MayNotUse
  Int_t Fill(const char*, const char*, const char*, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(const char*, Axis_t, const char*, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(const char*, const char*, Axis_t, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(Axis_t, const char*, const char*, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(Axis_t, const char*, Axis_t, Stat_t) {return -1;}    // MayNotUse
  Int_t Fill(Axis_t, Axis_t, const char*, Stat_t) {return -1;}    // MayNotUse

  ClassDef(TH3DEff,1)

};

#endif
