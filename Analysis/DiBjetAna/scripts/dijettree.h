//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 15 15:15:06 2010 by ROOT version 5.22/00d
// from TTree dijettree/dijettree
// found on file: jetMC.root
//////////////////////////////////////////////////////////

#ifndef dijettree_h
#define dijettree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class dijettree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nrun;
   Int_t           nlumi;
   Int_t           nevent;
   Int_t           datasetID;
   Char_t          datasetName[100];
   Double_t        eventWeight;
   Double_t        lumi;
   Double_t        lumiErr;
   Int_t           nJets;
   Int_t           leadJet;
   Int_t           subLeadJet;
   Double_t        jetRawPt[13];   //[nJets]
   Double_t        jetCorrPt[13];   //[nJets]
   Double_t        jetEta[13];   //[nJets]
   Double_t        jetPhi[13];   //[nJets]
   Double_t        jetEMF[13];   //[nJets]
   Double_t        jetIDEMF[13];   //[nJets]
   Double_t        jetFHPD[13];   //[nJets]
   Int_t           jetN90Hits[13];   //[nJets]
   Int_t           jetNTrkVtx[13];   //[nJets]
   Int_t           jetPassJetID[13];   //[nJets]
   Int_t           jetFlavor[13];   //[nJets]
   Double_t        jetSecVtxTagLooseDisc[13];   //[nJets]
   Double_t        jetTrkCntTagLooseDisc[13];   //[nJets]
   Double_t        jetSecVtxTagTightDisc[13];   //[nJets]
   Double_t        jetTrkCntTagTightDisc[13];   //[nJets]
   Double_t        jetSecVtxFlightDist[13];   //[nJets]
   Double_t        jetSecVtxFlightDistSig[13];   //[nJets]
   Double_t        jetSecVtxMass[13];   //[nJets]
   Double_t        jetFirstTrkIP3d[13];   //[nJets]
   Double_t        jetFirstTrkIP3dSig[13];   //[nJets]
   Double_t        jetFirstTrkIP2d[13];   //[nJets]
   Double_t        jetFirstTrkIP2dSig[13];   //[nJets]
   Double_t        jetSecondTrkIP3d[13];   //[nJets]
   Double_t        jetSecondTrkIP3dSig[13];   //[nJets]
   Double_t        jetSecondTrkIP2d[13];   //[nJets]
   Double_t        jetSecondTrkIP2dSig[13];   //[nJets]
   Double_t        jetThirdTrkIP3d[13];   //[nJets]
   Double_t        jetThirdTrkIP3dSig[13];   //[nJets]
   Double_t        jetThirdTrkIP2d[13];   //[nJets]
   Double_t        jetThirdTrkIP2dSig[13];   //[nJets]
   Double_t        dijetMass;
   Double_t        dijetDeta;
   Double_t        dijetDphi;
   Double_t        met;
   Double_t        sumEt;
   Double_t        metSig;
   Int_t           nMuons;
   Double_t        muonPt[8];   //[nMuons]
   Double_t        muonEta[8];   //[nMuons]
   Double_t        muonPhi[8];   //[nMuons]
   Double_t        muonDeltaZ[8];   //[nMuons]
   Double_t        muonD0[8];   //[nMuons]
   Int_t           muonJetMatch[8];   //[nMuons]
   Int_t           muonNValidHits[8];   //[nMuons]
   Int_t           muonNValidMuonHits[8];   //[nMuons]
   Double_t        muonNormChi2[8];   //[nMuons]

   // List of branches
   TBranch        *b_nrun;   //!
   TBranch        *b_nlumi;   //!
   TBranch        *b_nevent;   //!
   TBranch        *b_datasetID;   //!
   TBranch        *b_datasetName;   //!
   TBranch        *b_eventWeight;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_lumiErr;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_leadJet;   //!
   TBranch        *b_subLeadJet;   //!
   TBranch        *b_jetRawPt;   //!
   TBranch        *b_jetCorrPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetEMF;   //!
   TBranch        *b_jetIDEMF;   //!
   TBranch        *b_jetFHPD;   //!
   TBranch        *b_jetN90Hits;   //!
   TBranch        *b_jetNTrkVtx;   //!
   TBranch        *b_jetPassJetID;   //!
   TBranch        *b_jetFlavor;   //!
   TBranch        *b_jetSecVtxTagLooseDisc;   //!
   TBranch        *b_jetTrkCntTagLooseDisc;   //!
   TBranch        *b_jetSecVtxTagTightDisc;   //!
   TBranch        *b_jetTrkCntTagTightDisc;   //!
   TBranch        *b_jetSecVtxFlightDist;   //!
   TBranch        *b_jetSecVtxFlightDistSig;   //!
   TBranch        *b_jetSecVtxMass;   //!
   TBranch        *b_jetFirstTrkIP3d;   //!
   TBranch        *b_jetFirstTrkIP3dSig;   //!
   TBranch        *b_jetFirstTrkIP2d;   //!
   TBranch        *b_jetFirstTrkIP2dSig;   //!
   TBranch        *b_jetSecondTrkIP3d;   //!
   TBranch        *b_jetSecondTrkIP3dSig;   //!
   TBranch        *b_jetSecondTrkIP2d;   //!
   TBranch        *b_jetSecondTrkIP2dSig;   //!
   TBranch        *b_jetThirdTrkIP3d;   //!
   TBranch        *b_jetThirdTrkIP3dSig;   //!
   TBranch        *b_jetThirdTrkIP2d;   //!
   TBranch        *b_jetThirdTrkIP2dSig;   //!
   TBranch        *b_dijetMass;   //!
   TBranch        *b_dijetDeta;   //!
   TBranch        *b_dijetDphi;   //!
   TBranch        *b_met;   //!
   TBranch        *b_sumEt;   //!
   TBranch        *b_metSig;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_muonPt;   //!
   TBranch        *b_muonEta;   //!
   TBranch        *b_muonPhi;   //!
   TBranch        *b_muonDeltaZ;   //!
   TBranch        *b_muonD0;   //!
   TBranch        *b_muonJetMatch;   //!
   TBranch        *b_muonNValidHits;   //!
   TBranch        *b_muonNValidMuonHits;   //!
   TBranch        *b_muonNormChi2;   //!

   dijettree(TTree *tree=0);
   virtual ~dijettree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef dijettree_cxx
dijettree::dijettree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("jetMC.root");
      if (!f) {
         f = new TFile("jetMC.root");
      }
      tree = (TTree*)gDirectory->Get("dijettree");

   }
   Init(tree);
}

dijettree::~dijettree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dijettree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dijettree::LoadTree(Long64_t entry)
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

void dijettree::Init(TTree *tree)
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

   fChain->SetBranchAddress("nrun", &nrun, &b_nrun);
   fChain->SetBranchAddress("nlumi", &nlumi, &b_nlumi);
   fChain->SetBranchAddress("nevent", &nevent, &b_nevent);
   fChain->SetBranchAddress("datasetID", &datasetID, &b_datasetID);
   fChain->SetBranchAddress("datasetName", datasetName, &b_datasetName);
   fChain->SetBranchAddress("eventWeight", &eventWeight, &b_eventWeight);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("lumiErr", &lumiErr, &b_lumiErr);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("leadJet", &leadJet, &b_leadJet);
   fChain->SetBranchAddress("subLeadJet", &subLeadJet, &b_subLeadJet);
   fChain->SetBranchAddress("jetRawPt", jetRawPt, &b_jetRawPt);
   fChain->SetBranchAddress("jetCorrPt", jetCorrPt, &b_jetCorrPt);
   fChain->SetBranchAddress("jetEta", jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetPhi", jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetEMF", jetEMF, &b_jetEMF);
   fChain->SetBranchAddress("jetIDEMF", jetIDEMF, &b_jetIDEMF);
   fChain->SetBranchAddress("jetFHPD", jetFHPD, &b_jetFHPD);
   fChain->SetBranchAddress("jetN90Hits", jetN90Hits, &b_jetN90Hits);
   fChain->SetBranchAddress("jetNTrkVtx", jetNTrkVtx, &b_jetNTrkVtx);
   fChain->SetBranchAddress("jetPassJetID", jetPassJetID, &b_jetPassJetID);
   fChain->SetBranchAddress("jetFlavor", jetFlavor, &b_jetFlavor);
   fChain->SetBranchAddress("jetSecVtxTagLooseDisc", jetSecVtxTagLooseDisc, &b_jetSecVtxTagLooseDisc);
   fChain->SetBranchAddress("jetTrkCntTagLooseDisc", jetTrkCntTagLooseDisc, &b_jetTrkCntTagLooseDisc);
   fChain->SetBranchAddress("jetSecVtxTagTightDisc", jetSecVtxTagTightDisc, &b_jetSecVtxTagTightDisc);
   fChain->SetBranchAddress("jetTrkCntTagTightDisc", jetTrkCntTagTightDisc, &b_jetTrkCntTagTightDisc);
   fChain->SetBranchAddress("jetSecVtxFlightDist", jetSecVtxFlightDist, &b_jetSecVtxFlightDist);
   fChain->SetBranchAddress("jetSecVtxFlightDistSig", jetSecVtxFlightDistSig, &b_jetSecVtxFlightDistSig);
   fChain->SetBranchAddress("jetSecVtxMass", jetSecVtxMass, &b_jetSecVtxMass);
   fChain->SetBranchAddress("jetFirstTrkIP3d", jetFirstTrkIP3d, &b_jetFirstTrkIP3d);
   fChain->SetBranchAddress("jetFirstTrkIP3dSig", jetFirstTrkIP3dSig, &b_jetFirstTrkIP3dSig);
   fChain->SetBranchAddress("jetFirstTrkIP2d", jetFirstTrkIP2d, &b_jetFirstTrkIP2d);
   fChain->SetBranchAddress("jetFirstTrkIP2dSig", jetFirstTrkIP2dSig, &b_jetFirstTrkIP2dSig);
   fChain->SetBranchAddress("jetSecondTrkIP3d", jetSecondTrkIP3d, &b_jetSecondTrkIP3d);
   fChain->SetBranchAddress("jetSecondTrkIP3dSig", jetSecondTrkIP3dSig, &b_jetSecondTrkIP3dSig);
   fChain->SetBranchAddress("jetSecondTrkIP2d", jetSecondTrkIP2d, &b_jetSecondTrkIP2d);
   fChain->SetBranchAddress("jetSecondTrkIP2dSig", jetSecondTrkIP2dSig, &b_jetSecondTrkIP2dSig);
   fChain->SetBranchAddress("jetThirdTrkIP3d", jetThirdTrkIP3d, &b_jetThirdTrkIP3d);
   fChain->SetBranchAddress("jetThirdTrkIP3dSig", jetThirdTrkIP3dSig, &b_jetThirdTrkIP3dSig);
   fChain->SetBranchAddress("jetThirdTrkIP2d", jetThirdTrkIP2d, &b_jetThirdTrkIP2d);
   fChain->SetBranchAddress("jetThirdTrkIP2dSig", jetThirdTrkIP2dSig, &b_jetThirdTrkIP2dSig);
   fChain->SetBranchAddress("dijetMass", &dijetMass, &b_dijetMass);
   fChain->SetBranchAddress("dijetDeta", &dijetDeta, &b_dijetDeta);
   fChain->SetBranchAddress("dijetDphi", &dijetDphi, &b_dijetDphi);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("sumEt", &sumEt, &b_sumEt);
   fChain->SetBranchAddress("metSig", &metSig, &b_metSig);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("muonPt", muonPt, &b_muonPt);
   fChain->SetBranchAddress("muonEta", muonEta, &b_muonEta);
   fChain->SetBranchAddress("muonPhi", muonPhi, &b_muonPhi);
   fChain->SetBranchAddress("muonDeltaZ", muonDeltaZ, &b_muonDeltaZ);
   fChain->SetBranchAddress("muonD0", muonD0, &b_muonD0);
   fChain->SetBranchAddress("muonJetMatch", muonJetMatch, &b_muonJetMatch);
   fChain->SetBranchAddress("muonNValidHits", muonNValidHits, &b_muonNValidHits);
   fChain->SetBranchAddress("muonNValidMuonHits", muonNValidMuonHits, &b_muonNValidMuonHits);
   fChain->SetBranchAddress("muonNormChi2", muonNormChi2, &b_muonNormChi2);
   Notify();
}

Bool_t dijettree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dijettree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dijettree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dijettree_cxx
