

void zbi(int obs, double back, double backe) {
  double zbi=obs/sqrt(back+backe*backe);
  if(zbi>=obs) zbi=obs;
    return zbi;
}

void etaopt(int masscut)
{
  TFile f("opthists.root");
  double lumi=500.;
  TH2D* hSig2D=dynamic_cast<TH2D*>(gROOT->FindObject("hMS2500NED4KK1"));
  TH2D* hBac2D=dynamic_cast<TH2D*>(gROOT->FindObject("hMS100000NED4KK1"));

  // project
  TH1D* hSig=hSig2D->ProjectionX("_py", masscut, hSig2D->GetNbinsY()+1);
  TH1D* hBac=hBac2D->ProjectionX("_px", masscut, hBac2D->GetNbinsY()+1);

  // cdf
  for(int bin=hSig->GetNbinsX(); bin>=0; bin--) {
    hSig->SetBinContent(bin, hSig->GetBinContent(bin)+hSig->GetBinContent(bin+1));
    hBac->SetBinContent(bin, hBac->GetBinContent(bin)+hBac->GetBinContent(bin+1));
  }
  TCanvas *c=new TCanvas();
  c->Divide(2,2);
  c->cd(1);
  hSig->Draw();
  c->cd(2);
  hBac->Draw();

  
  




}
