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

  // get the RBX Noise collection
  edm::Handle<reco::HcalNoiseRBXCollection> handle;
  iEvent.getByLabel(rbxCollName_,handle);
  if(!handle.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << rbxCollName_ << ".\n";
    return;
  }

  // loop over the RBXs and fill the histograms
  for(reco::HcalNoiseRBXCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
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

      hRBXNHits_->Fill(nhits, i);
    }

  }   // done looping over RBXs
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
