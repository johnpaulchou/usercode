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

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  Int_t prevTree=-1;
  TH1D* hXS=new TH1D("hXS","XS*ACC*BF",100,0.5,100.5);
  TH1D* hCount=new TH1D("hCount","XS*ACC*BF",100,0.5,100.5);
  TH1D* hEtaG=new TH1D("hEtaG","Eta_G",100,0.5,100.5);
  TF1* fKfactor = new TF1("kfactor", "2.91986557985310036-0.284606795636766441/x^(-2.32654814731609899e-01)", 0, 2000) ;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    if(fCurrent!=prevTree) {
      prevTree=fCurrent;
      ostringstream histname;
      histname << "hMS" << MS << "NED" << NED << "KK" << KK;
      hXS->GetXaxis()->SetBinLabel(fCurrent+1, histname.str().c_str());
      if(NED==2 && KK==1) hEtaG->SetBinContent(fCurrent+1, 1.0/pow(MS/1000.,4.0));
      else if(KK==1) hEtaG->SetBinContent(fCurrent+1, 2./(NED-2.)/pow(MS/1000.,4.0));
      else if(KK==3 || KK==4) hEtaG->SetBinContent(fCurrent+1, 2./3.1415/pow(MS/1000.,4.0));
    }

    double kFactor=1.0;
    //    if(fCurrent==0) { // use a different k-factor for the background
    //      kFactor=fKfactor->Eval(invmass);
    //    }
    if(fabs(eta1)<1.44 && fabs(eta2)<1.44 && et1>60 && et2>60 && invmass>800) {
      hXS->Fill(fCurrent+1, weight*kFactor);
      hCount->Fill(fCurrent+1, 1.0);
    }
    
  }

  for(int i=1; i<=fCurrent+1; i++) {
    //    double num=hXS->GetBinContent(i)-hXS->GetBinContent(1);
    double num=hXS->GetBinContent(i);
    double cnt=hCount->GetBinContent(i);
    std:: cout << hXS->GetXaxis()->GetBinLabel(i) << " " << num << "+-" << num/sqrt(cnt) << " " << hEtaG->GetBinContent(i) << std::endl;
  }

  return;
}
