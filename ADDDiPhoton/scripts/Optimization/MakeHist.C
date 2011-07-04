#define addgentree_cxx
#include "addgentree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF1.h>

#include <sstream>

void MakeHist(void)
{
  TChain *ch=new TChain("addgentree");
  ch->Add("/uscms/home/johnpaul/nobackup/ADD/ADDGenTree_MS*.root");
  addgentree *t=new addgentree(ch);
  t->Loop();
  return;
}

void addgentree::Loop()
{
  if (fChain == 0) return;
  
  TFile* rootfile=new TFile("opthists.root","RECREATE");
  rootfile->cd();

  TF1* fKfactor = new TF1("kfactor", "-6.08061+9.90359/x^(3.88349e-02)", 100, 2000) ;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  Int_t prevTree=-1;
  TH2D* hist=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    if(fCurrent!=prevTree) {
      prevTree=fCurrent;
      if(hist) hist->Write();
      ostringstream histname;
      histname << "hMS" << MS << "NED" << NED << "KK" << KK;
      hist=new TH2D(histname.str().c_str(), histname.str().c_str(),50,0,5,40,0,2000);
    }
    double maxabseta=std::max(fabs(eta1), fabs(eta2));
    double newweight=1.6*weight;
    //    if(MS>=99999.) newweight*=fKfactor->Eval(invmass);
    //    else newweight*=1.6;
    hist->Fill(maxabseta, invmass, newweight);
  }

  if(hist) hist->Write();
  return;
}
