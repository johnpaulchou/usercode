//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jul  3 13:46:59 2011 by ROOT version 5.27/06b
// from TTree addgentree/addgentree
// found on file: ADDGenTree_MS100000_NED4_KK1.root
//////////////////////////////////////////////////////////

#ifndef addgentree_h
#define addgentree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class addgentree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        MS;
   Int_t           NED;
   Int_t           KK;
   Double_t        weight;
   Double_t        et1;
   Double_t        eta1;
   Double_t        phi1;
   Double_t        et2;
   Double_t        eta2;
   Double_t        phi2;
   Double_t        invmass;

   // List of branches
   TBranch        *b_MS;   //!
   TBranch        *b_NED;   //!
   TBranch        *b_KK;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_et1;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_et2;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_invmass;   //!

   addgentree(TTree *tree=0);
   virtual ~addgentree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef addgentree_cxx
addgentree::addgentree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ADDGenTree_MS100000_NED4_KK1.root");
      if (!f) {
         f = new TFile("ADDGenTree_MS100000_NED4_KK1.root");
      }
      tree = (TTree*)gDirectory->Get("addgentree");

   }
   Init(tree);
}

addgentree::~addgentree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t addgentree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t addgentree::LoadTree(Long64_t entry)
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

void addgentree::Init(TTree *tree)
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

   fChain->SetBranchAddress("MS", &MS, &b_MS);
   fChain->SetBranchAddress("NED", &NED, &b_NED);
   fChain->SetBranchAddress("KK", &KK, &b_KK);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("et1", &et1, &b_et1);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("et2", &et2, &b_et2);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("invmass", &invmass, &b_invmass);
   Notify();
}

Bool_t addgentree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void addgentree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t addgentree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef addgentree_cxx
