#include "Analysis/DataFormats/interface/DijetKinHist.hh"

#include <iostream>

DijetKinHist::DijetKinHist(void) :
  HistArray()
{
}

DijetKinHist::DijetKinHist(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins) :
  HistArray(name, title, nbinsx, xbins, NTAGLEVELS*NDATASETS*NHFCONTENTS)
{
}

DijetKinHist::DijetKinHist(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins) :
  HistArray(name, title, nbinsx, xbins, NTAGLEVELS*NDATASETS*NHFCONTENTS)
{
}

DijetKinHist::DijetKinHist(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup) :
  HistArray(name, title, nbinsx, xlow, xup, NTAGLEVELS*NDATASETS*NHFCONTENTS)
{
}

Int_t DijetKinHist::GetBiny(dataset d, taglevel t, hfcontent h)
{
  if(d==DATA) h=HF;
  Int_t bin1 = static_cast<Int_t>(t);
  Int_t bin2 = static_cast<Int_t>(d);
  Int_t bin3 = static_cast<Int_t>(h);
  Int_t nbins1 = static_cast<Int_t>(NTAGLEVELS);
  Int_t nbins2 = static_cast<Int_t>(NDATASETS);
  return bin1+nbins1*bin2+nbins1*nbins2*bin3;
}

Int_t DijetKinHist::GetBin(Double_t x, dataset d, taglevel t, hfcontent h)
{
  return HistArray::GetBin(x, GetBiny(d, t, h));
}

Int_t DijetKinHist::Fill(Double_t x, dataset d, taglevel t, hfcontent h, Double_t weight)
{
  Int_t bin = GetBin(x, d, t, h);
  if(bin<0) return -1;

  HistArray::AddBinContent(bin, weight);
  return bin;
}

TH1D* DijetKinHist::GetHist(const char* name, dataset d, taglevel t, hfcontent h)
{
  return HistArray::GetHist(name, GetBiny(d, t, h));
}

TH1D* DijetKinHist::GetDataHist(const char* name, taglevel t)
{
  return HistArray::GetHist(name, GetBiny(DATA, t, HF));
}

TH1D* DijetKinHist::GetJetHist(const char* name, taglevel t, hfcontent h)
{
  TH1D* hist=HistArray::GetHist(name, GetBiny(JET30, t, h));

  TString tempname=name;
  tempname += "tempname";
  TH1D* h50=HistArray::GetHist(tempname+TString("50"), GetBiny(JET50, t, h));
  TH1D* h80=HistArray::GetHist(tempname+TString("80"), GetBiny(JET80, t, h));
  TH1D* h120=HistArray::GetHist(tempname+TString("120"), GetBiny(JET120, t, h));
  TH1D* h170=HistArray::GetHist(tempname+TString("170"), GetBiny(JET170, t, h));
  TH1D* h300=HistArray::GetHist(tempname+TString("300"), GetBiny(JET300, t, h));
  TH1D* h470=HistArray::GetHist(tempname+TString("470"), GetBiny(JET470, t, h));
  TH1D* h600=HistArray::GetHist(tempname+TString("600"), GetBiny(JET600, t, h));
  TH1D* h800=HistArray::GetHist(tempname+TString("800"), GetBiny(JET800, t, h));
  TH1D* h1000=HistArray::GetHist(tempname+TString("1000"), GetBiny(JET1000, t, h));
  TH1D* h1400=HistArray::GetHist(tempname+TString("1400"), GetBiny(JET1400, t, h));
  TH1D* h1800=HistArray::GetHist(tempname+TString("1800"), GetBiny(JET1800, t, h));

  for(int i=0; i<=hist->GetNbinsX()+1; i++) {
    hist->AddBinContent(i, h50->GetBinContent(i));
    hist->AddBinContent(i, h80->GetBinContent(i));
    hist->AddBinContent(i, h120->GetBinContent(i));
    hist->AddBinContent(i, h170->GetBinContent(i));
    hist->AddBinContent(i, h300->GetBinContent(i));
    hist->AddBinContent(i, h470->GetBinContent(i));
    hist->AddBinContent(i, h600->GetBinContent(i));
    hist->AddBinContent(i, h800->GetBinContent(i));
    hist->AddBinContent(i, h1000->GetBinContent(i));
    hist->AddBinContent(i, h1400->GetBinContent(i));
    hist->AddBinContent(i, h1800->GetBinContent(i));
  }

  delete h50;
  delete h80;
  delete h120;
  delete h170;
  delete h300;
  delete h470;
  delete h600;
  delete h800;
  delete h1000;
  delete h1400;
  delete h1800;

  return hist;
}

TH1D* DijetKinHist::GetDataPreTagHist(const char* name)
{
  TH1D* h=GetDataHist(name, TAG0);
  TString tempname=name;
  tempname += "tempname";
  TH1D* ht1=GetDataHist(tempname+TString("tag1"), TAG1);
  TH1D* ht2=GetDataHist(tempname+TString("tag2"), TAG2);

  for(int i=0; i<=h->GetNbinsX()+1; i++) {
    h->AddBinContent(i, ht1->GetBinContent(i));
    h->AddBinContent(i, ht2->GetBinContent(i));
  }

  delete ht1;
  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetDataSingleTagHist(const char* name)
{
  TH1D* h=GetDataHist(name, TAG1);
  TString tempname=name;
  tempname += "tempnametag2";
  TH1D* ht2=GetDataHist(tempname, TAG2);

  for(int i=0; i<=h->GetNbinsX()+1; i++)
    h->AddBinContent(i, ht2->GetBinContent(i));

  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetDataDoubleTagHist(const char* name)
{
  return GetDataHist(name, TAG2);
}

TH1D* DijetKinHist::GetHFJetPreTagHist(const char* name)
{
  TH1D* h=GetJetHist(name, TAG0, HF);
  TString tempname=name;
  tempname += "tempname";
  TH1D* ht1=GetJetHist(tempname+TString("tag1"), TAG1, HF);
  TH1D* ht2=GetJetHist(tempname+TString("tag2"), TAG2, HF);

  for(int i=0; i<=h->GetNbinsX()+1; i++) {
    h->AddBinContent(i, ht1->GetBinContent(i));
    h->AddBinContent(i, ht2->GetBinContent(i));
  }

  delete ht1;
  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetHFJetSingleTagHist(const char* name)
{
  TH1D* h=GetJetHist(name, TAG1, HF);
  TString tempname=name;
  tempname += "tempnametag2";
  TH1D* ht2=GetJetHist(tempname, TAG2, HF);

  for(int i=0; i<=h->GetNbinsX()+1; i++)
    h->AddBinContent(i, ht2->GetBinContent(i));

  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetHFJetDoubleTagHist(const char* name)
{
  return GetJetHist(name, TAG2, HF);
}

TH1D* DijetKinHist::GetLFJetPreTagHist(const char* name)
{
  TH1D* h=GetJetHist(name, TAG0, LF);
  TString tempname=name;
  tempname += "tempname";
  TH1D* ht1=GetJetHist(tempname+TString("tag1"), TAG1, LF);
  TH1D* ht2=GetJetHist(tempname+TString("tag2"), TAG2, LF);

  for(int i=0; i<=h->GetNbinsX()+1; i++) {
    h->AddBinContent(i, ht1->GetBinContent(i));
    h->AddBinContent(i, ht2->GetBinContent(i));
  }

  delete ht1;
  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetLFJetSingleTagHist(const char* name)
{
  TH1D* h=GetJetHist(name, TAG1, LF);
  TString tempname=name;
  tempname += "tempnametag2";
  TH1D* ht2=GetJetHist(tempname, TAG2, LF);

  for(int i=0; i<=h->GetNbinsX()+1; i++)
    h->AddBinContent(i, ht2->GetBinContent(i));

  delete ht2;
  return h;
}

TH1D* DijetKinHist::GetLFJetDoubleTagHist(const char* name)
{
  return GetJetHist(name, TAG2, LF);
}



ClassImp(DijetKinHist)

