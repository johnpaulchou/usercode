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
  TH1D* hNum=new TH1D("hNum","XS*ACC*BF",100,0.5,100.5);
  TH1D* hDen=new TH1D("hDen","XS*ACC*BF",100,0.5,100.5);
  TH1D* hEtaG=new TH1D("hEtaG","Eta_G",100,0.5,100.5);

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    
    if(fCurrent!=prevTree) {
      prevTree=fCurrent;
      ostringstream histname;
      histname << "hMS" << MS << "NED" << NED << "KK" << KK;
      hNum->GetXaxis()->SetBinLabel(fCurrent+1, histname.str().c_str());
      if(NED==2 && KK==1) hEtaG->SetBinContent(fCurrent+1, 1.0/pow(MS/1000.,4.0));
      else if(KK==1) hEtaG->SetBinContent(fCurrent+1, 2./(NED-2.)/pow(MS/1000.,4.0));
      else if(KK==3 || KK==4) hEtaG->SetBinContent(fCurrent+1, 2./3.1415/pow(MS/1000.,4.0));
    }

    if(fabs(eta1)<1.442 && fabs(eta2)<1.442 && et1>30 && et2>30 && invmass>500)
      hNum->Fill(fCurrent+1, weight);
    else
      hDen->Fill(fCurrent+1, weight);
    
  }

  for(int i=1; i<=fCurrent+1; i++) {
    double num=hNum->GetBinContent(i)-hNum->GetBinContent(1);
    double den=hDen->GetBinContent(i)-hDen->GetBinContent(1);
    double acc=(num+den)==0 ? 0.0 : num/(num+den);
    cout << hDen->GetBinContent(i) << " " << hDen->GetBinContent(1) << endl;
    std:: cout << hNum->GetXaxis()->GetBinLabel(i) << " " << num << " " << den << " " << acc << " " << hEtaG->GetBinContent(i) << std::endl;
    //    std::cout << hNum->GetXaxis()->GetBinLabel(i) << " " << num << " " << den << " " << acc << " " << hEtaG->GetBinContent(i) << std::endl;
  }

  return;
}
