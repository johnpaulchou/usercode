TFile *rootfile=0;

void drawplots(int whichplot=3)
{
  switch(whichplot) {
  case 1:
    TCanvas *c=new TCanvas("photoneffetPAS","Photon Efficiency Eta PAS",600,600);
    drawplots(1,1,true,true);
    TCanvas *c=new TCanvas("photoneffetaPAS","Photon Efficiency Et PAS",600,600);
    drawplots(1,2,true,true);
    break;

  case 2:
    TCanvas *c=new TCanvas("photoneff","Photon Efficiencies",800,700);
    c->Divide(2,2);
    c->cd(1);
    drawplots(1,1);
    c->cd(2);
    drawplots(1,2);
    c->cd(3);
    drawplots(3,1);
    c->cd(4);
    drawplots(3,2);
    break;

  case 3:

    c=new TCanvas("photonisol","Photon Isolations",800,700);
    c->Divide(2,2);
    c->cd(1);
    compIsol(1);
    c->cd(2);
    compIsol(2);
    c->cd(3);
    compIsol(3);
    c->cd(4);
    compIsol(4);
    break;

  case 4:
    c=new TCanvas("jets","Jet Spectrum",800,350);
    c->Divide(2,1);
    c->cd(1);
    compIsol(5);
    c->cd(2);
    compIsol(6);
    break;
  }

  return;
}

void drawplots(int anatype, int varnum, bool showmarkers=false, bool PAS=false)
{
  TString var, vartitle;
  if(varnum==1) {
    var="Eta";
    vartitle="Gen Photon #eta";
  } else if(varnum==2) {
    var="Et";
    vartitle="Gen Photon E_{T} [GeV]";
  }

  if(anatype==1)
    rootfile=new TFile("results/born_plots.root");
  if(anatype==2)
    rootfile=new TFile("results/add_plots.root");
  if(anatype==3)
    rootfile=new TFile("results/add_noled_plots.root");

  TH1D* hEff1 = dynamic_cast<TH1D*>(gROOT->FindObject("h"+var+"Eff1"));
  TH1D* hEff2 = dynamic_cast<TH1D*>(gROOT->FindObject("h"+var+"Eff2"));
  TH1D* hEff3 = dynamic_cast<TH1D*>(gROOT->FindObject("h"+var+"Eff3"));
  TH1D* hEff4 = dynamic_cast<TH1D*>(gROOT->FindObject("h"+var+"Eff4"));

  if(anatype==1 && PAS) {
    hEff4->Fit("pol0");
    TF1* f1=dynamic_cast<TF1*>(gROOT->FindObject("pol0"));
  }

  hEff1->Draw();
  hEff2->Draw("same");
  hEff3->Draw("same");
  hEff4->Draw("same");

  hEff1->SetFillColor(0);
  hEff2->SetFillColor(0);
  hEff3->SetFillColor(0);
  hEff4->SetFillColor(0);

  hEff1->SetStats(0);
  hEff2->SetStats(0);
  hEff3->SetStats(0);
  hEff4->SetStats(0);

  hEff1->SetLineColor(kBlue+2);
  hEff2->SetLineColor(kRed+2);
  hEff3->SetLineColor(kCyan+2);
  hEff4->SetLineColor(kBlack);

  if(showmarkers) {
    hEff1->SetMarkerColor(kBlue+2);
    hEff2->SetMarkerColor(kRed+2);
    hEff3->SetMarkerColor(kCyan+2);
    hEff4->SetMarkerColor(kBlack);
    
    hEff1->SetMarkerStyle(21);
    hEff2->SetMarkerStyle(22);
    hEff3->SetMarkerStyle(23);
    hEff4->SetMarkerStyle(24);

    hEff1->SetMarkerSize(1.0);
    hEff2->SetMarkerSize(1.0);
    hEff3->SetMarkerSize(1.0);
    hEff4->SetMarkerSize(1.0);
  }

  hEff1->SetMinimum(0.0);
  hEff1->SetMaximum(1.05);

  hEff1->GetXaxis()->SetTitle(vartitle);
  hEff1->GetYaxis()->SetTitle("Efficiency");
  hEff1->GetYaxis()->SetTitleOffset(1.05);
  if(!PAS) {
    if(anatype==1)
      hEff1->SetTitle("Leading Photon Efficiency in Pythia (Born) Sample");
    else if(anatype==2)
      hEff1->SetTitle("Leading Photon Efficiency in Sherpa (ADD) Sample");
    else if(anatype==3)
      hEff1->SetTitle("Leading Photon Efficiency in Sherpa (ADD, M_S=1e5 TeV) Sample");
  } else {
    hEff1->SetTitle("");
    TText* text=new TLatex(0,0, "CMS Preliminary");
    text->SetTextFont(42);
    text->SetTextSize(0.05);
    text->SetNDC();
    text->SetX(.53);
    text->SetY(.86);
    text->Draw();
  }

  TLegend *leg=new TLegend(.30,.24,.50, .51);
  leg->AddEntry(hEff1, "Reconstruction Efficiency", "lp");
  leg->AddEntry(hEff2, "Pixel Veto Efficiency", "lp");
  leg->AddEntry(hEff3, "Isolation Efficiency", "lp");
  leg->AddEntry(hEff4, "Combined Efficiency", "lp");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetTextSize(0.050);
  leg->Draw();

}

TH1* GetPlot(int anatype, TString name)
{
  if(anatype==1)
    rootfile=new TFile("results/born_plots.root");
  else if(anatype==2)
    rootfile=new TFile("results/add_plots.root");
  else if(anatype==3)
    rootfile=new TFile("results/add_noled_plots.root");
  rootfile->cd();
  return dynamic_cast<TH1*>(gROOT->FindObject(name));
}

void compIsol(int varnum)
{
  TString varname, vartitle;
  if(varnum==1) {
    varname="hHEIsoPt";
    vartitle="H/E Isolation";
  }
  if(varnum==2) {
    varname="hHcalIsoPt";
    vartitle="HCAL Isolation";
  }
  if(varnum==3) {
    varname="hEcalIsoPt";
    vartitle="ECAL Isolation";
  }
  if(varnum==4) {
    varname="hTrkIsoPt";
    vartitle="Track Isolation";
  }
  if(varnum==5) {
    varname="hJetEt";
    vartitle="Jet E_{T} [GeV]";
  }
  if(varnum==6) {
    varname="hNJets";
    vartitle="Number of Jets";
  }
  
  
  TH1D* hBorn;
  TH1D* hADD;
  if(varnum<=4) {
    TH2D* hBorn2d = dynamic_cast<TH2D*>(GetPlot(1, varname));
    TH2D* hADD2d = dynamic_cast<TH2D*>(GetPlot(3, varname));
    hBorn = hBorn2d->ProjectionX(varname+"born");
    hADD = hADD2d->ProjectionX(varname+"add");
    cout << "asdf" << endl;
  } else {
    hBorn = dynamic_cast<TH1D*>(GetPlot(1, varname));
    hADD = dynamic_cast<TH1D*>(GetPlot(3, varname));
  }
  
  hBorn->Scale(1/hBorn->GetSumOfWeights());
  hADD->Scale(1/hADD->GetSumOfWeights());

  hBorn->SetLineColor(kBlue+2);
  hADD->SetLineColor(kCyan+4);
  hBorn->SetFillColor(0);
  hADD->SetFillColor(0);
  hBorn->SetLineStyle(1);
  hADD->SetLineStyle(2);
  
  //  hBorn->SetStats(0);
  //  hADD->SetStats(0);

  gPad->SetLogy(1);
  
  hBorn->GetXaxis()->SetTitle(vartitle);
  hBorn->GetYaxis()->SetTitle("Normalized Units");
  hBorn->SetTitle("Leading Photons in Pythia/Sherpa Samples");

  hBorn->Draw();
  hADD->Draw("sames");

  hBorn->SetMaximum(1);

  gPad->Update();
  TPaveStats *st1=(TPaveStats*)hBorn->GetListOfFunctions()->FindObject("stats");
  TPaveStats *st2=(TPaveStats*)hADD->GetListOfFunctions()->FindObject("stats");
  st1->SetName("Born");
  st2->SetName("ADD");
  st1->SetOptStat(101100);
  st2->SetOptStat(101100);
  st1->SetX1NDC(.25);
  st1->SetX2NDC(.55);
  st1->SetY1NDC(.56);
  st1->SetY2NDC(.80);
  st2->SetX1NDC(.56);
  st2->SetX2NDC(.86);
  st2->SetY1NDC(.56);
  st2->SetY2NDC(.80);

  TLegend* leg=new TLegend(.25,.78,.55,.85);
  leg->AddEntry(hBorn, "Pythia (Born)", "l");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetTextSize(0.045);
  leg->Draw();

  TLegend* leg=new TLegend(.56,.78,.86,.85);
  leg->AddEntry(hADD, "Sherpa (ADD)", "l");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetTextSize(0.045);
  leg->Draw();

  gPad->Update();
  

  return;
}
