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
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

// trigger stuff
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"

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
  // get the RBX Noise collection
  edm::Handle<reco::HcalNoiseRBXCollection> rbxs_h;
  iEvent.getByLabel(rbxCollName_,rbxs_h);
  if(!rbxs_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << rbxCollName_ << ".\n";
    return;
  }

  // get the hits
  edm::Handle<HBHERecHitCollection> hbhehits_h;
  iEvent.getByLabel(hbheRecHitCollName_, hbhehits_h);
  if(!hbhehits_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HBHERecHitCollection named " << hbheRecHitCollName_ << ".\n";
    return;
  }

  // get the calotowers
  edm::Handle<CaloTowerCollection> twrs_h;
  iEvent.getByLabel(caloTowerCollName_,twrs_h);
  if(!twrs_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloTowerCollection named " << caloTowerCollName_ << ".\n";
    return;
  }

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
  
  // get the tracks
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(trackCollName_, tracks_h);
  //    if(!tracks_h.isValid()) {
  //      throw edm::Exception(edm::errors::ProductNotFound)
  //	<< " could not find TrackCollection name " << trackCollName_ << ".\n";
  //      return;
  //    }
  
  // get the Noise summary object
  edm::Handle<HcalNoiseSummary> summary_h;
  iEvent.getByType(summary_h);
  if(!summary_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound) << " could not find HcalNoiseSummary.\n";
    return;
  }
  const HcalNoiseSummary summary = *summary_h;

  // get the trigger info
  trigger_ = 0;
  /*
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  evSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  edm::Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
  iEvent.getByLabel( edm::InputTag("gtDigis"), gtReadoutRecord);
  const DecisionWord& gtDecisionWordBeforeMask = gtReadoutRecord->decisionWord();
  const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = gtReadoutRecord->technicalTriggerWord();

  trigger_ |= 0x1 * menu->gtAlgorithmResult( "L1_SingleJet6", gtDecisionWordBeforeMask);
  trigger_ |= 0x2 * menu->gtAlgorithmResult( "L1_SingleJet10", gtDecisionWordBeforeMask);
  trigger_ |= 0x4 * menu->gtAlgorithmResult( "L1_SingleEG1", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMuBeamHalo", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMuOpen", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu0", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu3", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu5", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu7", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu10", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu14", gtDecisionWordBeforeMask);
  trigger_ |= 0x8 * menu->gtAlgorithmResult( "L1_SingleMu20", gtDecisionWordBeforeMask);
  trigger_ |= 0x10 * menu->gtAlgorithmResult( "L1_ETT60", gtDecisionWordBeforeMask);
  trigger_ |= 0x20 * menu->gtAlgorithmResult( "L1_ETM20", gtDecisionWordBeforeMask);
  trigger_ |= 0x40 * menu->gtAlgorithmResult( "L1_ETM80", gtDecisionWordBeforeMask);
  trigger_ |= 0x80 * menu->gtAlgorithmResult( "L1_HTT100", gtDecisionWordBeforeMask);
  trigger_ |= 0x100 * menu->gtAlgorithmResult( "L1_SingleJet10U_NotBptxC", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfBitCountsRing1_1", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfBitCountsRing2_1", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfRingEtSumsRing1_4", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfRingEtSumsRing1_200", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfRingEtSumsRing2_4", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_SingleHfRingEtSumsRing2_200", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfBitCountsRing1_P1N1", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfBitCountsRing2_P1N1", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfRingEtSumsRing1_P4N4", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfRingEtSumsRing1_P200N200", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfRingEtSumsRing2_P4N4", gtDecisionWordBeforeMask);
  trigger_ |= 0x200 * menu->gtAlgorithmResult( "L1_DoubleHfRingEtSumsRing2_P200N200", gtDecisionWordBeforeMask);
  trigger_ |= 0x400 * technicalTriggerWordBeforeMask.at(37);
  */
 
  // fill distributions
  nhits_=0;
  for(HBHERecHitCollection::const_iterator it=hbhehits_h->begin(); it!=hbhehits_h->end(); ++it) {
    const HBHERecHit &hit=(*it);
    if(hit.energy()<25) continue;
    hitieta_[nhits_] = hit.id().ieta();
    hitiphi_[nhits_] = hit.id().iphi();
    hitenergy_[nhits_] = hit.energy();
    hittime_[nhits_] = hit.time();

    ++nhits_;
  }

  nrbxs_=0;
  for(reco::HcalNoiseRBXCollection::const_iterator it=rbxs_h->begin(); it!=rbxs_h->end(); ++it) {
    const reco::HcalNoiseRBX &rbx=(*it);
    rbxnhits_[nrbxs_]    = rbx.numRecHits(1.5);
    rbxidnum_[nrbxs_]    = rbx.idnumber();
    rbxnadc0s_[nrbxs_]   = rbx.totalZeros();
    rbxenergy_[nrbxs_]   = rbx.recHitEnergy(1.5);
    rbxcalohade_[nrbxs_] = rbx.caloTowerHadE();
    rbxcaloeme_[nrbxs_]  = rbx.caloTowerEmE();
    float totalq = rbx.allChargeTotal();
    rbxratio_[nrbxs_] = totalq>0 ? rbx.allChargeHighest2TS()/totalq : 0.0;

    rbxnhpds_[nrbxs_] = 0;
    int cnt=0;
    std::vector<reco::HcalNoiseHPD> hpds = rbx.HPDs();
    for(std::vector<reco::HcalNoiseHPD>::const_iterator hpdit=hpds.begin(); hpdit!=hpds.end(); ++hpdit) {
      int nhpdhits = hpdit->numRecHits(1.5);
      if(nhpdhits>0) ++rbxnhpds_[nrbxs_];
      if(cnt==0) rbxnhitshpd0_[nrbxs_]=nhpdhits;
      if(cnt==1) rbxnhitshpd1_[nrbxs_]=nhpdhits;
      if(cnt==2) rbxnhitshpd2_[nrbxs_]=nhpdhits;
      if(cnt==3) rbxnhitshpd3_[nrbxs_]=nhpdhits;
      ++cnt;
    }

    std::vector<float> charge=rbx.allCharge();
    rbx0ts_[nrbxs_] = charge[0];
    rbx1ts_[nrbxs_] = charge[1];
    rbx2ts_[nrbxs_] = charge[2];
    rbx3ts_[nrbxs_] = charge[3];
    rbx4ts_[nrbxs_] = charge[4];
    rbx5ts_[nrbxs_] = charge[5];
    rbx6ts_[nrbxs_] = charge[6];
    rbx7ts_[nrbxs_] = charge[7];
    rbx8ts_[nrbxs_] = charge[8];
    rbx9ts_[nrbxs_] = charge[9];

    ++nrbxs_;
  }

  njets_=0;
  ntracks_=0;
  sumtrkp_=0;
  met_ =0;
  hade_=0;
  eme_=0;
  trackenergy_=0;
  status_=0;

  if(tracks_h.isValid()) {
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
  }
  
  for(CaloJetCollection::const_iterator it=jets_h->begin(); it!=jets_h->end(); ++it) {
    const CaloJet &jet=(*it);
    jetpt_[njets_] = jet.pt();
    jeteta_[njets_] = jet.eta();
    jetphi_[njets_] = jet.phi();
    jetHBhade_[njets_] = jet.hadEnergyInHB();
    jetHEhade_[njets_] = jet.hadEnergyInHE();
    jetEBeme_[njets_] = jet.emEnergyInEB();
    jetEEeme_[njets_] = jet.emEnergyInEE();
    ++njets_;
  }

  hade_=eme_=0;
  for(CaloTowerCollection::const_iterator it=twrs_h->begin(); it!=twrs_h->end(); ++it) {
    const CaloTower &twr=(*it);
    
    if(abs(twr.ieta())<=29) {
      hade_+=twr.hadEnergy();
      eme_+=twr.emEnergy();
    }
  }
  met_ = calomet.pt();
  trackenergy_=summary.eventTrackEnergy();

  status_=0;
  if(summary.minE2Over10TS()<0.70)  status_ |= 0x1;
  if(summary.maxE2Over10TS()>0.91)  status_ |= 0x1;
  if(summary.maxRBXHits()>=17)      status_ |= 0x2;
  if(summary.maxZeros()>=9)         status_ |= 0x4;
  if(summary.min25GeVHitTime()<-7.) status_ |= 0x8;
  if(summary.max25GeVHitTime()>6.)  status_ |= 0x8;
  
  tree_->Fill();

  // count stuff
  hEventCount_->Fill(0.0);
  int lumiSection = iEvent.luminosityBlock();
  lumiCountMap_[lumiSection]++;

  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
DataEfficiency::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hEventCount_ = new TH1D("hEventCount","Event Count", 1, -0.5, 0.5);
  hLumiBlockCount_ = new TH1D("hLumiBlockCount","Lumi Block Count", 1, -0.5, 0.5);

  tree_ = new TTree("tree","tree");
  tree_->Branch("nrbxs",&nrbxs_, "nrbxs/I");
  tree_->Branch("rbxnhits",rbxnhits_,"rbxnhits[nrbxs]/I");
  tree_->Branch("rbxnhpds",rbxnhpds_,"rbxnhpds[nrbxs]/I");
  tree_->Branch("rbxnhitshpd0",rbxnhitshpd0_,"rbxnhitshpd0[nrbxs]/I");
  tree_->Branch("rbxnhitshpd1",rbxnhitshpd1_,"rbxnhitshpd1[nrbxs]/I");
  tree_->Branch("rbxnhitshpd2",rbxnhitshpd2_,"rbxnhitshpd2[nrbxs]/I");
  tree_->Branch("rbxnhitshpd3",rbxnhitshpd3_,"rbxnhitshpd3[nrbxs]/I");
  tree_->Branch("rbxidnum",rbxidnum_,"rbxidnum[nrbxs]/I");
  tree_->Branch("rbxnadc0s",rbxnadc0s_,"rbxnadc0s[nrbxs]/I");
  tree_->Branch("rbxenergy",rbxenergy_,"rbxenergy[nrbxs]/F");
  tree_->Branch("rbxcalohade",rbxcalohade_,"rbxcalohade[nrbxs]/F");
  tree_->Branch("rbxcaloeme",rbxcaloeme_,"rbxcaloeme[nrbxs]/F");
  tree_->Branch("rbxratio",rbxratio_,"rbxratio[nrbxs]/F");
  tree_->Branch("rbx0ts",rbx0ts_,"rbx0ts[nrbxs]/F");
  tree_->Branch("rbx1ts",rbx1ts_,"rbx1ts[nrbxs]/F");
  tree_->Branch("rbx2ts",rbx2ts_,"rbx2ts[nrbxs]/F");
  tree_->Branch("rbx3ts",rbx3ts_,"rbx3ts[nrbxs]/F");
  tree_->Branch("rbx4ts",rbx4ts_,"rbx4ts[nrbxs]/F");
  tree_->Branch("rbx5ts",rbx5ts_,"rbx5ts[nrbxs]/F");
  tree_->Branch("rbx6ts",rbx6ts_,"rbx6ts[nrbxs]/F");
  tree_->Branch("rbx7ts",rbx7ts_,"rbx7ts[nrbxs]/F");
  tree_->Branch("rbx8ts",rbx8ts_,"rbx8ts[nrbxs]/F");
  tree_->Branch("rbx9ts",rbx9ts_,"rbx9ts[nrbxs]/F");
  tree_->Branch("nhits",&nhits_, "nhits/I");
  tree_->Branch("hitieta",hitieta_, "hitieta[nhits]/I");
  tree_->Branch("hitiphi",hitiphi_, "hitiphi[nhits]/I");
  tree_->Branch("hitenergy",hitenergy_, "hitenergy[nhits]/F");
  tree_->Branch("hittime",hittime_, "hittime[nhits]/F");
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
  tree_->Branch("jetHBhade",jetHBhade_,"jetHBhade[njets]/F");
  tree_->Branch("jetHEhade",jetHEhade_,"jetHEhade[njets]/F");
  tree_->Branch("jetEBeme",jetEBeme_,"jetEBeme[njets]/F");
  tree_->Branch("jetEEeme",jetEEeme_,"jetEEeme[njets]/F");
  tree_->Branch("hade",&hade_, "hade/F");
  tree_->Branch("eme",&eme_, "eme/F");
  tree_->Branch("met",&met_, "met/F");
  tree_->Branch("trackenergy",&trackenergy_, "trackenergy/F");
  tree_->Branch("status",&status_, "status/I");
  tree_->Branch("trigger",&trigger_, "trigger/I");
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DataEfficiency::endJob() {

  // write histograms
  rootfile_->cd();

  hEventCount_->Write();
  hLumiBlockCount_->Fill(0.0, lumiCountMap_.size());
  hLumiBlockCount_->Write();
  tree_->Write();

  rootfile_->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(DataEfficiency);
