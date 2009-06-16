#define ADDdiphotonTree_cxx
#include "ADDdiphotonTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
#include <cmath>

const int ANATYPE = std::atoi(std::getenv("ANATYPE"));

void makeplots(void)
{
  TChain *ch=new TChain("ADDdiphotonTree");
  if(ANATYPE==1)
    ch->Add("/localscratch/johnpaul/ADDDiPhoton/ana_DiPhotonBorn*.root");
  else if(ANATYPE==2)
    ch->Add("/localscratch/johnpaul/ADDDiPhoton/ana_ADDdiphoton_pixelMatch_MS1000_NED2_KK1_MASSCUT_200_10000_1.root");
  else if(ANATYPE==3)
    ch->Add("/localscratch/johnpaul/ADDDiPhoton/ana_ADDdiphoton_pixelMatch_MS100000_NED4_KK1_MASSCUT_200_10000_1.root");
  ADDdiphotonTree* t=new ADDdiphotonTree(ch);
  t->Loop();
  return;
}

void ADDdiphotonTree::Loop()
{
   if (fChain == 0) return;

   TFile *rootfile = 0;
   if(ANATYPE==1)
     rootfile = new TFile("results/born_plots.root","RECREATE");
   if(ANATYPE==2)
     rootfile = new TFile("results/add_plots.root","RECREATE");
   if(ANATYPE==3)
     rootfile = new TFile("results/add_noled_plots.root","RECREATE");

   const int netabins=25;
   const double maxeta=1.5;
   const double mineta=0.0;
   const int netbins=20;
   const double minet=0;
   const double maxet=1000;
   TH1D* hEta=new TH1D("hEta","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN1=new TH1D("hEtaN1","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN2=new TH1D("hEtaN2","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN3=new TH1D("hEtaN3","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN4=new TH1D("hEtaN4","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN5=new TH1D("hEtaN5","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN6=new TH1D("hEtaN6","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN7=new TH1D("hEtaN7","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaN8=new TH1D("hEtaN8","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaD=new TH1D("hEtaD","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff1=new TH1D("hEtaEff1","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff2=new TH1D("hEtaEff2","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff3=new TH1D("hEtaEff3","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff4=new TH1D("hEtaEff4","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff5=new TH1D("hEtaEff5","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff6=new TH1D("hEtaEff6","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff7=new TH1D("hEtaEff7","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEtaEff8=new TH1D("hEtaEff8","#eta of un-reco'ed photons",netabins,-maxeta,maxeta);
   TH1D* hEt=new TH1D("hEt","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN1=new TH1D("hEtN1","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN2=new TH1D("hEtN2","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN3=new TH1D("hEtN3","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN4=new TH1D("hEtN4","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN5=new TH1D("hEtN5","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN6=new TH1D("hEtN6","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN7=new TH1D("hEtN7","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtN8=new TH1D("hEtN8","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtD=new TH1D("hEtD","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff1 = new TH1D("hEtEff1","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff2 = new TH1D("hEtEff2","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff3 = new TH1D("hEtEff3","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff4 = new TH1D("hEtEff4","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff5 = new TH1D("hEtEff5","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff6 = new TH1D("hEtEff6","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff7 = new TH1D("hEtEff7","E_T of un-reco'ed photons",netbins,minet,maxet);
   TH1D* hEtEff8 = new TH1D("hEtEff8","E_T of un-reco'ed photons",netbins,minet,maxet);

   TH2D* hHEIsoPt = new TH2D("hHEIsoPt","H/E Isolation v. Pt",100,0,2,100,0,500);
   TH2D* hHcalIsoPt = new TH2D("hHcalIsoPt","Hcal Isolation v. Pt",100,0,50,100,0,500);
   TH2D* hEcalIsoPt = new TH2D("hEcalIsoPt","Ecal Isolation v. Pt",100,0,30,100,0,500);
   TH2D* hTrkIsoPt = new TH2D("hTrkIsoPt","Trk Isolation v. Pt",100,0,30,100,0,500);
   TH1D* hJetEt = new TH1D("hJetEt","Jet Et",100,0,300);
   TH1D* hNJets = new TH1D("hNJets","# Jets",21,-0.5,20.5);

   
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      int cntJets=0;
      for(int i=0; i<nJets; i++) {
	if(et_recJet[i]<30) continue;
	++cntJets;
	hJetEt->Fill(et_recJet[i]);
      }
      hNJets->Fill(cntJets);

      int leading=-1;
      for(int i=0; i<nGenPho; i++) {
	if(motherGenPdgId_pho[i]!=22) continue;
	if(fabs(genEta_pho[i])>maxeta) continue;
	if(fabs(genEta_pho[i])<mineta) continue;
	if(genPt_pho[i]<50) continue;

	if(leading<0 || genPt_pho[leading]<genPt_pho[i]) leading=i;
      }
      
      if(leading>=0) {
	// match to a reco photon
	int match=-1;
	for(int j=0; j<nPhos; j++) {
	  double deta = genEta_pho[leading] - eta_recPho[j];
	  double dphi = fabs(genPhi_pho[leading] - phi_recPho[j]);
	  if(dphi>3.1415) dphi = 2*3.1415 - dphi;
	  double dr = sqrt(deta*deta + dphi*dphi);
	  if(dr<0.4) match=j;
	}
	bool foundmatch = match>=0;

	// has a pixel seed
	bool hasSeed = hasPixelSeed[match];

	// pass iso
	bool passHEIso = hadronicOverEm[match] < 0.05;
	bool passEcalIso = isolationEcalRecHit[match] < 10;
	bool passHcalIso =isolationHcalRecHit[match] < 4.7 + 0.003*genPt_pho[leading];
	bool passTrkIso = isolationHollowTrkCone[match] < 5.0;
	bool passIso = passHEIso && passEcalIso && passHcalIso && passTrkIso;

	if(foundmatch) {
	  hHEIsoPt->Fill(hadronicOverEm[match],genPt_pho[leading]);
	  hHcalIsoPt->Fill(isolationHcalRecHit[match],genPt_pho[leading]);
	  hEcalIsoPt->Fill(isolationEcalRecHit[match],genPt_pho[leading]);
	  hTrkIsoPt->Fill(isolationHollowTrkCone[match],genPt_pho[leading]);
	}

	double filleta=genEta_pho[leading];
	double fillet=genPt_pho[leading];
	//	if(filleta>0.00 && filleta<0.02) continue;
	//	if(filleta>0.43 && filleta<0.46) continue;
	//	if(filleta>0.78 && filleta<0.81) continue;
	//	if(filleta>1.13 && filleta<1.15) continue;
	//	if(filleta>1.45 && filleta<1.58) continue;

	double phi = static_cast<double>(genPhi_pho[leading]);
	if(phi<0) phi += 6.29;
	double phirelative = std::fmod(phi, static_cast<double>(20.*3.1415/180.)) - 10*3.1415/180;

	if(!foundmatch) {
	  hEta->Fill(filleta);
	  hEt->Fill(fillet);
	}
	if(foundmatch) {
	  hEtaN1->Fill(filleta);
	  hEtN1->Fill(fillet);
	}
	if(!hasSeed) {
	  hEtaN2->Fill(filleta);
	  hEtN2->Fill(fillet);
	}
	if(passIso) {
	  hEtaN3->Fill(filleta);
	  hEtN3->Fill(fillet);
	}
	if(!hasSeed && foundmatch && passIso) {
	  hEtaN4->Fill(filleta);
	  hEtN4->Fill(fillet);
	}
	if(passHEIso) {
	  hEtaN5->Fill(filleta);
	  hEtN5->Fill(fillet);
	}
	if(passHcalIso) {
	  hEtaN6->Fill(filleta);
	  hEtN6->Fill(fillet);
	}
	if(passEcalIso) {
	  hEtaN7->Fill(filleta);
	  hEtN7->Fill(fillet);
	}
	if(passTrkIso) {
	  hEtaN8->Fill(filleta);
	  hEtN8->Fill(fillet);
	}

	hEtaD->Fill(filleta);
	hEtD->Fill(fillet);
      }
   }

   // end of loop

   rootfile->cd();

   for(int i=0; i<=hEtD->GetNbinsX()+1; i++) {
     double num1=hEtN1->GetBinContent(i);
     double num2=hEtN2->GetBinContent(i);
     double num3=hEtN3->GetBinContent(i);
     double num4=hEtN4->GetBinContent(i);
     double num5=hEtN5->GetBinContent(i);
     double num6=hEtN6->GetBinContent(i);
     double num7=hEtN7->GetBinContent(i);
     double num8=hEtN8->GetBinContent(i);
     double den=hEtD->GetBinContent(i);
     hEtEff1->SetBinContent(i, (den==0) ? 0 : num1/den);
     hEtEff1->SetBinError(i, (den==0) ? 0 : sqrt(num1/den*(1-num1/den)/den));
     hEtEff2->SetBinContent(i, (den==0) ? 0 : num2/den);
     hEtEff2->SetBinError(i, (den==0) ? 0 : sqrt(num2/den*(1-num2/den)/den));
     hEtEff3->SetBinContent(i, (den==0) ? 0 : num3/den);
     hEtEff3->SetBinError(i, (den==0) ? 0 : sqrt(num3/den*(1-num3/den)/den));
     hEtEff4->SetBinContent(i, (den==0) ? 0 : num4/den);
     hEtEff4->SetBinError(i, (den==0) ? 0 : sqrt(num4/den*(1-num4/den)/den));
     hEtEff5->SetBinContent(i, (den==0) ? 0 : num5/den);
     hEtEff5->SetBinError(i, (den==0) ? 0 : sqrt(num5/den*(1-num5/den)/den));
     hEtEff6->SetBinContent(i, (den==0) ? 0 : num6/den);
     hEtEff6->SetBinError(i, (den==0) ? 0 : sqrt(num6/den*(1-num6/den)/den));
     hEtEff7->SetBinContent(i, (den==0) ? 0 : num7/den);
     hEtEff7->SetBinError(i, (den==0) ? 0 : sqrt(num7/den*(1-num7/den)/den));
     hEtEff8->SetBinContent(i, (den==0) ? 0 : num8/den);
     hEtEff8->SetBinError(i, (den==0) ? 0 : sqrt(num8/den*(1-num8/den)/den));
   }

   for(int i=0; i<=hEtaD->GetNbinsX()+1; i++) {
     double num1=hEtaN1->GetBinContent(i);
     double num2=hEtaN2->GetBinContent(i);
     double num3=hEtaN3->GetBinContent(i);
     double num4=hEtaN4->GetBinContent(i);
     double num5=hEtaN5->GetBinContent(i);
     double num6=hEtaN6->GetBinContent(i);
     double num7=hEtaN7->GetBinContent(i);
     double num8=hEtaN8->GetBinContent(i);
     double den=hEtaD->GetBinContent(i);
     hEtaEff1->SetBinContent(i, (den==0) ? 0 : num1/den);
     hEtaEff1->SetBinError(i, (den==0) ? 0 : sqrt(num1/den*(1-num1/den)/den));
     hEtaEff2->SetBinContent(i, (den==0) ? 0 : num2/den);
     hEtaEff2->SetBinError(i, (den==0) ? 0 : sqrt(num2/den*(1-num2/den)/den));
     hEtaEff3->SetBinContent(i, (den==0) ? 0 : num3/den);
     hEtaEff3->SetBinError(i, (den==0) ? 0 : sqrt(num3/den*(1-num3/den)/den));
     hEtaEff4->SetBinContent(i, (den==0) ? 0 : num4/den);
     hEtaEff4->SetBinError(i, (den==0) ? 0 : sqrt(num4/den*(1-num4/den)/den));
     hEtaEff5->SetBinContent(i, (den==0) ? 0 : num5/den);
     hEtaEff5->SetBinError(i, (den==0) ? 0 : sqrt(num5/den*(1-num5/den)/den));
     hEtaEff6->SetBinContent(i, (den==0) ? 0 : num6/den);
     hEtaEff6->SetBinError(i, (den==0) ? 0 : sqrt(num6/den*(1-num6/den)/den));
     hEtaEff7->SetBinContent(i, (den==0) ? 0 : num7/den);
     hEtaEff7->SetBinError(i, (den==0) ? 0 : sqrt(num7/den*(1-num7/den)/den));
     hEtaEff8->SetBinContent(i, (den==0) ? 0 : num8/den);
     hEtaEff8->SetBinError(i, (den==0) ? 0 : sqrt(num8/den*(1-num8/den)/den));
   }

   hHEIsoPt->Write();
   hHcalIsoPt->Write();
   hEcalIsoPt->Write();
   hTrkIsoPt->Write();
   
   hEta->Write();
   hEt->Write();
   hEtaN1->Write();
   hEtN1->Write();
   hEtaN2->Write();
   hEtN2->Write();
   hEtaN3->Write();
   hEtN3->Write();
   hEtaN4->Write();
   hEtN4->Write();
   hEtaN5->Write();
   hEtN5->Write();
   hEtaN6->Write();
   hEtN6->Write();
   hEtaN7->Write();
   hEtN7->Write();
   hEtaN8->Write();
   hEtN8->Write();
   hEtaD->Write();
   hEtD->Write();
   hEtaEff1->Write();
   hEtEff1->Write();
   hEtaEff2->Write();
   hEtEff2->Write();
   hEtaEff3->Write();
   hEtEff3->Write();
   hEtaEff4->Write();
   hEtEff4->Write();
   hEtaEff5->Write();
   hEtEff5->Write();
   hEtaEff6->Write();
   hEtEff6->Write();
   hEtaEff7->Write();
   hEtEff7->Write();
   hEtaEff8->Write();
   hEtEff8->Write();

   hJetEt->Write();
   hNJets->Write();

   rootfile->Close();

   return;
}
