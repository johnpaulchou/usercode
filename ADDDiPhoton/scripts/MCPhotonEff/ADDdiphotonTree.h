//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jun 12 15:06:17 2009 by ROOT version 5.22/00a
// from TTree ADDdiphotonTree/6592
// found on file: ana_DiPhotonBornPt1100to1350_oldPhotonID_withGen_1.root
//////////////////////////////////////////////////////////

#ifndef ADDdiphotonTree_h
#define ADDdiphotonTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class ADDdiphotonTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           evt_run;
   Int_t           evt_event;
   Int_t           nJets;
   Float_t         eta_recJet[33];   //[nJets]
   Float_t         phi_recJet[33];   //[nJets]
   Float_t         e_recJet[33];   //[nJets]
   Float_t         et_recJet[33];   //[nJets]
   Float_t         p_recJet[33];   //[nJets]
   Float_t         emf_recJet[33];   //[nJets]
   Float_t         genE_recJet[33];   //[nJets]
   Float_t         genP_recJet[33];   //[nJets]
   Float_t         genEt_recJet[33];   //[nJets]
   Float_t         genEta_recJet[33];   //[nJets]
   Float_t         genPhi_recJet[33];   //[nJets]
   Int_t           nPhos;
   Float_t         eta_recPho[7];   //[nPhos]
   Float_t         phi_recPho[7];   //[nPhos]
   Float_t         e_recPho[7];   //[nPhos]
   Float_t         pt_recPho[7];   //[nPhos]
   Float_t         p_recPho[7];   //[nPhos]
   Float_t         genE_recPho[7];   //[nPhos]
   Float_t         genPt_recPho[7];   //[nPhos]
   Float_t         genEta_recPho[7];   //[nPhos]
   Float_t         genPhi_recPho[7];   //[nPhos]
   Int_t           genPdgId_recPho[7];   //[nPhos]
   Int_t           motherGenPdgId_recPho[7];   //[nPhos]
   Int_t           nGenPho;
   Float_t         genE_pho[276];   //[nGenPho]
   Float_t         genPt_pho[276];   //[nGenPho]
   Float_t         genEta_pho[276];   //[nGenPho]
   Float_t         genPhi_pho[276];   //[nGenPho]
   Int_t           genPdgId_pho[276];   //[nGenPho]
   Int_t           motherGenPdgId_pho[276];   //[nGenPho]
   Int_t           motherGenStatus_pho[276];   //[nGenPho]
   Int_t           isAlsoElectron[7];   //[nPhos]
   Int_t           doPassDefaultPhotonIDcut[7];   //[nPhos]
   Float_t         isolationEcalRecHit[7];   //[nPhos]
   Float_t         isolationHcalRecHit[7];   //[nPhos]
   Float_t         isolationSolidTrkCone[7];   //[nPhos]
   Float_t         isolationHollowTrkCone[7];   //[nPhos]
   Int_t           nTrkSolidCone[7];   //[nPhos]
   Int_t           nTrkHollowCone[7];   //[nPhos]
   Float_t         r9[7];   //[nPhos]
   Float_t         hadronicOverEm[7];   //[nPhos]
   Int_t           hasPixelSeed[7];   //[nPhos]
   Int_t           nPhotonPair;
   Int_t           indPho1_phoPair[6];   //[nPhotonPair]
   Int_t           indPho2_phoPair[6];   //[nPhotonPair]
   Float_t         invMass_phoPair[6];   //[nPhotonPair]
   Float_t         genInvMass_phoPair[6];   //[nPhotonPair]
   Float_t         dR_phoPair[6];   //[nPhotonPair]
   Float_t         dPhi_phoPair[6];   //[nPhotonPair]
   Int_t           nPhoJetPair;
   Float_t         minDrJet_phoJetPair[9];   //[nPhoJetPair]
   Int_t           indJet_phoJetPair[9];   //[nPhoJetPair]
   Float_t         minDrPho_phoJetPair[9];   //[nPhoJetPair]
   Int_t           indPho_phoJetPair[9];   //[nPhoJetPair]
   Float_t         dR_phoJetPair[9];   //[nPhoJetPair]
   Float_t         dPhi_phoJetPair[9];   //[nPhoJetPair]
   Float_t         invMass_phoJetPair[9];   //[nPhoJetPair]
   Int_t           nMCpart;
   Float_t         eta_genPart[1];   //[nMCpart]
   Float_t         phi_genPart[1];   //[nMCpart]
   Float_t         pt_genPart[1];   //[nMCpart]
   Float_t         e_genPart[1];   //[nMCpart]
   Int_t           pdgId_genPart[1];   //[nMCpart]
   Int_t           status_genPart[1];   //[nMCpart]
   Int_t           motherPdgId_genPart[1];   //[nMCpart]
   Int_t           motherStatus_genPart[1];   //[nMCpart]
   Int_t           grandMotherPdgId_genPart[1];   //[nMCpart]
   Int_t           grandMotherStatus_genPart[1];   //[nMCpart]
   Int_t           nGenJet;
   Float_t         eta_genJet[1];   //[nGenJet]
   Float_t         phi_genJet[1];   //[nGenJet]
   Float_t         et_genJet[1];   //[nGenJet]
   Float_t         e_genJet[1];   //[nGenJet]
   Float_t         pThat;
   Int_t           processId;
   Int_t           HLT_IsoPhoton40_L1R;
   Int_t           HLT_Photon25_L1R;
   Int_t           HLT_DoubleIsoPhoton20_L1R;

   // List of branches
   TBranch        *b_evt_run;   //!
   TBranch        *b_evt_event;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_eta_recJet;   //!
   TBranch        *b_phi_recJet;   //!
   TBranch        *b_e_recJet;   //!
   TBranch        *b_et_recJet;   //!
   TBranch        *b_p_recJet;   //!
   TBranch        *b_emf_recJet;   //!
   TBranch        *b_genE_recJet;   //!
   TBranch        *b_genP_recJet;   //!
   TBranch        *b_genEt_recJet;   //!
   TBranch        *b_genEta_recJet;   //!
   TBranch        *b_genPhi_recJet;   //!
   TBranch        *b_nPhos;   //!
   TBranch        *b_eta_recPho;   //!
   TBranch        *b_phi_recPho;   //!
   TBranch        *b_e_recPho;   //!
   TBranch        *b_pt_recPho;   //!
   TBranch        *b_p_recPho;   //!
   TBranch        *b_genE_recPho;   //!
   TBranch        *b_genPt_recPho;   //!
   TBranch        *b_genEta_recPho;   //!
   TBranch        *b_genPhi_recPho;   //!
   TBranch        *b_genPdgId_recPho;   //!
   TBranch        *b_motherGenPdgId_recPho;   //!
   TBranch        *b_nGenPho;   //!
   TBranch        *b_genE_pho;   //!
   TBranch        *b_genPt_pho;   //!
   TBranch        *b_genEta_pho;   //!
   TBranch        *b_genPhi_pho;   //!
   TBranch        *b_genPdgId_pho;   //!
   TBranch        *b_motherGenPdgId_pho;   //!
   TBranch        *b_motherGenStatus_pho;   //!
   TBranch        *b_isAlsoElectron;   //!
   TBranch        *b_doPassDefaultPhotonIDcut;   //!
   TBranch        *b_isolationEcalRecHit;   //!
   TBranch        *b_isolationHcalRecHit;   //!
   TBranch        *b_isolationSolidTrkCone;   //!
   TBranch        *b_isolationHollowTrkCone;   //!
   TBranch        *b_nTrkSolidCone;   //!
   TBranch        *b_nTrkHollowCone;   //!
   TBranch        *b_r9;   //!
   TBranch        *b_hadronicOverEm;   //!
   TBranch        *b_hasPixelSeed;   //!
   TBranch        *b_nPhotonPair;   //!
   TBranch        *b_indPho1_phoPair;   //!
   TBranch        *b_indPho2_phoPair;   //!
   TBranch        *b_invMass_phoPair;   //!
   TBranch        *b_genInvMass_phoPair;   //!
   TBranch        *b_dR_phoPair;   //!
   TBranch        *b_dPhi_phoPair;   //!
   TBranch        *b_nPhoJetPair;   //!
   TBranch        *b_minDrJet_phoJetPair;   //!
   TBranch        *b_indJet_phoJetPair;   //!
   TBranch        *b_minDrPho_phoJetPair;   //!
   TBranch        *b_indPho_phoJetPair;   //!
   TBranch        *b_dR_phoJetPair;   //!
   TBranch        *b_dPhi_phoJetPair;   //!
   TBranch        *b_invMass_phoJetPair;   //!
   TBranch        *b_nMCpart;   //!
   TBranch        *b_eta_genPart;   //!
   TBranch        *b_phi_genPart;   //!
   TBranch        *b_pt_genPart;   //!
   TBranch        *b_e_genPart;   //!
   TBranch        *b_pdgId_genPart;   //!
   TBranch        *b_status_genPart;   //!
   TBranch        *b_motherPdgId_genPart;   //!
   TBranch        *b_motherStatus_genPart;   //!
   TBranch        *b_grandMotherPdgId_genPart;   //!
   TBranch        *b_grandMotherStatus_genPart;   //!
   TBranch        *b_nGenJet;   //!
   TBranch        *b_eta_genJet;   //!
   TBranch        *b_phi_genJet;   //!
   TBranch        *b_et_genJet;   //!
   TBranch        *b_e_genJet;   //!
   TBranch        *b_pThat;   //!
   TBranch        *b_processId;   //!
   TBranch        *b_HLT_IsoPhoton40_L1R;   //!
   TBranch        *b_HLT_Photon25_L1;   //!
   TBranch        *b_HLT_DoubleIsoPhoton20_L1R;   //!

   ADDdiphotonTree(TTree *tree=0);
   virtual ~ADDdiphotonTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ADDdiphotonTree_cxx
ADDdiphotonTree::ADDdiphotonTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ana_DiPhotonBornPt1100to1350_oldPhotonID_withGen_1.root");
      if (!f) {
         f = new TFile("ana_DiPhotonBornPt1100to1350_oldPhotonID_withGen_1.root");
      }
      tree = (TTree*)gDirectory->Get("ADDdiphotonTree");

   }
   Init(tree);
}

ADDdiphotonTree::~ADDdiphotonTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ADDdiphotonTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ADDdiphotonTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ADDdiphotonTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt_run", &evt_run, &b_evt_run);
   fChain->SetBranchAddress("evt_event", &evt_event, &b_evt_event);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("eta_recJet", eta_recJet, &b_eta_recJet);
   fChain->SetBranchAddress("phi_recJet", phi_recJet, &b_phi_recJet);
   fChain->SetBranchAddress("e_recJet", e_recJet, &b_e_recJet);
   fChain->SetBranchAddress("et_recJet", et_recJet, &b_et_recJet);
   fChain->SetBranchAddress("p_recJet", p_recJet, &b_p_recJet);
   fChain->SetBranchAddress("emf_recJet", emf_recJet, &b_emf_recJet);
   fChain->SetBranchAddress("genE_recJet", genE_recJet, &b_genE_recJet);
   fChain->SetBranchAddress("genP_recJet", genP_recJet, &b_genP_recJet);
   fChain->SetBranchAddress("genEt_recJet", genEt_recJet, &b_genEt_recJet);
   fChain->SetBranchAddress("genEta_recJet", genEta_recJet, &b_genEta_recJet);
   fChain->SetBranchAddress("genPhi_recJet", genPhi_recJet, &b_genPhi_recJet);
   fChain->SetBranchAddress("nPhos", &nPhos, &b_nPhos);
   fChain->SetBranchAddress("eta_recPho", eta_recPho, &b_eta_recPho);
   fChain->SetBranchAddress("phi_recPho", phi_recPho, &b_phi_recPho);
   fChain->SetBranchAddress("e_recPho", e_recPho, &b_e_recPho);
   fChain->SetBranchAddress("pt_recPho", pt_recPho, &b_pt_recPho);
   fChain->SetBranchAddress("p_recPho", p_recPho, &b_p_recPho);
   fChain->SetBranchAddress("genE_recPho", genE_recPho, &b_genE_recPho);
   fChain->SetBranchAddress("genPt_recPho", genPt_recPho, &b_genPt_recPho);
   fChain->SetBranchAddress("genEta_recPho", genEta_recPho, &b_genEta_recPho);
   fChain->SetBranchAddress("genPhi_recPho", genPhi_recPho, &b_genPhi_recPho);
   fChain->SetBranchAddress("genPdgId_recPho", genPdgId_recPho, &b_genPdgId_recPho);
   fChain->SetBranchAddress("motherGenPdgId_recPho", motherGenPdgId_recPho, &b_motherGenPdgId_recPho);
   fChain->SetBranchAddress("nGenPho", &nGenPho, &b_nGenPho);
   fChain->SetBranchAddress("genE_pho", genE_pho, &b_genE_pho);
   fChain->SetBranchAddress("genPt_pho", genPt_pho, &b_genPt_pho);
   fChain->SetBranchAddress("genEta_pho", genEta_pho, &b_genEta_pho);
   fChain->SetBranchAddress("genPhi_pho", genPhi_pho, &b_genPhi_pho);
   fChain->SetBranchAddress("genPdgId_pho", genPdgId_pho, &b_genPdgId_pho);
   fChain->SetBranchAddress("motherGenPdgId_pho", motherGenPdgId_pho, &b_motherGenPdgId_pho);
   fChain->SetBranchAddress("motherGenStatus_pho", motherGenStatus_pho, &b_motherGenStatus_pho);
   fChain->SetBranchAddress("isAlsoElectron", isAlsoElectron, &b_isAlsoElectron);
   fChain->SetBranchAddress("doPassDefaultPhotonIDcut", doPassDefaultPhotonIDcut, &b_doPassDefaultPhotonIDcut);
   fChain->SetBranchAddress("isolationEcalRecHit", isolationEcalRecHit, &b_isolationEcalRecHit);
   fChain->SetBranchAddress("isolationHcalRecHit", isolationHcalRecHit, &b_isolationHcalRecHit);
   fChain->SetBranchAddress("isolationSolidTrkCone", isolationSolidTrkCone, &b_isolationSolidTrkCone);
   fChain->SetBranchAddress("isolationHollowTrkCone", isolationHollowTrkCone, &b_isolationHollowTrkCone);
   fChain->SetBranchAddress("nTrkSolidCone", nTrkSolidCone, &b_nTrkSolidCone);
   fChain->SetBranchAddress("nTrkHollowCone", nTrkHollowCone, &b_nTrkHollowCone);
   fChain->SetBranchAddress("r9", r9, &b_r9);
   fChain->SetBranchAddress("hadronicOverEm", hadronicOverEm, &b_hadronicOverEm);
   fChain->SetBranchAddress("hasPixelSeed", hasPixelSeed, &b_hasPixelSeed);
   fChain->SetBranchAddress("nPhotonPair", &nPhotonPair, &b_nPhotonPair);
   fChain->SetBranchAddress("indPho1_phoPair", indPho1_phoPair, &b_indPho1_phoPair);
   fChain->SetBranchAddress("indPho2_phoPair", indPho2_phoPair, &b_indPho2_phoPair);
   fChain->SetBranchAddress("invMass_phoPair", invMass_phoPair, &b_invMass_phoPair);
   fChain->SetBranchAddress("genInvMass_phoPair", genInvMass_phoPair, &b_genInvMass_phoPair);
   fChain->SetBranchAddress("dR_phoPair", dR_phoPair, &b_dR_phoPair);
   fChain->SetBranchAddress("dPhi_phoPair", dPhi_phoPair, &b_dPhi_phoPair);
   fChain->SetBranchAddress("nPhoJetPair", &nPhoJetPair, &b_nPhoJetPair);
   fChain->SetBranchAddress("minDrJet_phoJetPair", minDrJet_phoJetPair, &b_minDrJet_phoJetPair);
   fChain->SetBranchAddress("indJet_phoJetPair", indJet_phoJetPair, &b_indJet_phoJetPair);
   fChain->SetBranchAddress("minDrPho_phoJetPair", minDrPho_phoJetPair, &b_minDrPho_phoJetPair);
   fChain->SetBranchAddress("indPho_phoJetPair", indPho_phoJetPair, &b_indPho_phoJetPair);
   fChain->SetBranchAddress("dR_phoJetPair", dR_phoJetPair, &b_dR_phoJetPair);
   fChain->SetBranchAddress("dPhi_phoJetPair", dPhi_phoJetPair, &b_dPhi_phoJetPair);
   fChain->SetBranchAddress("invMass_phoJetPair", invMass_phoJetPair, &b_invMass_phoJetPair);
   fChain->SetBranchAddress("nMCpart", &nMCpart, &b_nMCpart);
   fChain->SetBranchAddress("eta_genPart", &eta_genPart, &b_eta_genPart);
   fChain->SetBranchAddress("phi_genPart", &phi_genPart, &b_phi_genPart);
   fChain->SetBranchAddress("pt_genPart", &pt_genPart, &b_pt_genPart);
   fChain->SetBranchAddress("e_genPart", &e_genPart, &b_e_genPart);
   fChain->SetBranchAddress("pdgId_genPart", &pdgId_genPart, &b_pdgId_genPart);
   fChain->SetBranchAddress("status_genPart", &status_genPart, &b_status_genPart);
   fChain->SetBranchAddress("motherPdgId_genPart", &motherPdgId_genPart, &b_motherPdgId_genPart);
   fChain->SetBranchAddress("motherStatus_genPart", &motherStatus_genPart, &b_motherStatus_genPart);
   fChain->SetBranchAddress("grandMotherPdgId_genPart", &grandMotherPdgId_genPart, &b_grandMotherPdgId_genPart);
   fChain->SetBranchAddress("grandMotherStatus_genPart", &grandMotherStatus_genPart, &b_grandMotherStatus_genPart);
   fChain->SetBranchAddress("nGenJet", &nGenJet, &b_nGenJet);
   fChain->SetBranchAddress("eta_genJet", &eta_genJet, &b_eta_genJet);
   fChain->SetBranchAddress("phi_genJet", &phi_genJet, &b_phi_genJet);
   fChain->SetBranchAddress("et_genJet", &et_genJet, &b_et_genJet);
   fChain->SetBranchAddress("e_genJet", &e_genJet, &b_e_genJet);
   fChain->SetBranchAddress("pThat", &pThat, &b_pThat);
   fChain->SetBranchAddress("processId", &processId, &b_processId);
   fChain->SetBranchAddress("HLT_IsoPhoton40_L1R", &HLT_IsoPhoton40_L1R, &b_HLT_IsoPhoton40_L1R);
   fChain->SetBranchAddress("HLT_Photon25_L1R", &HLT_Photon25_L1R, &b_HLT_Photon25_L1);
   fChain->SetBranchAddress("HLT_DoubleIsoPhoton20_L1R", &HLT_DoubleIsoPhoton20_L1R, &b_HLT_DoubleIsoPhoton20_L1R);
   Notify();
}

Bool_t ADDdiphotonTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ADDdiphotonTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ADDdiphotonTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ADDdiphotonTree_cxx
