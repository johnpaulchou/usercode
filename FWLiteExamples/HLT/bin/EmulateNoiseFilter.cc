////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// EmulateNoiseFilter:
//     Emulate the noise filter
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

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"

int main(int argc, char* argv[]) 
{
  using namespace std;
  using namespace edm;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // get the list of files
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << "[files...]" << std::endl;
    return 0;
  }
  vector<string> filenames;
  for(int i=1; i<argc; i++) {
    cout << "Processing file " << argv[i] << endl;
    filenames.push_back(argv[i]);
  }

  int Jet15Num=0, Jet30Num=0, Jet50Num=0, Met45Num=0, Met100Num=0;
  int Jet15Den=0, Jet30Den=0, Jet50Den=0, Met45Den=0, Met100Den=0;

  // loop over the events
  cout << "Starting loop." << endl;
  unsigned int iEvent=0;
  fwlite::ChainEvent ev(filenames);
  int numevents=ev.size();
  for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
    edm::EventBase const & event = ev;

    // simple event counter
    if(iEvent%1000==0){
      std::cout << "processing event " << iEvent << " out of " << numevents << std::endl;
    }

    // get the jets
    edm::Handle<reco::CaloJetCollection> jets_h;
    event.getByLabel(edm::InputTag("iterativeCone5CaloJets"),jets_h);
    if(!jets_h.isValid()) {
      std::cout << "Could not find jet collection" << std::endl;
      continue;
    }

    // get the met
    edm::Handle<reco::CaloMETCollection> met_h;
    event.getByLabel(edm::InputTag("met"),met_h);
    if(!met_h.isValid()) {
      std::cout << "Could not find met collection" << std::endl;
      continue;
    }

    // get the rbxs
    edm::Handle<reco::HcalNoiseRBXCollection> rbxs_h;
    event.getByLabel(edm::InputTag("hcalnoise"),rbxs_h);
    if(!rbxs_h.isValid()) {
      std::cout << "Could not find rbx collection" << std::endl;
      continue;
    }

    // get the hits
    edm::Handle<HBHERecHitCollection> hbhehits_h;
    event.getByLabel(edm::InputTag("hbhereco"),hbhehits_h);
    if(!hbhehits_h.isValid()) {
      std::cout << "Could not find hbhe hit collection" << std::endl;
      continue;
    }

    // emulate jet/met triggers
    bool passJet15=false, passJet30=false, passJet50=false, passMet45=false, passMet100=false;
    for(int i=0; i<static_cast<int>(jets_h->size()); i++) {
      const reco::CaloJet* jet=&jets_h->at(i);
      if(jet->pt()>15) passJet15=true;
      if(jet->pt()>30) passJet30=true;
      if(jet->pt()>50) passJet50=true;
    }
    if(met_h->at(0).pt()>45) passMet45=true;
    if(met_h->at(0).pt()>100) passMet100=true;

    if(!passJet15 && !passMet45) continue;
    std::cout << passJet15 << " " << passJet30 << " " << passJet50 << " " << passMet45 << " " << passMet100 << std::endl;

    // determine if the event passed the filter
    bool passNoiseFilter=true;
    for(int i=0; i<static_cast<int>(rbxs_h->size()); i++) {
      reco::HcalNoiseRBX rbx=rbxs_h->at(i);
      double rbxe=rbx.recHitEnergy();
      if(rbxe<50.0) continue;
      std::vector<float> charge=rbx.allCharge();
      double e2=0, e10=0;
      for(int i=0; i<static_cast<int>(charge.size()); i++) {
	e10 += charge[i];
	if(i==4 || i==5) e2+= charge[i];
      }
      double ratio = e2/e10;
      int nrbxhits=rbx.numRecHits();
      int nhpdhits=0;
      for(std::vector<reco::HcalNoiseHPD>::const_iterator it=rbx.HPDsBegin(); it!=rbx.HPDsEnd(); ++it) {
	int temp=it->numRecHits();
	if(temp>nhpdhits) nhpdhits=temp;
      }
      int nhothpdhits = nhpdhits==nrbxhits ? nhpdhits : 0;
      double emenergy=rbx.caloTowerEmE();
      if(emenergy/rbxe<0.02 && (ratio<0.65 || ratio>0.98 || rbx.maxZeros()>=10 || nhpdhits>=17 || nhothpdhits>=10)) {
	passNoiseFilter=false;
	std::cout << emenergy/rbxe << " " << ratio << " " << rbx.maxZeros() << " " << nhpdhits << " " << nhothpdhits << std::endl;
      }
    }

    // count the RBXs with E>10 GeV
    int nE10RBX=0;
    std::map<int, double> rbxenergymap;
    for(int i=0; i<static_cast<int>(hbhehits_h->size()); i++) {
      const HBHERecHit hit=(*hbhehits_h)[i];
      if(hit.energy()<1.5) continue;
      int rbxindex=HcalHPDRBXMap::indexRBX(hit.id());
      rbxenergymap[rbxindex]+=hit.energy();
    }
    for(std::map<int, double>::const_iterator it=rbxenergymap.begin(); it!=rbxenergymap.end(); ++it) {
      if(it->second>10.0) nE10RBX++;
    }
    std::cout << passNoiseFilter << " " << nE10RBX << "\n" << std::endl;

    if(passJet15) Jet15Den++;
    if(passJet15 && (passNoiseFilter || nE10RBX>2)) Jet15Num++;
    if(passJet30) Jet30Den++;
    if(passJet30 && (passNoiseFilter || nE10RBX>2)) Jet30Num++;
    if(passJet50) Jet50Den++;
    if(passJet50 && (passNoiseFilter || nE10RBX>2)) Jet50Num++;
    if(passMet45) Met45Den++;
    if(passMet45 && (passNoiseFilter || nE10RBX>2)) Met45Num++;
    if(passMet100) Met100Den++;
    if(passMet100 && (passNoiseFilter || nE10RBX>2)) Met100Num++;
  }

  std::cout << "passJet15= " << Jet15Num << "/" << Jet15Den << "=" << Jet15Num/static_cast<double>(Jet15Den) << std::endl;
  std::cout << "passJet30= " << Jet30Num << "/" << Jet30Den << "=" << Jet30Num/static_cast<double>(Jet30Den) << std::endl;
  std::cout << "passJet50= " << Jet50Num << "/" << Jet50Den << "=" << Jet50Num/static_cast<double>(Jet50Den) << std::endl;
  std::cout << "passMet45= " << Met45Num << "/" << Met45Den << "=" << Met45Num/static_cast<double>(Met45Den) << std::endl;
  std::cout << "passMet100= " << Met100Num << "/" << Met100Den << "=" << Met100Num/static_cast<double>(Met100Den) << std::endl;

  // finished
  return 0;
}
