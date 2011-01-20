#include <TFile.h>
#include <TObject.h>
#include <TH1D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TStyle.h>

#include "Analysis/DataFormats/interface/DijetKinHist.hh"

#include <iostream>

TFile* rootfile;
const std::string filename="../data/plots.root";

double GetLuminosity(void);
void drawDijetKinHist(TString name, TString xlabel, TString ylabel, int whichtags, bool doLogScale);
DijetKinHist* GetDijetKinHist(TString name);
void drawplots(void);

double GetLuminosity(void)
{
  rootfile=new TFile(filename.c_str());
  rootfile->cd();
  TH1D* h = dynamic_cast<TH1D*>(gROOT->FindObject("hLumi"));
  if(h==0) {
    std::cout << "Could not find luminosity histogram in " << filename << "." << std::endl;
    return 0;
  }
  return h->GetBinContent(1);
}

DijetKinHist* GetDijetKinHist(TString name)
{
  rootfile=new TFile(filename.c_str());
  rootfile->cd();
  TObject* obj = gROOT->FindObject(name);
  if(!obj) {
    std::cout << "Could not find an object named " << name << std::endl;
    return 0;
  }
  DijetKinHist* h=dynamic_cast<DijetKinHist*>(obj);
  if(!h) {
    std::cout << "Could not find a DijetKinHist named " << name << std::endl;
    return 0;
  }
  return h;
}

void drawplots(void)
{
  drawDijetKinHist("hDijetMass", "Dijet Mass [GeV]", "Events/25 GeV", 0, true);
  drawDijetKinHist("hDijetMass", "Dijet Mass [GeV]", "Events/25 GeV", 1, true);
  drawDijetKinHist("hDijetMass", "Dijet Mass [GeV]", "Events/25 GeV", 2, false);
  drawDijetKinHist("hDijetMass", "Dijet Mass [GeV]", "Events/25 GeV", 2, true);

  drawDijetKinHist("hLeadJetPt", "Leading Jet p_{T} [GeV]", "Jets/20 GeV",0,true);
  drawDijetKinHist("hLeadJetPt", "Leading Jet p_{T} [GeV]", "Jets/20 GeV",1,true);
  drawDijetKinHist("hLeadJetPt", "Leading Jet p_{T} [GeV]", "Jets/20 GeV",2,true);

  drawDijetKinHist("hLeadJetEta", "Leading Jet #eta", "Jets/0.1",0,false);
  drawDijetKinHist("hLeadJetEta", "Leading Jet #eta", "Jets/0.1",1,false);
  drawDijetKinHist("hLeadJetEta", "Leading Jet #eta", "Jets/0.1",2,false);

  drawDijetKinHist("hSubLeadJetEta", "Sub-Leading Jet #eta", "Jets/0.1",0,false);
  drawDijetKinHist("hSubLeadJetEta", "Sub-Leading Jet #eta", "Jets/0.1",1,false);
  drawDijetKinHist("hSubLeadJetEta", "Sub-Leading Jet #eta", "Jets/0.1",2,false);

  drawDijetKinHist("hLeadJetPhi", "Leading Jet #phi", "Jets/0.126",0,false);
  drawDijetKinHist("hLeadJetPhi", "Leading Jet #phi", "Jets/0.126",1,false);
  drawDijetKinHist("hLeadJetPhi", "Leading Jet #phi", "Jets/0.126",2,false);

  drawDijetKinHist("hSubLeadJetPhi", "Sub-Leading Jet #phi", "Jets/0.126",0,false);
  drawDijetKinHist("hSubLeadJetPhi", "Sub-Leading Jet #phi", "Jets/0.126",1,false);
  drawDijetKinHist("hSubLeadJetPhi", "Sub-Leading Jet #phi", "Jets/0.126",2,false);

  drawDijetKinHist("hMET", "Calo #slash{E}_{T} [GeV]", "Events/10 GeV",0,true);
  drawDijetKinHist("hMET", "Calo #slash{E}_{T} [GeV]", "Events/10 GeV",1,true);
  drawDijetKinHist("hMET", "Calo #slash{E}_{T} [GeV]", "Events/10 GeV",2,true);

  drawDijetKinHist("hMETSig", "Calo #slash{E}_{T}/#SigmaE_{T}", "Events/0.5",0,true);
  drawDijetKinHist("hMETSig", "Calo #slash{E}_{T}/#SigmaE_{T}", "Events/0.5",1,true);
  drawDijetKinHist("hMETSig", "Calo #slash{E}_{T}/#SigmaE_{T}", "Events/0.5",2,true);

  drawDijetKinHist("hDijetDphi", "Dijet |#Delta#phi|", "Events/0.0628",0,true);
  drawDijetKinHist("hDijetDphi", "Dijet |#Delta#phi|", "Events/0.0628",1,true);
  drawDijetKinHist("hDijetDphi", "Dijet |#Delta#phi|", "Events/0.0628",2,true);

  drawDijetKinHist("hMuonPt", "Muon p_{T} [GeV]","Muons/4.0 GeV",0,false);
  drawDijetKinHist("hMuonPt", "Muon p_{T} [GeV]","Muons/4.0 GeV",1,false);
  drawDijetKinHist("hMuonPt", "Muon p_{T} [GeV]","Muons/4.0 GeV",2,false);

  drawDijetKinHist("hMuonDijetMass", "Dijet Mass [GeV]","Muons/50.0 GeV",0,true);
  drawDijetKinHist("hMuonDijetMass", "Dijet Mass [GeV]","Muons/50.0 GeV",1,true);
  drawDijetKinHist("hMuonDijetMass", "Dijet Mass [GeV]","Muons/50.0 GeV",2,false);

  drawDijetKinHist("hMuonEta", "Muon #eta","Muons/0.1",0,false);
  drawDijetKinHist("hMuonEta", "Muon #eta","Muons/0.1",1,false);
  drawDijetKinHist("hMuonEta", "Muon #eta","Muons/0.1",2,false);

  drawDijetKinHist("hMuonD0", "Muon 2-d Impact Parameter [cm]","Muons/0.005 cm",0,false);
  drawDijetKinHist("hMuonD0", "Muon 2-d Impact Parameter [cm]","Muons/0.005 cm",1,false);
  drawDijetKinHist("hMuonD0", "Muon 2-d Impact Parameter [cm]","Muons/0.005 cm",2,false);

  drawDijetKinHist("hMuonDeltaZ","Muon |#DeltaZ| [cm]", "Muons/0.04 cm",0,false);
  drawDijetKinHist("hMuonDeltaZ","Muon |#DeltaZ| [cm]", "Muons/0.04 cm",1,false);
  drawDijetKinHist("hMuonDeltaZ","Muon |#DeltaZ| [cm]", "Muons/0.04 cm",2,false);

  drawDijetKinHist("hMuonNValidHits","# of Track Hits","Muons",0,false);
  drawDijetKinHist("hMuonNValidHits","# of Track Hits","Muons",1,false);
  drawDijetKinHist("hMuonNValidHits","# of Track Hits","Muons",2,false);

  drawDijetKinHist("hMuonNValidMuonHits","# of Valid Muon Hits","Muons",0,false);
  drawDijetKinHist("hMuonNValidMuonHits","# of Valid Muon Hits","Muons",1,false);
  drawDijetKinHist("hMuonNValidMuonHits","# of Valid Muon Hits","Muons",2,false);

  drawDijetKinHist("hMuonNormChi2","Muon #chi^{2}/d.o.f.","Muons/0.5",0,false);
  drawDijetKinHist("hMuonNormChi2","Muon #chi^{2}/d.o.f.","Muons/0.5",1,false);
  drawDijetKinHist("hMuonNormChi2","Muon #chi^{2}/d.o.f.","Muons/0.5",2,false);
}


void drawDijetKinHist(TString name, TString xlabel, TString ylabel, int whichtags, bool doLogScale)
{
  TString label=name;
  label+=whichtags;
  if(doLogScale) label+="log";
  TString canvasname=TString("c")+label;
  TString dataname=TString("data")+label;
  TString hfjetmcname=TString("hfjetmc")+label;
  TString lfjetmcname=TString("lfjetmc")+label;
  TString diffname=TString("datajetdiff")+label;
  TString stackname=TString("stack")+label;

  int canvaswidth=600;
  int canvasheight=550;

  DijetKinHist* kinhist=GetDijetKinHist(name);
  TCanvas *c=new TCanvas(canvasname, canvasname, canvaswidth, canvasheight);
  double small=1e-8;
  if(doLogScale) {
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    c->Divide(1,2);
    c->cd(1);
    gPad->SetBottomMargin(small);
    gPad->SetPad(0,0.286,1,1);
  }

  TH1D *hData, *hHFJets, *hLFJets;
  if(whichtags==0) { // pretags
    hData = kinhist->GetDataPreTagHist(dataname);
    hHFJets = kinhist->GetHFJetPreTagHist(hfjetmcname);
    hLFJets = kinhist->GetLFJetPreTagHist(lfjetmcname);
  } else if(whichtags==1) { // single tags
    hData = kinhist->GetDataSingleTagHist(dataname);
    hHFJets = kinhist->GetHFJetSingleTagHist(hfjetmcname);
    hLFJets = kinhist->GetLFJetSingleTagHist(lfjetmcname);
  } else if(whichtags==2) { // double tags
    hData = kinhist->GetDataDoubleTagHist(dataname);
    hHFJets = kinhist->GetHFJetDoubleTagHist(hfjetmcname);
    hLFJets = kinhist->GetLFJetDoubleTagHist(lfjetmcname);
  } else if(whichtags==3) { // ==0 tags
    hData = kinhist->GetDataHist(dataname, DijetKinHist::TAG0);
    hHFJets = kinhist->GetJetHist(hfjetmcname, DijetKinHist::TAG0, DijetKinHist::HF);
    hLFJets = kinhist->GetJetHist(lfjetmcname, DijetKinHist::TAG0, DijetKinHist::LF);
  } else if(whichtags==4) { // ==1 tag
    hData = kinhist->GetDataHist(dataname, DijetKinHist::TAG1);
    hHFJets = kinhist->GetJetHist(hfjetmcname, DijetKinHist::TAG1, DijetKinHist::HF);
    hLFJets = kinhist->GetJetHist(lfjetmcname, DijetKinHist::TAG1, DijetKinHist::LF);
  } else {
    std::cout << whichtags << " is not a valid whichtags." << std::endl;
    return;
  }

  hData->SetStats(0);
  hHFJets->SetStats(0);
  hLFJets->SetStats(0);
  hData->SetMarkerStyle(20);
  hData->SetMarkerSize(0.9);
  hHFJets->SetFillColor(kCyan-1);
  hLFJets->SetFillColor(kCyan-2);
  hData->SetLineWidth(1);
  hHFJets->SetLineWidth(0);
  hLFJets->SetLineWidth(0);

  THStack* stack=new THStack(stackname, stackname);
  stack->Add(hLFJets);
  stack->Add(hHFJets);
  stack->Draw();
  hData->Draw("Esame");
  double maxdata=hData->GetMaximum()+sqrt(hData->GetMaximum());
  if(maxdata>stack->GetMaximum()) {
    if(doLogScale) stack->SetMaximum(maxdata*2);
    else stack->SetMaximum(maxdata*1.05);
  }
  

  stack->GetXaxis()->SetTitle(xlabel);
  stack->GetYaxis()->SetTitle(ylabel);
  if(doLogScale) {
    stack->GetXaxis()->SetTitleSize(0.08);
    stack->GetYaxis()->SetTitleSize(0.08);
    stack->GetXaxis()->SetLabelSize(0.06);
    stack->GetYaxis()->SetLabelSize(0.06);
    stack->GetYaxis()->SetTitleOffset(0.8);
  } else {
    stack->GetXaxis()->SetTitleSize(0.06);
    stack->GetYaxis()->SetTitleSize(0.06);
    stack->GetXaxis()->SetLabelSize(0.05);
    stack->GetYaxis()->SetLabelSize(0.05);
    stack->GetYaxis()->SetTitleOffset(1.0);
  }

  if(whichtags==0) stack->SetTitle("Pretags");
  if(whichtags==1) stack->SetTitle("Single-Tags");
  if(whichtags==2) stack->SetTitle("Double-Tags");
  if(whichtags==3) stack->SetTitle("=0 Tags");
  if(whichtags==4) stack->SetTitle("=1 Tags");

  if(doLogScale) gPad->SetLogy(true);


  double legendoffset=0.0;
  if(name=="hDijetDphi") legendoffset=-0.22;
  TLegend *leg=new TLegend(0.4+legendoffset,0.65,0.80+legendoffset,0.82);
  double lumi=GetLuminosity();
  char entryname[100];
  sprintf(entryname, "CMS Data (%2.1lf pb^{-1}) ", lumi);
  leg->AddEntry(hData, entryname, "p");
  leg->AddEntry(hHFJets, "Pythia 6 Dijet MC (b#bar{b})", "f");
  leg->AddEntry(hLFJets, "Pythia 6 Dijet MC (other)", "f");
  if(doLogScale) {
    TH1D* hTemp=new TH1D();
    hTemp->SetFillStyle(3004);
    hTemp->SetFillColor(kGray+3);
    hTemp->SetLineColor(0);
    leg->AddEntry(hTemp, "Data/MC-1 (#pm1 #sigma)", "f");
  }
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();

  double datasum=hData->GetSumOfWeights();
  double mcsum=hHFJets->GetSumOfWeights()+hLFJets->GetSumOfWeights();
  double datamcdiff=datasum/mcsum-1;
  double datamcdiffe=sqrt(datasum)/mcsum;
  std::cout << "Data=" << datasum
	    << "; MC=" << mcsum
	    << "; data/mc-1=" << datamcdiff
	    << " +/- " << datamcdiffe
	    << std::endl;

  if(doLogScale) {
    c->cd(2);
    gPad->SetTopMargin(small);
    gPad->SetBottomMargin(0.5);
    gPad->SetPad(0,0,1,0.286);
    gPad->SetTickx();

    int binmaxoffset=0;
    TH1D* hDiff=dynamic_cast<TH1D*>(hData->Clone(diffname));
    for(int i=0; i<=hDiff->GetNbinsX()+1; i++) {
      double x=hData->GetBinContent(i);
      double y=hHFJets->GetBinContent(i)+hLFJets->GetBinContent(i);
      double diff = (x==0 || y==0) ? 0 : x/y-1;
      double diffe = (x==0 || y==0) ? 0 : sqrt(x)/y;
      hDiff->SetBinContent(i, diff);
      hDiff->SetBinError(i, diffe);
      if(diffe<0.5 && fabs(hDiff->GetBinContent(binmaxoffset))<fabs(hDiff->GetBinContent(i)))
        binmaxoffset=i;
    }
    hDiff->Draw("E");
    hDiff->SetTitle("");
    hDiff->GetXaxis()->SetTitle(xlabel);
    hDiff->GetYaxis()->SetTitle("Data/MC-1 ");
    hDiff->GetXaxis()->SetTitleSize(0.20);
    hDiff->GetYaxis()->SetTitleSize(0.15);
    hDiff->GetXaxis()->SetLabelSize(0.15);
    hDiff->GetXaxis()->SetTitleOffset(0.9);
    hDiff->GetYaxis()->SetLabelSize(0.15);
    hDiff->GetYaxis()->SetTitleOffset(0.4);

    double minmax=fabs(hDiff->GetBinContent(binmaxoffset))*1.05;
    hDiff->SetMinimum(-minmax);
    hDiff->SetMaximum(minmax);
    TBox* box=new TBox(hDiff->GetXaxis()->GetXmin(),
		       datamcdiff-datamcdiffe,
		       hDiff->GetXaxis()->GetXmax(),
		       datamcdiff+datamcdiffe);
    box->SetFillStyle(3004);
    box->SetFillColor(kGray+3);
    box->Draw();
  }

  double resize=1.0;
  TString directory="../data/";
  c->Print(directory+canvasname+".eps");
  c->SetWindowSize(canvaswidth*resize, canvasheight*resize);
  c->Resize();
  c->Print(directory+canvasname+".gif");

  return;
}

/*void drawDijetKinHist2(TString name, TString xlabel, TString ylabel)
{
  TString label=name+TString(t)+"2";
  TString canvasname=TString("c")+label;
  TString dataname=TString("data")+label;
  TString jetname=TString("jet")+label;
  TString stackname=TString("stack")+label;

  DijetKinHist2* kinhist=GetDijetKinHist2(name);
  TCanvas *c=new TCanvas(canvasname, canvasname, 1000, 800);
  c->cd();

  TH1D *hData = kinhist->GetHist(dataname,DijetKinHist2::DATA,t);
  TH1D *hJet30 = kinhist->GetHist(jetname+"30",DijetKinHist2::JET30,t);
  TH1D *hJet50 = kinhist->GetHist(jetname+"50",DijetKinHist2::JET50,t);
  TH1D *hJet80 = kinhist->GetHist(jetname+"80",DijetKinHist2::JET80,t);
  TH1D *hJet120 = kinhist->GetHist(jetname+"120",DijetKinHist2::JET120,t);
  TH1D *hJet170 = kinhist->GetHist(jetname+"170",DijetKinHist2::JET170,t);
  TH1D *hJet300 = kinhist->GetHist(jetname+"300",DijetKinHist2::JET300,t);
  TH1D *hJet470 = kinhist->GetHist(jetname+"470",DijetKinHist2::JET470,t);
  TH1D *hJet600 = kinhist->GetHist(jetname+"600",DijetKinHist2::JET600,t);
  TH1D *hJet800 = kinhist->GetHist(jetname+"800",DijetKinHist2::JET800,t);
  TH1D *hJet1000 = kinhist->GetHist(jetname+"1000",DijetKinHist2::JET1000,t);
  TH1D *hJet1400 = kinhist->GetHist(jetname+"1400",DijetKinHist2::JET1400,t);
  TH1D *hJet1800 = kinhist->GetHist(jetname+"1800",DijetKinHist2::JET1800,t);

  THStack* stack=new THStack(stackname, stackname);
  stack->Add(hJet1800);
  stack->Add(hJet1400);
  stack->Add(hJet1000);
  stack->Add(hJet800);
  stack->Add(hJet600);
  stack->Add(hJet470);
  stack->Add(hJet300);
  stack->Add(hJet170);
  stack->Add(hJet120);
  stack->Add(hJet80);
  stack->Add(hJet50);
  stack->Add(hJet30);

  hJet30->SetFillColor(kCyan+2);
  hJet50->SetFillColor(kCyan+3);
  hJet80->SetFillColor(kCyan+4);
  hJet120->SetFillColor(kBlue-4);
  hJet170->SetFillColor(kBlue-3);
  hJet300->SetFillColor(kBlue-2);
  hJet470->SetFillColor(kBlue-1);
  hJet600->SetFillColor(kBlue);
  hJet800->SetFillColor(kBlue+1);
  hJet1000->SetFillColor(kBlue+2);
  hJet1400->SetFillColor(kBlue+3);
  hJet1800->SetFillColor(kBlue+4);
  

  hJet30->SetLineWidth(1);
  hJet50->SetLineWidth(1);
  hJet80->SetLineWidth(1);
  hJet120->SetLineWidth(1);
  hJet170->SetLineWidth(1);
  hJet300->SetLineWidth(1);
  hJet470->SetLineWidth(1);
  hJet600->SetLineWidth(1);
  hJet800->SetLineWidth(1);
  hJet1000->SetLineWidth(1);
  hJet1400->SetLineWidth(1);
  hJet1800->SetLineWidth(1);
  
  stack->Draw();
  hData->Draw("E0same");

  hData->SetStats(0);
  hData->SetMarkerStyle(4);
  gPad->SetLogy(1);

  stack->SetTitle("");
  stack->GetXaxis()->SetTitle(xlabel);
  stack->GetYaxis()->SetTitle(ylabel);

  TLegend *leg = new TLegend(0.5,0.5,0.8,0.8);
  leg->AddEntry(hData, "Data", "l");
  leg->AddEntry(hJet30, "Jet 30>#hat{p}_{T} [GeV]>50", "f");
  leg->AddEntry(hJet50, "Jet 50>#hat{p}_{T} [GeV]>80", "f");
  leg->AddEntry(hJet80, "Jet 80>#hat{p}_{T} [GeV]>120", "f");
  leg->AddEntry(hJet120, "Jet 120>#hat{p}_{T} [GeV]>170", "f");
  leg->AddEntry(hJet170, "Jet 170>#hat{p}_{T} [GeV]>300", "f");
  leg->AddEntry(hJet300, "Jet 300>#hat{p}_{T} [GeV]>470", "f");
  leg->AddEntry(hJet470, "Jet 470>#hat{p}_{T} [GeV]>600", "f");
  leg->AddEntry(hJet600, "Jet 600>#hat{p}_{T} [GeV]>800", "f");
  leg->AddEntry(hJet800, "Jet 800>#hat{p}_{T} [GeV]>1000", "f");
  leg->AddEntry(hJet1000, "Jet 1000>#hat{p}_{T} [GeV]>1400", "f");
  leg->AddEntry(hJet1400, "Jet 1400>#hat{p}_{T} [GeV]>1800", "f");
  leg->AddEntry(hJet1800, "Jet 1800>#hat{p}_{T} [GeV]", "f");
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->Draw();

  return;
}
*/
