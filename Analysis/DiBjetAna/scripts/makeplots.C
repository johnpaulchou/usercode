#define dijettree_cxx
#include "dijettree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "Analysis/DataFormats/interface/DijetKinHist.hh"

#include <iostream>
#include <string>

const char* datafn="../data/data.root";
const char* jetmcfn="../data/jetMC.root";

void makeplots(void)
{
  TChain *ch=new TChain("dijettree");
  ch->Add(datafn);
  ch->Add(jetmcfn);
  dijettree *t=new dijettree(ch);
  t->Loop();
  return;
}

void dijettree::Loop()
{
  // setup histograms
  TFile* rootfile=new TFile("../data/plots.root","RECREATE");

  TH1D* hLumi = new TH1D("hLumi","Luminosity",1,-0.5,0.5);
  DijetKinHist* hDijetMass = new DijetKinHist("hDijetMass","Dijet Mass",88,500,2700);
  DijetKinHist* hLeadJetPt = new DijetKinHist("hLeadJetPt","Lead Jet PT",60,160,1360);
  DijetKinHist* hSubLeadJetPt = new DijetKinHist("hSubLeadJetPt","Sub-lead Jet PT",60,160,1360);
  DijetKinHist* hLeadJetEta = new DijetKinHist("hLeadJetEta","Lead Jet Eta",50,-2.5,2.5);
  DijetKinHist* hSubLeadJetEta = new DijetKinHist("hSubLeadJetEta","Sub-lead Jet Eta",50,-2.5,2.5);
  DijetKinHist* hLeadJetPhi = new DijetKinHist("hLeadJetPhi","Lead Jet Phi",50,-3.1416,3.1416);
  DijetKinHist* hSubLeadJetPhi = new DijetKinHist("hSubLeadJetPhi","Sub-lead Jet Phi",50,-3.1416,3.1416);
  DijetKinHist* hMET = new DijetKinHist("hMET","MET",50,0,500);
  DijetKinHist* hMETSig = new DijetKinHist("hMETSig","MET Significance",40,0,20);
  DijetKinHist* hDijetDphi = new DijetKinHist("hDijetDphi","Delta Jet phi",50,0.0,3.1416);
  DijetKinHist* hJetEMF = new DijetKinHist("hJetEMF","Jet EMF",50,0,1.0);
  DijetKinHist* hMuonDijetMass = new DijetKinHist("hMuonDijetMass", "Muons per dijet Mass",44,500,2700);
  DijetKinHist* hMuonDeltaZ = new DijetKinHist("hMuonDeltaZ","Muon Delta-Z",50,-1,1);
  DijetKinHist* hMuonD0 = new DijetKinHist("hMuonD0","Muon D0",40,0,0.2);
  DijetKinHist* hMuonPt = new DijetKinHist("hMuonPt","Muon Pt",50,0,200);
  DijetKinHist* hMuonEta = new DijetKinHist("hMuonEta","Muon Eta",50,-2.5,2.5);
  DijetKinHist* hMuonNValidHits=new DijetKinHist("hMuonNValidHits","Muon # Valid Hits",20,9.5,29.5);
  DijetKinHist* hMuonNValidMuonHits=new DijetKinHist("hMuonNValidMuonHits","Muon # Valid Hits",52,-0.5,51.5);
  DijetKinHist* hMuonNormChi2=new DijetKinHist("hMuonNormChi2","Muon chi^2",20,0,10);

  double maxInvmass=0.0;
  double pvalue=0.0;

  // begin loop
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntries();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    if(jentry%100000==0) {
      std::cout << "Looping over entry " << jentry << " out of " << nentries << std::endl;
    }

    if(dijetMass<500.) continue;
    if(fabs(dijetDeta)>1.3) continue;
    if(jetPassJetID[leadJet]!=1) continue;
    if(jetPassJetID[subLeadJet]!=1) continue;

    DijetKinHist::dataset d;
    if(strcmp(datasetName, "JetMETTauNov4thReRecoDijetSelection385p3")==0) d=DijetKinHist::DATA;
    else if(strcmp(datasetName, "JetMETNov4thReRecoDijetSelection385p3")==0) d=DijetKinHist::DATA;
    else if(strcmp(datasetName, "JetNov4thReRecoDijetSelection385p3")==0) d=DijetKinHist::DATA;
    else if(strcmp(datasetName, "QCDPt80to120Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET120;
    else if(strcmp(datasetName, "QCDPt120to170Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET120;
    else if(strcmp(datasetName, "QCDPt170to300Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET170;
    else if(strcmp(datasetName, "QCDPt300to470Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET300;
    else if(strcmp(datasetName, "QCDPt470to600Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET470;
    else if(strcmp(datasetName, "QCDPt600to800Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET600;
    else if(strcmp(datasetName, "QCDPt800to1000Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET800;
    else if(strcmp(datasetName, "QCDPt1000to1400Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET1000;
    else if(strcmp(datasetName, "QCDPt1400to1800Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET1400;
    else if(strcmp(datasetName, "QCDPt1800Pythia6Fall10DijetSelection385p3")==0) d=DijetKinHist::JET1800;
    else { std::cout << "What do we do with the dataset `" << datasetName << "'?" << std::endl; }
    
    //    bool leadTag=jetSecVtxTagLooseDisc[leadJet]>1.74;
    //    bool subLeadTag=jetSecVtxTagLooseDisc[subLeadJet]>1.74;
    bool leadTag=jetSecVtxTagLooseDisc[leadJet]>1.74;
    bool subLeadTag=jetSecVtxTagLooseDisc[subLeadJet]>1.74;
    int ntags = leadTag+subLeadTag;
    DijetKinHist::taglevel t = static_cast<DijetKinHist::taglevel>(ntags);

    DijetKinHist::hfcontent h;
    if((abs(jetFlavor[leadJet])==4 || abs(jetFlavor[leadJet])==5) && (abs(jetFlavor[subLeadJet])==4 || abs(jetFlavor[subLeadJet])==5)) h=DijetKinHist::HF;
    else h=DijetKinHist::LF;

    double dijetDphi = fabs(jetPhi[leadJet]-jetPhi[subLeadJet]);
    if(dijetDphi>3.1416) dijetDphi = 6.29-dijetDphi;

    hDijetMass->Fill(dijetMass, d, t, h, eventWeight);
    hDijetDphi->Fill(dijetDphi, d, t, h, eventWeight);
    hLeadJetPt->Fill(jetCorrPt[leadJet], d, t, h, eventWeight);
    hSubLeadJetPt->Fill(jetCorrPt[subLeadJet], d, t, h, eventWeight);
    hLeadJetEta->Fill(jetEta[leadJet], d, t, h, eventWeight);
    hSubLeadJetEta->Fill(jetEta[subLeadJet], d, t, h, eventWeight);
    hLeadJetPhi->Fill(jetPhi[leadJet], d, t, h, eventWeight);
    hSubLeadJetPhi->Fill(jetPhi[subLeadJet], d, t, h, eventWeight);
    hMET->Fill(met, d, t, h, eventWeight);
    hMETSig->Fill(metSig, d, t, h, eventWeight);
    
    hJetEMF->Fill(jetEMF[leadJet], d, t, h, eventWeight);
    hJetEMF->Fill(jetEMF[subLeadJet], d, t, h, eventWeight);

    // fill muon plots
    for(int i=0; i<nMuons; i++) {
      if(muonJetMatch[i]!=leadJet && muonJetMatch[i]!=subLeadJet) continue;
      if(muonNValidMuonHits[i]<1) continue;
      if(muonNormChi2[i]>=10.0) continue;
      if(fabs(muonDeltaZ[i])>1.0) continue;
      if(muonD0[i]>0.2) continue;
      if(muonNValidHits[i]<10) continue;
      hMuonPt->Fill(muonPt[i], d, t, h, eventWeight);
      hMuonEta->Fill(muonEta[i], d, t, h, eventWeight);
      hMuonDeltaZ->Fill(muonDeltaZ[i], d, t, h, eventWeight);
      hMuonD0->Fill(muonD0[i], d, t, h, eventWeight);
      hMuonNValidHits->Fill(muonNValidHits[i], d, t, h, eventWeight);
      hMuonNValidMuonHits->Fill(muonNValidMuonHits[i], d, t, h, eventWeight);
      hMuonNormChi2->Fill(muonNormChi2[i], d, t, h, eventWeight);
      hMuonDijetMass->Fill(dijetMass, d, t, h, eventWeight);
    }

    if(d==DijetKinHist::DATA && hLumi->GetEntries()==0) {
      hLumi->SetBinContent(1, lumi);
      hLumi->SetBinError(1, lumiErr);
    }

    // find maximum invariant mass (assumes data runs before MC)
    if(d==DijetKinHist::DATA && maxInvmass<dijetMass) maxInvmass=dijetMass;
    if(d!=DijetKinHist::DATA && dijetMass>=maxInvmass) pvalue += eventWeight;
  }
  
  rootfile->cd();
  hDijetMass->Write();
  hDijetDphi->Write();
  hLeadJetPt->Write();
  hSubLeadJetPt->Write();
  hLeadJetEta->Write();
  hSubLeadJetEta->Write();
  hLeadJetPhi->Write();
  hSubLeadJetPhi->Write();
  hMET->Write();
  hMETSig->Write();
  hJetEMF->Write();
  hMuonDijetMass->Write();
  hMuonPt->Write();
  hMuonEta->Write();
  hMuonDeltaZ->Write();
  hMuonD0->Write();
  hMuonNValidHits->Write();
  hMuonNValidMuonHits->Write();
  hMuonNormChi2->Write();

  hLumi->Write();
  rootfile->Close();

  std::cout << "maximum mass=" << maxInvmass << " GeV." << std::endl;
  std::cout << "p-value=" << pvalue << std::endl;

  return;
}
