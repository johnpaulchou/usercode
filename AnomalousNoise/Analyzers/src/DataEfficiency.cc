//
// DataEfficiency.cc
//
//    description: Measure the efficiency of data after noise rejection
//
//    author: J.P. Chou, Brown
//
//

#include "AnomalousNoise/Analyzers/interface/DataEfficiency.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/HcalNoiseRBX.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

// root stuff
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TMath.h"

DataEfficiency::DataEfficiency(const edm::ParameterSet& iConfig)
{
  rbxCollName_     = iConfig.getParameter<std::string>("rbxCollName");
  jetCollName_      = iConfig.getParameter<std::string>("jetCollName");
  metCollName_      = iConfig.getParameter<std::string>("metCollName");
  trackCollName_    = iConfig.getParameter<std::string>("trackCollName");
  hbheRecHitCollName_ = iConfig.getParameter<std::string>("hbheRecHitCollName");
  caloTowerCollName_ = iConfig.getParameter<std::string>("caloTowerCollName");
  rootHistFilename_ = iConfig.getParameter<std::string>("rootHistFilename");
}

DataEfficiency::~DataEfficiency()
{
}

//
// member functions
//

// ------------ method called to for each event  ------------
void
DataEfficiency::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup)
{
  // get the MET
  edm::Handle<reco::CaloMETCollection> met_h;
  iEvent.getByLabel(metCollName_,met_h);
  if(!met_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloMET named " << metCollName_ << ".\n";
    return;
  }
  const reco::CaloMET &calomet = *(met_h->begin());
  double met = calomet.pt();

  // get the Jets
  edm::Handle<reco::CaloJetCollection> jets_h;
  iEvent.getByLabel(jetCollName_, jets_h);
  if(!jets_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloJet collection name " << jetCollName_ << ".\n";
    return;
  }

  // get the tracks
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(trackCollName_, tracks_h);
  if(!tracks_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find TrackCollection name " << trackCollName_ << ".\n";
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
  bool passTrigger = true;

  // determine if we pass the trigger filter
  for(reco::HcalNoiseRBXCollection::const_iterator it=rbxs_h->begin(); it!=rbxs_h->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);
    int rbxhits=rbx.numRecHits(1.0);
    double rbxemf=rbx.caloTowerEmFraction();
    if(rbxhits>=50 && rbxemf<=0.01) passTrigger=false;
    else {
      std::vector<reco::HcalNoiseHPD> hpds=rbx.HPDs();
      for(std::vector<reco::HcalNoiseHPD>::const_iterator hit=hpds.begin(); hit!=hpds.end(); ++hit) {
	int hpdhits=hit->numRecHits(1.0);
	double hpdemf=hit->caloTowerEmFraction();
	if(hpdhits>=17 && hpdemf<=0.01) passTrigger=false;
      }
    }
  }

  // fill distributions
  float energy = summary.eventEMEnergy()+summary.eventHadEnergy();
  if(energy>10) {
    hCHF_->Fill(summary.eventChargeFraction());
    hCHE_->Fill(summary.eventTrackEnergy());
    hCalE_->Fill(summary.eventEMEnergy()+summary.eventHadEnergy());
  }
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

  int njets=0;
  for(CaloJetCollection::const_iterator it=jets_h->begin(); it!=jets_h->end(); ++it) {
    const CaloJet &jet=(*it);
    if(!(jet.pt()>5 && fabs(jet.eta())<3.0)) continue;
    ++njets;
    hJetPt_->Fill(jet.pt());
    hJetEta_->Fill(jet.eta());
    if(passLoose) {
      hJetPtLoose_->Fill(jet.pt());
      hJetEtaLoose_->Fill(jet.eta());
    }
    if(passTight) {
      hJetPtTight_->Fill(jet.pt());
      hJetEtaTight_->Fill(jet.eta());
    }
    if(passTrigger) {
      hJetPtTrigger_->Fill(jet.pt());
      hJetEtaTrigger_->Fill(jet.eta());
    }
  }
  hNJets_->Fill(njets);
  hMET_->Fill(met);
  if(passLoose) {
    hNJetsLoose_->Fill(njets);
    hMETLoose_->Fill(met);
  }
  if(passTight) {
    hNJetsTight_->Fill(njets);
    hMETTight_->Fill(met);
  }
  if(passTrigger) {
    hNJetsTrigger_->Fill(njets);
    hMETTrigger_->Fill(met);
  }

  ntracks_=0;
  sumtrkp_=0;
  for(reco::TrackCollection::const_iterator iTrack = tracks_h->begin(); iTrack!=tracks_h->end(); ++iTrack) {
    reco::Track trk=*iTrack;
    if(trk.pt()<1.0) continue;
    trkpt_[ntracks_]=trk.pt();
    trkp_[ntracks_]=trk.p();
    trketa_[ntracks_]=trk.eta();
    trkphi_[ntracks_]=trk.phi();
    sumtrkp_ += trk.p();
    ++ntracks_;
  }

  njets_=0;
  for(CaloJetCollection::const_iterator it=jets_h->begin(); it!=jets_h->end(); ++it) {
    const CaloJet &jet=(*it);
    jetpt_[njets_] = jet.pt();
    jeteta_[njets_] = jet.eta();
    jetphi_[njets_] = jet.phi();
  }

  hade_=summary.eventHadEnergy();
  eme_=summary.eventEMEnergy();
  tree_->Fill();

  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
DataEfficiency::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");
  tree_ = new TTree("tree","tree");
  tree_->Branch("ntracks",&ntracks_, "ntracks/I");
  tree_->Branch("trkpt",trkpt_, "trkpt[ntracks]/F");
  tree_->Branch("trkp",trkp_, "trkp[ntracks]/F");
  tree_->Branch("trketa",trketa_, "trketa[ntracks]/F");
  tree_->Branch("trkphi",trkphi_, "trkphi[ntracks]/F");
  tree_->Branch("sumtrkp",&sumtrkp_, "sumtrkp/F");
  tree_->Branch("njets",&njets_,"njets/I");
  tree_->Branch("jetpt",jetpt_,"jetpt[njets]/F");
  tree_->Branch("jeteta",jeteta_,"jeteta[njets]/F");
  tree_->Branch("jetphi",jetphi_,"jetphi[njets]/F");
  tree_->Branch("hade",&hade_, "hade/F");
  tree_->Branch("eme",&eme_, "eme/F");


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
  
  hMET_        = new TH1D("hMET","MET",100,0,1000);
  hMETLoose_   = new TH1D("hMETLoose","METLoose",100,0,1000);
  hMETTight_   = new TH1D("hMETTight","METTight",100,0,1000);
  hMETTrigger_ = new TH1D("hMETTrigger","METTrigger",100,0,1000);

  hNJets_        = new TH1D("hNJets","NJets",10,-0.5,9.5);
  hNJetsLoose_   = new TH1D("hNJetsLoose","NJetsLoose",10,-0.5,9.5);
  hNJetsTight_   = new TH1D("hNJetsTight","NJetsTight",10,-0.5,9.5);
  hNJetsTrigger_ = new TH1D("hNJetsTrigger","NJetsTrigger",10,-0.5,9.5);

  hJetPt_        = new TH1D("hJetPt","JetPt",100,0,100);
  hJetPtLoose_   = new TH1D("hJetPtLoose","JetPtLoose",100,0,100);
  hJetPtTight_   = new TH1D("hJetPtTight","JetPtTight",100,0,100);
  hJetPtTrigger_ = new TH1D("hJetPtTrigger","JetPtTrigger",100,0,100);

  hJetEta_        = new TH1D("hJetEta","JetEta",60,-3,3);
  hJetEtaLoose_   = new TH1D("hJetEtaLoose","JetEtaLoose",60,-3,3);
  hJetEtaTight_   = new TH1D("hJetEtaTight","JetEtaTight",60,-3,3);
  hJetEtaTrigger_ = new TH1D("hJetEtaTrigger","JetEtaTrigger",60,-3,3);

  hCHF_           = new TH1D("hCHF","CHF",100,0,2);
  hCHE_           = new TH1D("hCHE","CHE",100,0,1000);
  hCalE_          = new TH1D("hCalE","CalE",100,0,1000);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DataEfficiency::endJob() {

  // write histograms
  rootfile_->cd();

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
  
  hMET_->Write();
  hMETLoose_->Write();
  hMETTight_->Write();
  hMETTrigger_->Write();
  hNJets_->Write();
  hNJetsLoose_->Write();
  hNJetsTight_->Write();
  hNJetsTrigger_->Write();
  hJetPt_->Write();
  hJetPtLoose_->Write();
  hJetPtTight_->Write();
  hJetPtTrigger_->Write();
  hJetEta_->Write();
  hJetEtaLoose_->Write();
  hJetEtaTight_->Write();
  hJetEtaTrigger_->Write();
  hCHF_->Write();
  hCHE_->Write();
  hCalE_->Write();

  tree_->Write();

  rootfile_->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(DataEfficiency);
