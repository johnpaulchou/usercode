////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MakeDijetTree:
//     Make a tree for the analysis of ADD diphoton events at gen level
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

#include <TTree.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <TString.h>
#include <TLorentzVector.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

int main(int argc, char* argv[])
{

  ////////////////////////////////////////////////////////////////////////////////
  // basic setup
  ////////////////////////////////////////////////////////////////////////////////

  using namespace std;
  using namespace edm;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // use the arguements to get a filename, MS, nED, etc.
  if(argc<=1) {
    std::cout << "USAGE: MakeADDGenTree anatype" << std::endl;
    return 0;
  }

  int anatype=std::atoi(argv[1]);
  std::string inname, outname;
  double MS;
  double xs;
  int NED;
  int KK;
  if(anatype==1)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS100000_NED4_KK1.root"; MS=100000.; NED=4; KK=1; xs=2.64583; }
  if(anatype==2)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED2_KK1.root"; MS=2500.; NED=2; KK=1; xs=2.71261; }
  if(anatype==3)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED3_KK1.root"; MS=2500.; NED=3; KK=1; xs=2.72516; }
  if(anatype==4)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED4_KK1.root"; MS=2500.; NED=4; KK=1; xs=2.67353; }
  if(anatype==5)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED5_KK1.root"; MS=2500.; NED=5; KK=1; xs=2.66219; }
  if(anatype==6)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED6_KK1.root"; MS=2500.; NED=6; KK=1; xs=2.65839; }
  if(anatype==7)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED7_KK1.root"; MS=2500.; NED=7; KK=1; xs=2.65477; }
  if(anatype==8)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED2_KK1.root"; MS=3000.; NED=2; KK=1; xs=2.67602; }
  if(anatype==9)  { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED3_KK1.root"; MS=3000.; NED=3; KK=1; xs=2.67279; }
  if(anatype==10) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED4_KK1.root"; MS=3000.; NED=4; KK=1; xs=2.65698; }
  if(anatype==11) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED5_KK1.root"; MS=3000.; NED=5; KK=1; xs=2.65339; }
  if(anatype==12) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED6_KK1.root"; MS=3000.; NED=6; KK=1; xs=2.65153; }
  if(anatype==13) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED7_KK1.root"; MS=3000.; NED=7; KK=1; xs=2.65055; }
  if(anatype==14) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED2_KK1.root"; MS=3500.; NED=2; KK=1; xs=2.66209; }
  if(anatype==15) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED3_KK1.root"; MS=3500.; NED=3; KK=1; xs=2.65815; }
  if(anatype==16) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED4_KK1.root"; MS=3500.; NED=4; KK=1; xs=2.64997; }
  if(anatype==17) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED5_KK1.root"; MS=3500.; NED=5; KK=1; xs=2.64974; }
  if(anatype==18) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED6_KK1.root"; MS=3500.; NED=6; KK=1; xs=2.64894; }
  if(anatype==19) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED7_KK1.root"; MS=3500.; NED=7; KK=1; xs=2.64721; }
  if(anatype==20) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED2_KK1.root"; MS=4000.; NED=2; KK=1; xs=2.65868; }
  if(anatype==21) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED3_KK1.root"; MS=4000.; NED=3; KK=1; xs=2.65352; }
  if(anatype==22) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED4_KK1.root"; MS=4000.; NED=4; KK=1; xs=2.64834; }
  if(anatype==23) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED5_KK1.root"; MS=4000.; NED=5; KK=1; xs=2.64856; }
  if(anatype==24) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED6_KK1.root"; MS=4000.; NED=6; KK=1; xs=2.64688; }
  if(anatype==25) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED7_KK1.root"; MS=4000.; NED=7; KK=1; xs=2.64659; }
  if(anatype==26) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED2_KK1.root"; MS=4500.; NED=2; KK=1; xs=2.65321; }
  if(anatype==27) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED3_KK1.root"; MS=4500.; NED=3; KK=1; xs=2.64946; }
  if(anatype==28) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED4_KK1.root"; MS=4500.; NED=4; KK=1; xs=2.64917; }
  if(anatype==29) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED5_KK1.root"; MS=4500.; NED=5; KK=1; xs=2.64729; }
  if(anatype==30) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED6_KK1.root"; MS=4500.; NED=6; KK=1; xs=2.64605; }
  if(anatype==31) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED7_KK1.root"; MS=4500.; NED=7; KK=1; xs=2.64760; }
  if(anatype==32) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED2_KK1.root"; MS=5000.; NED=2; KK=1; xs=2.65058; }
  if(anatype==33) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED3_KK1.root"; MS=5000.; NED=3; KK=1; xs=2.64959; }
  if(anatype==34) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED4_KK1.root"; MS=5000.; NED=4; KK=1; xs=2.64889; }
  if(anatype==35) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED5_KK1.root"; MS=5000.; NED=5; KK=1; xs=2.64614; }
  if(anatype==36) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED6_KK1.root"; MS=5000.; NED=6; KK=1; xs=2.64604; }
  if(anatype==37) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED7_KK1.root"; MS=5000.; NED=7; KK=1; xs=2.64622; }
  if(anatype==38) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED4_KK3.root"; MS=2500.; NED=4; KK=3; xs=2.66292; }
  if(anatype==39) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED4_KK3.root"; MS=3000.; NED=4; KK=3; xs=2.65312; }
  if(anatype==40) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED4_KK3.root"; MS=3500.; NED=4; KK=3; xs=2.65048; }
  if(anatype==41) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED4_KK3.root"; MS=4000.; NED=4; KK=3; xs=2.64820; }
  if(anatype==42) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED4_KK3.root"; MS=4500.; NED=4; KK=3; xs=2.64676; }
  if(anatype==43) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED4_KK3.root"; MS=5000.; NED=4; KK=3; xs=2.64629; }
  if(anatype==44) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS2500_NED4_KK4.root"; MS=2500.; NED=4; KK=4; xs=2.64376; }
  if(anatype==45) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3000_NED4_KK4.root"; MS=3000.; NED=4; KK=4; xs=2.64364; }
  if(anatype==46) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS3500_NED4_KK4.root"; MS=3500.; NED=4; KK=4; xs=2.64531; }
  if(anatype==47) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4000_NED4_KK4.root"; MS=4000.; NED=4; KK=4; xs=2.64570; }
  if(anatype==48) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS4500_NED4_KK4.root"; MS=4500.; NED=4; KK=4; xs=2.64506; }
  if(anatype==49) { inname="/uscms/home/johnpaul/nobackup/ADD/Sherpa_GEN_MS5000_NED4_KK4.root"; MS=5000.; NED=4; KK=4; xs=2.64696; }
  ostringstream oss;
  oss << "ADDGenTree_MS" << MS << "_NED" << NED << "_KK" << KK << ".root";
  outname=oss.str();

  // setup the tree
  TFile* outfile=new TFile(outname.c_str(), "RECREATE");
  outfile->cd();
  TTree* mytree = new TTree("addgentree","addgentree");
  double et1, eta1, phi1;
  double et2, eta2, phi2;
  double invmass;
  double weight;

  mytree->Branch("MS",&MS,"MS/D");
  mytree->Branch("NED",&NED,"NED/I");
  mytree->Branch("KK",&KK,"KK/I");
  mytree->Branch("weight",&weight,"weight/D");
  mytree->Branch("et1",&et1,"et1/D");
  mytree->Branch("eta1",&eta1,"eta1/D");
  mytree->Branch("phi1",&phi1,"phi1/D");
  mytree->Branch("et2",&et2,"et2/D");
  mytree->Branch("eta2",&eta2,"eta2/D");
  mytree->Branch("phi2",&phi2,"phi2/D");
  mytree->Branch("invmass",&invmass,"invmass/D");

  // start loop
  cout << "Starting loop." << endl;
  unsigned int iEvent=0;
  std::vector<std::string> filenames;
  filenames.push_back(inname);
  fwlite::ChainEvent ev(filenames);
  int numevents=ev.size();
  weight=xs/numevents;
  for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
    edm::EventBase const & event = ev;

    // simple event counter
    if(iEvent%1000==0){
      std::cout << "processing event " << iEvent << " out of " << numevents << std::endl;
    }

    // get genparticles
    edm::Handle<reco::GenParticleCollection> genpcoll;
    event.getByLabel(edm::InputTag("genParticles"), genpcoll);
    if(!(genpcoll.isValid())) {
      std::cout << "Could not find genParticles" << std::endl;
      return 0;
    }
    
    // loop over gen particles
    const reco::GenParticle* leading=0;
    const reco::GenParticle* subleading=0;
    for(unsigned int i=0; i<genpcoll->size(); i++) {
      const reco::GenParticle *p=&(genpcoll->at(i));
      if(p->pdgId()!=22) continue;
      if(p->status()!=1) continue;

      if(!leading) {
	leading=p;
      } else if(p->pt()>leading->pt()) {
	subleading=leading;
	leading=p;
      } else if(!subleading) {
	subleading=p;
      } else if(p->pt()>subleading->pt()) {
	subleading=p;
      }
    }
    et1=leading->pt();
    eta1=leading->eta();
    phi1=leading->phi();
    et2=subleading->pt();
    eta2=subleading->eta();
    phi2=subleading->phi();

    TLorentzVector v1, v2;
    v1.SetPtEtaPhiM(et1, eta1, phi1, 0.0);
    v2.SetPtEtaPhiM(et2, eta2, phi2, 0.0);
    v1+=v2;
    invmass=v1.Mag();

    mytree->Fill();
  }

  outfile->Write();
  return 0;
}



