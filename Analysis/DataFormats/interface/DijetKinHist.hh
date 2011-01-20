#ifndef __DIJET_KIN_HIST_HH__
#define __DIJET_KIN_HIST_HH__

#include "HistArray.hh"

class DijetKinHist : public HistArray
{
 public:
  enum dataset { DATA=0, JET30, JET50, JET80, JET120, JET170, JET300, JET470, JET600, JET800, JET1000, JET1400, JET1800, NDATASETS };
  enum taglevel { TAG0=0, TAG1, TAG2, NTAGLEVELS };
  enum hfcontent { HF=0, LF, NHFCONTENTS };

  DijetKinHist(void);
  DijetKinHist(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins);
  DijetKinHist(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins);
  DijetKinHist(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup);

  // helper functions
  Int_t GetBin(Double_t x, dataset d, taglevel t, hfcontent h);

  // fill stuff here
  Int_t Fill(Double_t x, dataset d, taglevel t, hfcontent h, Double_t weight=1.0);

  // get histograms (N.B.: it is up to the user to delete them)
  TH1D* GetHist(const char* name, dataset d, taglevel t, hfcontent h);

  TH1D* GetDataHist(const char* name, taglevel t);
  TH1D* GetJetHist(const char* name, taglevel t, hfcontent h);

  TH1D* GetDataPreTagHist(const char* name);
  TH1D* GetDataSingleTagHist(const char* name);
  TH1D* GetDataDoubleTagHist(const char* name);

  TH1D* GetHFJetPreTagHist(const char* name);
  TH1D* GetHFJetSingleTagHist(const char* name);
  TH1D* GetHFJetDoubleTagHist(const char* name);

  TH1D* GetLFJetPreTagHist(const char* name);
  TH1D* GetLFJetSingleTagHist(const char* name);
  TH1D* GetLFJetDoubleTagHist(const char* name);


protected:

  virtual Int_t GetBin(Double_t, Int_t) { return -1; }
  virtual Int_t Fill(Double_t, Int_t, Double_t) { return -1; }
  virtual TH1D* GetHist(const char*, Int_t) { return 0; }


private:

  Int_t GetBiny(dataset d, taglevel t, hfcontent h);

  ClassDef(DijetKinHist, 1)
};

#endif

