
#include "Analysis/DataFormats/interface/TH1DEff.hh"

#include <iostream>
#include <cmath>

//////////////////////////////////////////////////////
//
// Implementation of TH1DEff
//
//////////////////////////////////////////////////////

TH1DEff::TH1DEff() : TH1D()
{
  numerator.Set(GetNbinsX()+2);
  denominator.Set(GetNbinsX()+2);
  numeratorSB.Set(GetNbinsX()+2);
  denominatorSB.Set(GetNbinsX()+2);
}

TH1DEff::TH1DEff(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup)
  : TH1D(name, title, nbinsx, xlow, xup)
{
  numerator.Set(GetNbinsX()+2);
  denominator.Set(GetNbinsX()+2);
  numeratorSB.Set(GetNbinsX()+2);
  denominatorSB.Set(GetNbinsX()+2);
}

TH1DEff::TH1DEff(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins)
  : TH1D(name, title, nbinsx, xbins)
{
  numerator.Set(GetNbinsX()+2);
  denominator.Set(GetNbinsX()+2);
  numeratorSB.Set(GetNbinsX()+2);
  denominatorSB.Set(GetNbinsX()+2);
}

TH1DEff::TH1DEff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins)
  : TH1D(name, title, nbinsx, xbins)
{
  numerator.Set(GetNbinsX()+2);
  denominator.Set(GetNbinsX()+2);
  numeratorSB.Set(GetNbinsX()+2);
  denominatorSB.Set(GetNbinsX()+2);
}

TH1DEff::~TH1DEff()
{
}

Int_t TH1DEff::Fill(Axis_t x, bool w)
{
  // fill everything as if it is in the signal region
  return TH1DEff::Fill(x, w, 1);
}

Int_t TH1DEff::Fill(Axis_t x, bool w, Int_t SBS)
{
  bool tagged=w;

  Int_t bin = fXaxis.FindBin(x);
  
  CheckBin(bin);

  if(SBS>0) {
    if(tagged)
      numerator[bin]+=1;
    denominator[bin]+=1;
  } else if(SBS<0) {
    if(tagged)
      numeratorSB[bin]+=1;
    denominatorSB[bin]+=1;    
  }
  CalcBinContent(bin);

  return bin;
}

void TH1DEff::Add(TF1* h1, Double_t c1, Option_t* option)
{
  Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
  TH1::Add(h1, c1, option);
}

void TH1DEff::Add(const TH1* h1, Double_t c1)
{
  if (!h1) {
    Error("Add","Attempt to add a non-existing histogram");
    return;
  }

  if(const TH1DEff* h1eff = dynamic_cast<const TH1DEff*>(h1)) {
    Int_t nbinsx = GetNbinsX();
    
    // Check histogram compatibility
    if (nbinsx != h1eff->GetNbinsX()) {
      Error("Add","Attempt to add histograms with different number of bins");
      return;
    }
    
    // Issue a Warning if histogram limits are different
    if (fXaxis.GetXmin() != h1eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h1eff->fXaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms with different axis limits");
    }
    
    for(Int_t bin=0; bin<=nbinsx+1; bin++) {
      numerator[bin]+=h1eff->GetBinNumerator(bin);
      denominator[bin]+=h1eff->GetBinDenominator(bin);
      numeratorSB[bin]+=h1eff->GetBinNumeratorSB(bin);
      denominatorSB[bin]+=h1eff->GetBinDenominatorSB(bin);

      CalcBinContent(bin);
    }

  } else {
    Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
    TH1::Add(h1, c1);
  }
  return; 
}

void TH1DEff::Add(const TH1* h1, const TH1* h2, Double_t c1, Double_t c2)
{
  if (!h1 || !h2) {
    Error("Add","Attempt to add a non-existing histogram");
    return;
  }
  const TH1DEff* h1eff = dynamic_cast<const TH1DEff*>(h1);
  const TH1DEff* h2eff = dynamic_cast<const TH1DEff*>(h2);
  if(h1eff && h2eff) {

    Int_t nbinsx = GetNbinsX();

    // Check histogram compatibility
    if (nbinsx != h1eff->GetNbinsX() || nbinsx != h2eff->GetNbinsX()) {
      Error("Add","Attempt to add histograms with different number of bins");
      return;
    }

    // Issue a Warning if histogram limits are different
    if (fXaxis.GetXmin() != h1eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h1eff->fXaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms with different axis limits");
    }
    if (fXaxis.GetXmin() != h2eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h2eff->fXaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms::Add with different axis limits");
    }
    
    for(Int_t bin=0; bin<=nbinsx+1; bin++) {
      numerator[bin]=h1eff->GetBinNumerator(bin)+h2eff->GetBinNumerator(bin);
      denominator[bin]=h1eff->GetBinDenominator(bin)+h2eff->GetBinDenominator(bin);
      numeratorSB[bin]=h1eff->GetBinNumeratorSB(bin)+h2eff->GetBinNumeratorSB(bin);
      denominatorSB[bin]=h1eff->GetBinDenominatorSB(bin)+h2eff->GetBinDenominatorSB(bin);
      
      CalcBinContent(bin);
    }

  } else {
    Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
    TH1::Add(h1, h2, c1, c2);
  }
  return;
}

Int_t TH1DEff::GetBinNumerator(Int_t bin) const
{
  CheckBin(bin);
  return numerator[bin];
}

Int_t TH1DEff::GetBinDenominator(Int_t bin) const
{
  CheckBin(bin);
  return denominator[bin];
}

Int_t TH1DEff::GetBinNumeratorSB(Int_t bin) const
{
  CheckBin(bin);
  return numeratorSB[bin];
}

Int_t TH1DEff::GetBinDenominatorSB(Int_t bin) const
{
  CheckBin(bin);
  return denominatorSB[bin];
}

TH1D* TH1DEff::GetDenominatorHist(TString newname)
{
  TH1D* hist = dynamic_cast<TH1D*>(TH1D::Clone(newname));

  Int_t nbinsx = GetNbinsX();
  for(Int_t bin=0; bin<=nbinsx+1; bin++) {
    Int_t den = GetBinDenominator(bin);
    hist->SetBinContent(bin, den);
    hist->SetBinError(bin, sqrt(den));
  }
  return hist;
}

Int_t TH1DEff::SetBinNumerator(Int_t bin, Int_t val)
{
  CheckBin(bin);
  numerator[bin]=val;
  CalcBinContent(bin);
  return bin;
}

Int_t TH1DEff::SetBinDenominator(Int_t bin, Int_t val)
{
  CheckBin(bin);
  denominator[bin]=val;
  CalcBinContent(bin);
  return bin;
}

Int_t TH1DEff::CheckBin(Int_t &bin) const
{
  // arrays have a binning such that:
  // array[0] = underflow
  // array[1] = bin 1
  // array[GetNbinsX()] = bin GetNbinsX()
  // array[GetNbinsX()+1] = overflow

  // underflow
  if(bin<0)
    bin = 0;

  // overflow
  if(bin>GetNbinsX()+1)
    bin = GetNbinsX()+1;

  return bin;
}

void TH1DEff::CalcBinContent(Int_t bin)
{
  CheckBin(bin);
  double num=numerator[bin];
  double den=denominator[bin];
  double numSB=numeratorSB[bin];
  double denSB=denominatorSB[bin];

  double eff=0, err=0;
  if((den-denSB)!=0) {
    eff = (num-numSB)/(den-denSB);
    err = sqrt(((1-2*eff)*(num+numSB)+eff*eff*(den+denSB))/((den-denSB)*(den-denSB)));
  }

  // Reset the kCanRebin option. Otherwise SetBinContent on the overflow bin
  // would resize the axis limits!
  ResetBit(kCanRebin);

  TH1D::SetBinContent(bin, eff);
  TH1D::SetBinError(bin, err);
}

//////////////////////////////////////////////////////
//
// Implementation of TH3DEff
//
//////////////////////////////////////////////////////

TH3DEff::TH3DEff() :TH3D()
{
  numerator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
  denominator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
}

TH3DEff::TH3DEff(const char* name, const char* title, Int_t nbinsx, Axis_t xlow, Axis_t xup, Int_t nbinsy, Axis_t ylow, Axis_t yup, Int_t nbinsz, Axis_t zlow, Axis_t zup)
  : TH3D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup)
{
  numerator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
  denominator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
}

TH3DEff::TH3DEff(const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins)
  : TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins)
{
  numerator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
  denominator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
}

TH3DEff::TH3DEff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins)
  : TH3D(name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins)
{
  numerator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
  denominator.Set((GetNbinsX()+2)*(GetNbinsY()+2)*(GetNbinsZ()+2));
}


TH3DEff::~TH3DEff()
{
}

Int_t TH3DEff::Fill(Axis_t x, Axis_t y, Axis_t z, bool w)
{
  bool tagged = w;

  Int_t binx = fXaxis.FindBin(x);
  Int_t biny = fYaxis.FindBin(y);
  Int_t binz = fZaxis.FindBin(z);

  int bin = CheckBin(binx, biny, binz);

  if(tagged)
    numerator[bin]++;
  denominator[bin]++;

  CalcBinContent(binx, biny, binz);

  return bin;
}

void TH3DEff::Add(TF1* h1, Double_t c1, Option_t* option)
{
  Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
  TH1::Add(h1, c1, option);
}

void TH3DEff::Add(const TH1* h1, Double_t c1)
{
  if (!h1) {
    Error("Add","Attempt to add a non-existing histogram");
    return;
  }

  if(const TH3DEff* h1eff = dynamic_cast<const TH3DEff*>(h1)) {
    Int_t nbinsx = GetNbinsX();
    Int_t nbinsy = GetNbinsY();
    Int_t nbinsz = GetNbinsZ();
    
    // Check histogram compatibility
    if (nbinsx != h1eff->GetNbinsX() ||
	nbinsy != h1eff->GetNbinsY() ||
	nbinsz != h1eff->GetNbinsZ()) {
      Error("Add","Attempt to add histograms with different number of bins");
      return;
    }

    // Issue a Warning if histogram limits are different
    if (fXaxis.GetXmin() != h1eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h1eff->fXaxis.GetXmax() ||
	fYaxis.GetXmin() != h1eff->fYaxis.GetXmin() ||
	fYaxis.GetXmax() != h1eff->fYaxis.GetXmax() ||
	fZaxis.GetXmin() != h1eff->fZaxis.GetXmin() ||
	fZaxis.GetXmax() != h1eff->fZaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms with different axis limits");
    }
    
    for(Int_t binx=0; binx<=nbinsx+1; binx++)
      for(Int_t biny=0; biny<=nbinsy+1; biny++)
	for(Int_t binz=0; binz<=nbinsz+1; binz++) {
	  
	  Int_t bin = CheckBin(binx, biny, binz);
	  
	  numerator[bin]+=h1eff->GetBinNumerator(binx, biny, binz);
	  denominator[bin]+=h1eff->GetBinDenominator(binx, biny, binz);
	  
	  CalcBinContent(binx, biny, binz);
	}

  } else {
    Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
    TH1::Add(h1, c1);
  }
  return;
}

void TH3DEff::Add(const TH1* h1, const TH1* h2, Double_t c1, Double_t c2)
{
  if (!h1 || !h2) {
    Error("Add","Attempt to add a non-existing histogram");
    return;
  }

  const TH3DEff* h1eff = dynamic_cast<const TH3DEff*>(h1);
  const TH3DEff* h2eff = dynamic_cast<const TH3DEff*>(h2);
  if(h1eff && h2eff) {
    Int_t nbinsx = GetNbinsX();
    Int_t nbinsy = GetNbinsY();
    Int_t nbinsz = GetNbinsZ();
    
    // Check histogram compatibility
    if (nbinsx != h1eff->GetNbinsX() || nbinsx != h2eff->GetNbinsX() ||
	nbinsy != h1eff->GetNbinsY() || nbinsy != h2eff->GetNbinsY() ||
	nbinsz != h1eff->GetNbinsZ() || nbinsz != h2eff->GetNbinsZ()) {
      Error("Add","Attempt to add histograms with different number of bins");
      return;
    }
    
    // Issue a Warning if histogram limits are different
    if (fXaxis.GetXmin() != h1eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h1eff->fXaxis.GetXmax() ||
	fYaxis.GetXmin() != h1eff->fYaxis.GetXmin() ||
	fYaxis.GetXmax() != h1eff->fYaxis.GetXmax() ||
	fZaxis.GetXmin() != h1eff->fZaxis.GetXmin() ||
	fZaxis.GetXmax() != h1eff->fZaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms with different axis limits");
    }
    if (fXaxis.GetXmin() != h2eff->fXaxis.GetXmin() ||
	fXaxis.GetXmax() != h2eff->fXaxis.GetXmax() ||
	fYaxis.GetXmin() != h2eff->fYaxis.GetXmin() ||
	fYaxis.GetXmax() != h2eff->fYaxis.GetXmax() ||
	fZaxis.GetXmin() != h2eff->fZaxis.GetXmin() ||
	fZaxis.GetXmax() != h2eff->fZaxis.GetXmax()) {
      Warning("Add","Attempt to add histograms with different axis limits");
    }
    
    for(Int_t binx=0; binx<=nbinsx+1; binx++)
      for(Int_t biny=0; biny<=nbinsy+1; biny++)
	for(Int_t binz=0; binz<=nbinsz+1; binz++) {
	  
	  Int_t bin = CheckBin(binx, biny, binz);
	  
	  numerator[bin]+=h1eff->GetBinNumerator(binx, biny, binz)+h2eff->GetBinNumerator(binx, biny, binz);
	  denominator[bin]+=h1eff->GetBinDenominator(binx, biny, binz)+h2eff->GetBinDenominator(binx, biny, binz);
	  
	  CalcBinContent(binx, biny, binz);
	}

  } else {
    Warning("Add","This will simply do a linear add.  Are you sure this is what you want to do?");
    TH1::Add(h1, h2, c1, c2);
  }
  return;
}

Int_t TH3DEff::GetBinNumerator(Int_t binx, Int_t biny, Int_t binz) const
{
  Int_t bin = CheckBin(binx, biny, binz);
  return numerator[bin];
}

Int_t TH3DEff::GetBinDenominator(Int_t binx, Int_t biny, Int_t binz) const
{
  Int_t bin = CheckBin(binx, biny, binz);
  return denominator[bin];
}

Int_t TH3DEff::SetBinNumerator(Int_t binx, Int_t biny, Int_t binz, Int_t val)
{
  Int_t bin = CheckBin(binx, biny, binz);
  numerator[bin]=val;
  CalcBinContent(binx, biny, binz);
  return bin;
}

Int_t TH3DEff::SetBinDenominator(Int_t binx, Int_t biny, Int_t binz, Int_t val)
{
  Int_t bin = CheckBin(binx, biny, binz);
  denominator[bin]=val;
  CalcBinContent(binx, biny, binz);
  return bin;
}


Int_t TH3DEff::CheckBin(Int_t &binx, Int_t &biny, Int_t &binz) const
{
  // arrays have a binning such that:
  // array[0] = underflow
  // array[1] = bin 1
  // array[GetNbinsX()] = bin GetNbinsX()
  // array[GetNbinsX()+1] = overflow

  Int_t nbinx = GetNbinsX();
  Int_t nbiny = GetNbinsY();
  Int_t nbinz = GetNbinsZ();

  // underflow
  if(binx<0) binx = 0;
  if(biny<0) biny = 0;
  if(binz<0) binz = 0;

  // overflow
  if(binx>nbinx+1) binx = nbinx+1;
  if(biny>nbiny+1) biny = nbiny+1;
  if(binz>nbinz+1) binz = nbinz+1;

  return binx + (nbinx + 2)*(biny + (nbiny + 2)*binz);
}

void TH3DEff::CalcBinContent(Int_t binx, Int_t biny, Int_t binz)
{
  Int_t bin = CheckBin(binx, biny, binz);

  int numval=numerator[bin];
  int denval=denominator[bin];
  double eff = denval==0 ? 0 : double(numval)/double(denval);
  double err = denval==0 ? 0 : sqrt(eff*(1-eff)/denval);

  // Reset the kCanRebin option. Otherwise SetBinContent on the overflow bin
  // would resize the axis limits!
  ResetBit(kCanRebin);

  TH3D::SetBinContent(binx, biny, binz, eff);
  TH3D::SetBinError(binx, biny, binz, err);

  return;
}

ClassImp(TH1DEff)
ClassImp(TH3DEff)
