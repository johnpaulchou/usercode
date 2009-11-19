//
// NoiseRates.cc
//
//   description: Calculation for noise rates
//
//   author: J.P. Chou, Brown
//
//

#include "AnomalousNoise/Analyzers/interface/NoiseRates.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "FWCore/Utilities/interface/EDMException.h"

// L1 stuff
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"

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
  jetCollName_      = iConfig.getParameter<std::string>("jetCollName");
  metCollName_      = iConfig.getParameter<std::string>("metCollName");
  hbheRecHitCollName_ = iConfig.getParameter<std::string>("hbheRecHitCollName");
  caloTowerCollName_ = iConfig.getParameter<std::string>("caloTowerCollName");
  rootHistFilename_ = iConfig.getParameter<std::string>("rootHistFilename");
  findTrigger_      = iConfig.getParameter<bool>("findTrigger");
  trigName_         = iConfig.getParameter<std::string>("trigName");
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
  /*  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  evSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  std::vector<std::string> names;
  for (CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
    names.push_back(algo->second.algoName());
  }
  for(unsigned int i=0; i<names.size(); i++) {
    if(l1AlgorithmResult(iEvent, evSetup, names[i])) {
      std::cout << "Found trigger=" << names[i] << std::endl;
    }
  }
  */

  // get the lumi section
  int lumiSection = iEvent.luminosityBlock();
  lumiCountMap_[lumiSection]++;

  // get the L1 results
  if(findTrigger_ && !l1AlgorithmResult(iEvent, evSetup, trigName_)) return;

  // get the MET
  edm::Handle<reco::CaloMETCollection> met_h;
  iEvent.getByLabel(metCollName_,met_h);
  if(!met_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloMET named " << metCollName_ << ".\n";
    return;
  }
  const reco::CaloMET &calomet = *(met_h->begin());

  // get the Jets
  edm::Handle<reco::CaloJetCollection> jets_h;
  iEvent.getByLabel(jetCollName_, jets_h);
  if(!jets_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloJet collection name " << jetCollName_ << ".\n";
    return;
  }

  // get the RBX Noise collection
  edm::Handle<reco::HcalNoiseRBXCollection> rbxs_h;
  iEvent.getByLabel(rbxCollName_,rbxs_h);
  if(!rbxs_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << rbxCollName_ << ".\n";
    return;
  }

  // get the Noise summary object
  edm::Handle<HcalNoiseSummary> summary_h;
  iEvent.getByType(summary_h);
  if(!summary_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound) << " could not find HcalNoiseSummary.\n";
    return;
  }
  const HcalNoiseSummary summary = *summary_h;
  bool passLoose = summary.passLooseNoiseFilter();
  bool passTight = summary.passTightNoiseFilter();
  bool passTriggerFilter = true;

  // determine if we pass the trigger filter
  for(reco::HcalNoiseRBXCollection::const_iterator it=rbxs_h->begin(); it!=rbxs_h->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);
    int rbxhits=rbx.numRecHits(minHitEnergy_);
    double rbxemf=rbx.caloTowerEmFraction();
    if(rbxhits>=50 && rbxemf<=0.01) passTriggerFilter=false;
    else {
      std::vector<reco::HcalNoiseHPD> hpds=rbx.HPDs();
      for(std::vector<reco::HcalNoiseHPD>::const_iterator hit=hpds.begin(); hit!=hpds.end(); ++hit) {
	int hpdhits=hit->numRecHits(minHitEnergy_);
	double hpdemf=hit->caloTowerEmFraction();
	if(hpdhits>=17 && hpdemf<=0.01) passTriggerFilter=false;
      }
    }
  }

  passLoose=true;
  passTight=true;
  if(summary.minE2Over10TS()<0.7 ||
     summary.min25GeVHitTime()<18. ||
     summary.max25GeVHitTime()>31. ||
     summary.maxZeros()>8)
    passLoose=false;
  if(summary.minE2Over10TS()<0.8 ||
     summary.min25GeVHitTime()<20. ||
     summary.max25GeVHitTime()>29. ||
     summary.maxZeros()>7 ||
     summary.maxHPDHits()>16)
    passTight=false;

  // get the rechits
  edm::Handle<HBHERecHitCollection> rechits_h;
  iEvent.getByLabel(hbheRecHitCollName_,rechits_h);
  if(!rechits_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HBHERecHitCollection named " << hbheRecHitCollName_ << ".\n";
    return;
  }

  // get the caloTowers
  edm::Handle<CaloTowerCollection> towers_h;
  iEvent.getByLabel(caloTowerCollName_,towers_h);
  if(!towers_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CalotTowerCollection named " << caloTowerCollName_ << ".\n";
    return;
  }

  // determine whether we pass the flag requirement
  bool passFlag=true;
  for(HBHERecHitCollection::const_iterator it=rechits_h->begin(); it!=rechits_h->end(); ++it) {
    const HBHERecHit &rechit=(*it);
    if(rechit.flags() & 0x3) {
      passFlag=false;
    }
    hHitEnergy_->Fill(rechit.energy());
  }

  double met = calomet.pt();

  // loop over the RBXs and fill the histograms
  int cnt=0;
  for(reco::HcalNoiseRBXCollection::const_iterator it=rbxs_h->begin(); it!=rbxs_h->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);
    double energy=rbx.recHitEnergy(minHitEnergy_);
    int nhits=rbx.numRecHits(minHitEnergy_);
    if(energy < minRBXEnergy_) continue;
    ++cnt;
    hRBXEnergy_->Fill(energy);
    if(nhits<=9)
      hRBXEnergyType1_->Fill(energy);
    else if(nhits<=18)
      hRBXEnergyType2_->Fill(energy);
    else
      hRBXEnergyType3_->Fill(energy);
    
    if(passFlag)  hRBXEnergyAfterCut1_->Fill(energy);
    if(passLoose) hRBXEnergyAfterCut2_->Fill(energy);
    if(passTight) hRBXEnergyAfterCut3_->Fill(energy);
    
    hRBXNHits_->Fill(nhits);

    hRBXEnergyMET_->Fill(met);
    if (nhits<=9)
      hRBXEnergyType1MET_->Fill(met);
    else if (nhits<=18)
      hRBXEnergyType2MET_->Fill(met);
    else
      hRBXEnergyType3MET_->Fill(met);


  }   // done looping over RBXs
  if(cnt>1) std::cout << "count = " << cnt << std::endl;

  // fill met
  hMET_->Fill(met);

  if(passFlag)          hMETAfterCut1_->Fill(met);
  if(passLoose)         hMETAfterCut2_->Fill(met);
  if(passTight)         hMETAfterCut3_->Fill(met);
  if(passTriggerFilter) hMETAfterCut4_->Fill(met);

  // fill distributions
  if(summary.maxHPDHits()>=1) {
    hMin10GeVHitTime_->Fill(summary.min10GeVHitTime());
    hMax10GeVHitTime_->Fill(summary.max10GeVHitTime());
    hMin25GeVHitTime_->Fill(summary.min25GeVHitTime());
    hMax25GeVHitTime_->Fill(summary.max25GeVHitTime());
    hMinE2Over10TS_->Fill(summary.minE2Over10TS());
    hMaxZeros_->Fill(summary.maxZeros());
    hMaxHPDHits_->Fill(summary.maxHPDHits());
    hMaxRBXHits_->Fill(summary.maxRBXHits());
    hMinHPDEMF_->Fill(summary.minHPDEMF());
    hMinRBXEMF_->Fill(summary.minRBXEMF());
  }

  // fill jets
  double maxJetEt=-999;
  for(CaloJetCollection::const_iterator it=jets_h->begin(); it!=jets_h->end(); ++it) {
    const CaloJet &jet=(*it);
    if(jet.pt()>maxJetEt) maxJetEt=jet.pt();
  }
  hJetEt_->Fill(maxJetEt);
  if(passFlag)  hJetEtAfterCut1_->Fill(maxJetEt);
  if(passLoose) hJetEtAfterCut2_->Fill(maxJetEt);
  if(passTight) hJetEtAfterCut3_->Fill(maxJetEt);
  if(passTriggerFilter) hJetEtAfterCut4_->Fill(maxJetEt);

  // fill "trigger towers"
  std::map<std::pair<int, int>, double> hcalenergymap;
  for(CaloTowerCollection::const_iterator it=towers_h->begin(); it!=towers_h->end(); ++it) {
    const CaloTower &twr=(*it);
    double hcale=twr.hadEnergy();
    int ieta=twr.ieta();
    int iphi=twr.iphi();

    // one-to-one
    if(twr.ietaAbs() < theTopology.firstHEDoublePhiRing()) {
      hcalenergymap[std::pair<int, int>(ieta, iphi)] += hcale;
      triggerlist_.insert(std::pair<int, int>(ieta, iphi));

    // two-to-one
    } else if(twr.ietaAbs() < 30) {
      if(ieta == theTopology.lastHERing())  --ieta;
      if(ieta == -theTopology.lastHERing()) ++ieta;
      hcalenergymap[std::pair<int, int>(ieta, iphi)]   += hcale/2.0;
      hcalenergymap[std::pair<int, int>(ieta, iphi+1)] += hcale/2.0;
      triggerlist_.insert(std::pair<int, int>(ieta, iphi));
      triggerlist_.insert(std::pair<int, int>(ieta, iphi+1));
    }
  }

  // fill the trigger towers histogram
  for(std::map<std::pair<int, int>, double>::const_iterator it=hcalenergymap.begin(); it!=hcalenergymap.end(); ++it) {
    hTrigTowersEnergy_->Fill(it->second);
    if(summary.maxRBXHits()<=9) hTrigTowersEnergyType1_->Fill(it->second);
    else if(summary.maxRBXHits()<=18) hTrigTowersEnergyType2_->Fill(it->second);
    else hTrigTowersEnergyType3_->Fill(it->second);
  }

  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
NoiseRates::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hLumiBlockCount_ = new TH1D("hLumiBlockCount","LumiBlockCount",1,-0.5,0.5);
  hRBXEnergy_ = new TH1D("hRBXEnergy","hRBXEnergy",100,0,1000);
  hRBXEnergyType1_ = new TH1D("hRBXEnergyType1","hRBXEnergyType1",100,0,1000);
  hRBXEnergyType2_ = new TH1D("hRBXEnergyType2","hRBXEnergyType2",100,0,1000);
  hRBXEnergyType3_ = new TH1D("hRBXEnergyType3","hRBXEnergyType3",100,0,1000);
  hRBXEnergyMET_ = new TH1D("hRBXEnergyMET","hRBXEnergy",100,0,1000);
  hRBXEnergyType1MET_ = new TH1D("hRBXEnergyType1MET","hRBXEnergyType1 vs MET",100,0,1000);
  hRBXEnergyType2MET_ = new TH1D("hRBXEnergyType2MET","hRBXEnergyType2 vs MET",100,0,1000);
  hRBXEnergyType3MET_ = new TH1D("hRBXEnergyType3MET","hRBXEnergyType3 vs MET",100,0,1000);

  hRBXNHits_ = new TH1D("hRBXNHits","hRBXNHits",73,-0.5,72.5);
  hRBXEnergyAfterCut1_ = new TH1D("hRBXEnergyAfterCut1","hRBXEnergy After Cut 1",100,0,1000);
  hRBXEnergyAfterCut2_ = new TH1D("hRBXEnergyAfterCut2","hRBXEnergy After Cut 1",100,0,1000);
  hRBXEnergyAfterCut3_ = new TH1D("hRBXEnergyAfterCut3","hRBXEnergy After Cut 1",100,0,1000);
  hMET_ = new TH1D("hMET","hMET",1000,0,1000);
  hMETAfterCut1_ = new TH1D("hMETAfterCut1","hMETAfterCut1",1000,0,1000);
  hMETAfterCut2_ = new TH1D("hMETAfterCut2","hMETAfterCut2",1000,0,1000);
  hMETAfterCut3_ = new TH1D("hMETAfterCut3","hMETAfterCut3",1000,0,1000);
  hMETAfterCut4_ = new TH1D("hMETAfterCut4","hMETAfterCut4",1000,0,1000);
  hJetEt_ = new TH1D("hJetEt","hJetEt",1000,0,1000);
  hJetEtAfterCut1_ = new TH1D("hJetEtAfterCut1","hJetEtAfterCut1",1000,0,1000);
  hJetEtAfterCut2_ = new TH1D("hJetEtAfterCut2","hJetEtAfterCut2",1000,0,1000);
  hJetEtAfterCut3_ = new TH1D("hJetEtAfterCut3","hJetEtAfterCut3",1000,0,1000);
  hJetEtAfterCut4_ = new TH1D("hJetEtAfterCut4","hJetEtAfterCut4",1000,0,1000);
  hMin10GeVHitTime_ = new TH1D("hMin10GeVHitTime","Min10GeVHitTime",100,-50,50);
  hMax10GeVHitTime_ = new TH1D("hMax10GeVHitTime","Max10GeVHitTime",100,-50,50);
  hMin25GeVHitTime_ = new TH1D("hMin25GeVHitTime","Min25GeVHitTime",100,-50,50);
  hMax25GeVHitTime_ = new TH1D("hMax25GeVHitTime","Max25GeVHitTime",100,-50,50);
  hMinE2Over10TS_   = new TH1D("hMinE2Over10TS","MinE2Over10TS",100,0,1.5);
  hMaxZeros_        = new TH1D("hMaxZeros","MaxZeros",25,0,25);
  hMaxHPDHits_      = new TH1D("hMaxHPDHits","MaxHPDHits",19,0,19);
  hMaxRBXHits_      = new TH1D("hMaxRBXHits","MaxRBXHits",73,0,73);
  hMinHPDEMF_       = new TH1D("hMinHPDEMF","MinHPDEMF",100,0,1.);
  hMinRBXEMF_       = new TH1D("hMinRBXEMF","MinRBXEMF",100,0,1.);

  hHitEnergy_ = new TH1D("hHitEnergy","Hit Energy",100,0,1000);

  hTrigTowersEnergy_ = new TH1D("hTrigTowersEnergy","TrigTowersEnergy",100,0,1000);
  hTrigTowersEnergyType1_ = new TH1D("hTrigTowersEnergyType1","TrigTowersEnergy",100,0,1000);
  hTrigTowersEnergyType2_ = new TH1D("hTrigTowersEnergyType2","TrigTowersEnergy",100,0,1000);
  hTrigTowersEnergyType3_ = new TH1D("hTrigTowersEnergyType3","TrigTowersEnergy",100,0,1000);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
NoiseRates::endJob() {

  // write histograms
  rootfile_->cd();

  hRBXEnergy_->Write();
  hRBXEnergyType1_->Write();
  hRBXEnergyType2_->Write();
  hRBXEnergyType3_->Write();
  
  hRBXEnergyMET_->Write();
  hRBXEnergyType1MET_->Write();
  hRBXEnergyType2MET_->Write();
  hRBXEnergyType3MET_->Write();

  hRBXEnergyAfterCut1_->Write();
  hRBXEnergyAfterCut2_->Write();
  hRBXEnergyAfterCut3_->Write();
  hMET_->Write();
  hMETAfterCut1_->Write();
  hMETAfterCut2_->Write();
  hMETAfterCut3_->Write();
  hMETAfterCut4_->Write();
  hJetEt_->Write();
  hJetEtAfterCut1_->Write();
  hJetEtAfterCut2_->Write();
  hJetEtAfterCut3_->Write();
  hJetEtAfterCut4_->Write();
  hRBXNHits_->Write();

  hMin10GeVHitTime_->Write();
  hMax10GeVHitTime_->Write();
  hMin25GeVHitTime_->Write();
  hMax25GeVHitTime_->Write();
  hMinE2Over10TS_->Write();
  hMaxZeros_->Write();
  hMaxHPDHits_->Write();
  hMaxRBXHits_->Write();
  hMinHPDEMF_->Write();
  hMinRBXEMF_->Write();
  
  hHitEnergy_->Write();

  hTrigTowersEnergy_->Write();
  hTrigTowersEnergyType1_->Write();
  hTrigTowersEnergyType2_->Write();
  hTrigTowersEnergyType3_->Write();

  // dump the luminosity block info
  for(std::map<int, int>::const_iterator it=lumiCountMap_.begin(); it!=lumiCountMap_.end(); ++it) {
    std::cout << "lumiblock=" << it->first << "; count=" << it->second << std::endl;
  }
  hLumiBlockCount_->Fill(0.0, lumiCountMap_.size());
  hLumiBlockCount_->Write();

  // dump the trigger list
  for(std::set<std::pair<int, int> >::const_iterator it=triggerlist_.begin(); it!=triggerlist_.end(); ++it) {
    std::cout << it->first << " " << it->second << std::endl;
  }
  rootfile_->Close();

}


//define this as a plug-in
DEFINE_FWK_MODULE(NoiseRates);
