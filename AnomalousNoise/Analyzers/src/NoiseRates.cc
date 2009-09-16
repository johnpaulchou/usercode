//
// NoiseRates.cc
//
//   description: Calculation for single particle response corrections
//
//   author: J.P. Chou, Brown
//
//

#include "AnomalousNoise/Analyzers/interface/NoiseRates.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "FWCore/Utilities/interface/EDMException.h"

// L1 stuff
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

// root stuff
#include "TFile.h"
#include "TAxis.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

//
// constructors and destructor
//

NoiseRates::NoiseRates(const edm::ParameterSet& iConfig)
{
  // set parameters
  rbxCollName_      = iConfig.getParameter<std::string>("rbxCollName");
  metCollName_      = iConfig.getParameter<std::string>("metCollName");
  rootHistFilename_ = iConfig.getParameter<std::string>("rootHistFilename");
  trigNames_        = iConfig.getParameter<std::vector<std::string> >("trigNames");
  minRBXEnergy_     = iConfig.getParameter<double>("minRBXEnergy");
  minHitEnergy_     = iConfig.getParameter<double>("minHitEnergy");
}
  
  
NoiseRates::~NoiseRates()
{
}
  
  
//
// member functions
//
  
// ------------ method called to for each event  ------------
void
NoiseRates::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup)
{
  // get the lumi section
  int lumiSection = iEvent.luminosityBlock();
  lumiCountMap_[lumiSection]++;

  // get the L1 results
  std::vector<bool> l1results(trigNames_.size(), false);
  for(unsigned int i=0; i<trigNames_.size(); i++) {
    l1results[i] = l1AlgorithmResult(iEvent, evSetup, trigNames_[i]);
    if(l1results[i]) hL1Cntr_->Fill(i);
  }

  // get the MET
  edm::Handle<reco::CaloMETCollection> handle1;
  iEvent.getByLabel(metCollName_,handle1);
  if(!handle1.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloMET named " << metCollName_ << ".\n";
    return;
  }
  const reco::CaloMET &calomet = *(handle1->begin());

  // get the RBX Noise collection
  edm::Handle<reco::HcalNoiseRBXCollection> handle2;
  iEvent.getByLabel(rbxCollName_,handle2);
  if(!handle2.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << rbxCollName_ << ".\n";
    return;
  }

  // get the Noise summary object
  edm::Handle<HcalNoiseSummary> handle3;
  iEvent.getByType(handle3);
  if(!handle3.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound) << " could not find HcalNoiseSummary.\n";
    return;
  }
  const HcalNoiseSummary summary = *handle3;
  bool passLoose = summary.passLooseNoiseFilter();
  bool passTight = summary.passTightNoiseFilter();

  // determine whether we pass the flag requirement
  bool passFlag=true;
  for(reco::HcalNoiseRBXCollection::const_iterator it=handle2->begin(); it!=handle2->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);

    // get the RecHits by way of the HPDs
    std::vector<reco::HcalNoiseHPD> hpds=rbx.HPDs();
    for(std::vector<reco::HcalNoiseHPD>::const_iterator hit=hpds.begin(); hit!=hpds.end(); ++hit) {
      const edm::RefVector<HBHERecHitCollection> rechits=hit->recHits();
      for(edm::RefVector<HBHERecHitCollection>::const_iterator rit=rechits.begin(); rit!=rechits.end(); ++rit) {
	const HBHERecHit &rechit=(**rit);
	if((rechit.flags() & 0x3) && rechit.energy()>20.0) {
	  passFlag=false;
	  break;
	}
      }
    }
  }

  // loop over the RBXs and fill the histograms
  for(reco::HcalNoiseRBXCollection::const_iterator it=handle2->begin(); it!=handle2->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);
    double energy=rbx.recHitEnergy(minHitEnergy_);
    int nhits=rbx.numRecHits(minHitEnergy_);
    if(energy < minRBXEnergy_) continue;

    for(unsigned int i=0; i<l1results.size(); i++) {
      if(!l1results[i]) continue;
      hRBXEnergy_->Fill(energy, i);
      if(nhits<=9)
	hRBXEnergyType1_->Fill(energy, i);
      else if(nhits<=18)
	hRBXEnergyType2_->Fill(energy, i);
      else
	hRBXEnergyType3_->Fill(energy, i);
      
      if(passFlag)  hRBXEnergyAfterCut1_->Fill(energy, i);
      if(passLoose) hRBXEnergyAfterCut2_->Fill(energy, i);
      if(passTight) hRBXEnergyAfterCut3_->Fill(energy, i);
      
      hRBXNHits_->Fill(nhits, i);
    }
  }   // done looping over RBXs

  // fill met
  double met = calomet.pt();
  for(unsigned int i=0; i<l1results.size(); i++) {
    if(!l1results[i]) continue;
    hMET_->Fill(met, i);
    if(passFlag)  hMETAfterCut1_->Fill(met, i);
    if(passLoose) hMETAfterCut2_->Fill(met, i);
    if(passTight) hMETAfterCut3_->Fill(met, i);
  }

  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
NoiseRates::beginJob(const edm::EventSetup&)
{
  int ntrigs=trigNames_.size();
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hL1Cntr_ = new TH1D("hL1Cntr","hL1Cntr",ntrigs, -0.5, ntrigs-0.5);
  hLumiBlockCount_ = new TH1D("hLumiBlockCount","LumiBlockCount",1,-0.5,0.5);
  hRBXEnergy_ = new TH2D("hRBXEnergy","hRBXEnergy",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyType1_ = new TH2D("hRBXEnergyType1","hRBXEnergyType1",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyType2_ = new TH2D("hRBXEnergyType2","hRBXEnergyType2",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyType3_ = new TH2D("hRBXEnergyType3","hRBXEnergyType3",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXNHits_ = new TH2D("hRBXNHits","hRBXNHits",73,-0.5,72.5,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyAfterCut1_ = new TH2D("hRBXEnergyAfterCut1","hRBXEnergy After Cut 1",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyAfterCut2_ = new TH2D("hRBXEnergyAfterCut2","hRBXEnergy After Cut 1",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hRBXEnergyAfterCut3_ = new TH2D("hRBXEnergyAfterCut3","hRBXEnergy After Cut 1",100,0,1000,ntrigs, -0.5, ntrigs-0.5);

  hMET_ = new TH2D("hMET","hMET",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hMETAfterCut1_ = new TH2D("hMETAfterCut1","hMETAfterCut1",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hMETAfterCut2_ = new TH2D("hMETAfterCut2","hMETAfterCut2",100,0,1000,ntrigs, -0.5, ntrigs-0.5);
  hMETAfterCut3_ = new TH2D("hMETAfterCut3","hMETAfterCut3",100,0,1000,ntrigs, -0.5, ntrigs-0.5);

  for(unsigned int i=0; i<trigNames_.size(); i++)
    hL1Cntr_->GetXaxis()->SetBinLabel(i+1, trigNames_[i].c_str());

}

// ------------ method called once each job just after ending the event loop  ------------
void 
NoiseRates::endJob() {

  // write histograms
  rootfile_->cd();

  hL1Cntr_->Write();
  hRBXEnergy_->Write();
  hRBXEnergyType1_->Write();
  hRBXEnergyType2_->Write();
  hRBXEnergyType3_->Write();
  hRBXEnergyAfterCut1_->Write();
  hRBXEnergyAfterCut2_->Write();
  hRBXEnergyAfterCut3_->Write();
  hMET_->Write();
  hMETAfterCut1_->Write();
  hMETAfterCut2_->Write();
  hMETAfterCut3_->Write();
  hRBXNHits_->Write();

  // dump the luminosity block info
  for(std::map<int, int>::const_iterator it=lumiCountMap_.begin(); it!=lumiCountMap_.end(); ++it) {
    std::cout << "lumiblock=" << it->first << "; count=" << it->second << std::endl;
  }
  hLumiBlockCount_->Fill(0.0, lumiCountMap_.size());
  hLumiBlockCount_->Write();

  rootfile_->Close();

}


//define this as a plug-in
DEFINE_FWK_MODULE(NoiseRates);
