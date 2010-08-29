//
// CalcRespCorr.cc
//
//   description: Calculation of dijet response corrections
//
//   author: J.P. Chou, Brown
//
//

#include "HcalClosureTest/Analyzers/interface/CalcRespCorrDiJets.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TClonesArray.h"

#include <vector>
#include <set>

//
// CalcrespCorrDiJets
//

CalcRespCorrDiJets::CalcRespCorrDiJets(const edm::ParameterSet& iConfig)
{
  // set parameters
  jetCollName_       = iConfig.getParameter<std::string>("jetCollName");
  jetCorrName_       = iConfig.getParameter<std::string>("jetCorrName");
  genJetCollName_    = iConfig.getParameter<std::string>("genJetCollName");
  rootHistFilename_  = iConfig.getParameter<std::string>("rootHistFilename");
  maxDeltaEta_       = iConfig.getParameter<double>("maxDeltaEta");
  minTagJetEta_      = iConfig.getParameter<double>("minTagJetEta");
  maxTagJetEta_      = iConfig.getParameter<double>("maxTagJetEta");
  minSumJetEt_       = iConfig.getParameter<double>("minSumJetEt");
  minJetEt_          = iConfig.getParameter<double>("minJetEt");
  maxThirdJetEt_     = iConfig.getParameter<double>("maxThirdJetEt");
  maxJetEMF_         = iConfig.getParameter<double>("maxJetEMF");
  debug_             = iConfig.getUntrackedParameter<bool>("debug", false);
}

CalcRespCorrDiJets::~CalcRespCorrDiJets()
{
}
  
  
//
// member functions
//
  
// ------------ method called to for each event  ------------
void
CalcRespCorrDiJets::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup)
{ 
  edm::Handle<reco::CaloJetCollection> calojets;
  iEvent.getByLabel(jetCollName_,calojets);
  if(!calojets.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloJetCollection named " << jetCollName_ << ".\n";
    return;
  }

  edm::Handle<reco::GenJetCollection> genjets;
  iEvent.getByLabel(genJetCollName_,genjets);
  if(!genjets.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find GenJetCollection named " << genJetCollName_ << ".\n";
    return;
  }

  const JetCorrector* corrector = JetCorrector::getJetCorrector(jetCorrName_,evSetup);


  //////////////////////////////
  // Event Selection
  //////////////////////////////

  // determine which cut results in failure
  int passSel=0;

  // sort jets by corrected et
  std::set<JetCorretPair, JetCorretPairComp> jetcorretpairset;
  for(reco::CaloJetCollection::const_iterator it=calojets->begin(); it!=calojets->end(); ++it) {
    const reco::CaloJet* jet=&(*it);
    jetcorretpairset.insert( JetCorretPair(jet, corrector->correction(jet->p4())) );
  }

  // find highest two (corrected) et jets
  JetCorretPair tag, probe;
  thirdjet_px_=thirdjet_py_=0.0;
  int cntr=0;
  for(std::set<JetCorretPair, JetCorretPairComp>::const_iterator it=jetcorretpairset.begin(); it!=jetcorretpairset.end(); ++it) {
    JetCorretPair jet=(*it);
    ++cntr;
    if(cntr==1) tag=jet;
    else if(cntr==2) probe=jet;
    else {
      thirdjet_px_ += jet.scale()*jet.jet()->px();
      thirdjet_py_ += jet.scale()*jet.jet()->py();
    }
  }
  if(!tag.jet() || !probe.jet()) return;

  // require that the first two jets are above some minimum,
  // and the rest are below some maximum
  if((tag.jet()->et()+probe.jet()->et())<minSumJetEt_)         passSel |= 0x1;
  if(tag.jet()->et()<minJetEt_ || probe.jet()->et()<minJetEt_) passSel |= 0x2;
  if(sqrt(thirdjet_px_*thirdjet_px_ + thirdjet_py_*thirdjet_py_)>maxThirdJetEt_) passSel |= 0x4;

  // force the tag jet to have the smaller |eta|
  if(std::fabs(tag.jet()->eta())>std::fabs(probe.jet()->eta())) {
    JetCorretPair temp=tag;
    tag=probe;
    probe=temp;
  }

  // eta cuts
  double dAbsEta=std::fabs(std::fabs(tag.jet()->eta())-std::fabs(probe.jet()->eta()));
  if(dAbsEta>maxDeltaEta_) passSel |= 0x8;
  if(fabs(tag.jet()->eta())<minTagJetEta_) passSel |= 0x10;
  if(fabs(tag.jet()->eta())>maxTagJetEta_) passSel |= 0x10;

  // emf cuts
  if(tag.jet()->emEnergyFraction()>maxJetEMF_) passSel |= 0x20;
  if(probe.jet()->emEnergyFraction()>maxJetEMF_) passSel |= 0x20;

  // make the cuts
  hPassSel_->Fill(passSel);
  if(passSel) return;

  // dump
  if(debug_) {
    std::cout << "Run: " << iEvent.id().run() << "; Event: " << iEvent.id().event() << std::endl;
    for(reco::CaloJetCollection::const_iterator it=calojets->begin(); it!=calojets->end(); ++it) {
      const reco::CaloJet *jet=&(*it);
      std::cout << "istag=" << (jet==tag.jet()) << "; isprobe=" << (jet==probe.jet()) << "; et=" << jet->et() << "; eta=" << jet->eta() << std::endl;
    }
  }

  // fill tag jet variables
  tjet_pt_    = tag.jet()->pt();
  tjet_p_     = tag.jet()->p();
  tjet_eta_   = tag.jet()->eta();
  tjet_phi_   = tag.jet()->phi();
  tjet_emf_   = tag.jet()->emEnergyFraction();
  tjet_scale_ = tag.scale();
  tjet_EBE_   = tag.jet()->emEnergyInEB();
  tjet_EEE_   = tag.jet()->emEnergyInEE();
  tjet_HBE_   = tag.jet()->hadEnergyInHB();
  tjet_HEE_   = tag.jet()->hadEnergyInHE();
  tjet_HFE_   = tag.jet()->emEnergyInHF() + tag.jet()->hadEnergyInHF();
  tjet_ntwrs_=0;
  std::vector<CaloTowerPtr> tagconst=tag.jet()->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator it=tagconst.begin(); it!=tagconst.end(); ++it) {
    int ieta=(*it)->id().ieta();
    int ietaAbs=(*it)->id().ietaAbs();
    tjet_twr_ieta_[tjet_ntwrs_]=ieta;
    if(ietaAbs<=29) {
      tjet_twr_eme_[tjet_ntwrs_] = (*it)->emEnergy();
      tjet_twr_hade_[tjet_ntwrs_] = (*it)->hadEnergy();
    } else {
      tjet_twr_eme_[tjet_ntwrs_] = 0;
      tjet_twr_hade_[tjet_ntwrs_] = (*it)->emEnergy()+(*it)->hadEnergy();
    }
    ++tjet_ntwrs_;
  }

  // fill probe jet variables
  pjet_pt_    = probe.jet()->pt();
  pjet_p_     = probe.jet()->p();
  pjet_eta_   = probe.jet()->eta();
  pjet_phi_   = probe.jet()->phi();
  pjet_emf_   = probe.jet()->emEnergyFraction();
  pjet_scale_ = probe.scale();
  pjet_EBE_   = probe.jet()->emEnergyInEB();
  pjet_EEE_   = probe.jet()->emEnergyInEE();
  pjet_HBE_   = probe.jet()->hadEnergyInHB();
  pjet_HEE_   = probe.jet()->hadEnergyInHE();
  pjet_HFE_   = probe.jet()->emEnergyInHF() + probe.jet()->hadEnergyInHF();
  pjet_ntwrs_=0;
  std::vector<CaloTowerPtr> probeconst=probe.jet()->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator it=probeconst.begin(); it!=probeconst.end(); ++it) {
    int ieta=(*it)->id().ieta();
    int ietaAbs=(*it)->id().ietaAbs();
    pjet_twr_ieta_[pjet_ntwrs_]=ieta;
    if(ietaAbs<=29) {
      pjet_twr_eme_[pjet_ntwrs_] = (*it)->emEnergy();
      pjet_twr_hade_[pjet_ntwrs_] = (*it)->hadEnergy();
    } else {
      pjet_twr_eme_[pjet_ntwrs_] = 0;
      pjet_twr_hade_[pjet_ntwrs_] = (*it)->emEnergy()+(*it)->hadEnergy();
    }
    ++pjet_ntwrs_;
  }

  // fill genjet tag/probe variables
  tjet_gendr_ = 99999.;
  tjet_genpt_ = 0;
  tjet_genp_  = 0;
  pjet_gendr_ = 99999.;
  pjet_genpt_ = 0;
  pjet_genp_  = 0;
  for(reco::GenJetCollection::const_iterator it=genjets->begin(); it!=genjets->end(); ++it) {
    const reco::GenJet* jet=&(*it);
    double dr=deltaR(jet, probe.jet());
    if(dr<pjet_gendr_) {
      pjet_gendr_ = dr;
      pjet_genpt_ = jet->pt();
      pjet_genp_ = jet->p();
    }
    dr=deltaR(jet, tag.jet());
    if(dr<tjet_gendr_) {
      tjet_gendr_ = dr;
      tjet_genpt_ = jet->pt();
      tjet_genp_ = jet->p();
    }
  }

  // fill dijet variables
  dijet_deta_=std::fabs(std::fabs(tag.jet()->eta())-std::fabs(probe.jet()->eta()));
  dijet_dphi_=tag.jet()->phi()-probe.jet()->phi();
  if(dijet_dphi_>3.1415) dijet_dphi_ = 6.2832-dijet_dphi_;
  dijet_balance_ = (tjet_pt_-pjet_pt_)/(tjet_pt_+pjet_pt_);

  tree_->Fill();
  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorrDiJets::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hPassSel_ = new TH1D("hPassSelection", "Selection Pass Failures",200,-0.5,199.5);

  tree_ = new TTree("dijettree", "tree for dijet balancing");

  tree_->Branch("tjet_pt",&tjet_pt_, "tjet_pt/F");
  tree_->Branch("tjet_p",&tjet_p_, "tjet_p/F");
  tree_->Branch("tjet_eta",&tjet_eta_, "tjet_eta/F");
  tree_->Branch("tjet_phi",&tjet_phi_, "tjet_phi/F");
  tree_->Branch("tjet_emf",&tjet_emf_, "tjet_emf/F");
  tree_->Branch("tjet_scale",&tjet_scale_, "tjet_scale/F");
  tree_->Branch("tjet_genpt",&tjet_genpt_, "tjet_genpt/F");
  tree_->Branch("tjet_genp",&tjet_genp_, "tjet_genp/F");
  tree_->Branch("tjet_gendr",&tjet_gendr_, "tjet_gendr/F");
  tree_->Branch("tjet_EBE",&tjet_EBE_, "tjet_EBE/F");
  tree_->Branch("tjet_EEE",&tjet_EEE_, "tjet_EEE/F");
  tree_->Branch("tjet_HBE",&tjet_HBE_, "tjet_HBE/F");
  tree_->Branch("tjet_HEE",&tjet_HEE_, "tjet_HEE/F");
  tree_->Branch("tjet_HFE",&tjet_HFE_, "tjet_HFE/F");
  tree_->Branch("tjet_ntwrs",&tjet_ntwrs_, "tjet_ntwrs/I");
  tree_->Branch("tjet_twr_ieta",tjet_twr_ieta_, "tjet_twr_ieta[tjet_ntwrs]/I");
  tree_->Branch("tjet_twr_eme",tjet_twr_eme_, "tjet_twr_eme[tjet_ntwrs]/F");
  tree_->Branch("tjet_twr_hade",tjet_twr_hade_, "tjet_twr_hade[tjet_ntwrs]/F");
  tree_->Branch("pjet_pt",&pjet_pt_, "pjet_pt/F");
  tree_->Branch("pjet_p",&pjet_p_, "pjet_p/F");
  tree_->Branch("pjet_eta",&pjet_eta_, "pjet_eta/F");
  tree_->Branch("pjet_phi",&pjet_phi_, "pjet_phi/F");
  tree_->Branch("pjet_emf",&pjet_emf_, "pjet_emf/F");
  tree_->Branch("pjet_scale",&pjet_scale_, "pjet_scale/F");
  tree_->Branch("pjet_genpt",&pjet_genpt_, "pjet_genpt/F");
  tree_->Branch("pjet_genp",&pjet_genp_, "pjet_genp/F");
  tree_->Branch("pjet_gendr",&pjet_gendr_, "pjet_gendr/F");
  tree_->Branch("pjet_EBE",&pjet_EBE_, "pjet_EBE/F");
  tree_->Branch("pjet_EEE",&pjet_EEE_, "pjet_EEE/F");
  tree_->Branch("pjet_HBE",&pjet_HBE_, "pjet_HBE/F");
  tree_->Branch("pjet_HEE",&pjet_HEE_, "pjet_HEE/F");
  tree_->Branch("pjet_HFE",&pjet_HFE_, "pjet_HFE/F");
  tree_->Branch("pjet_ntwrs",&pjet_ntwrs_, "pjet_ntwrs/I");
  tree_->Branch("pjet_twr_ieta",pjet_twr_ieta_, "pjet_twr_ieta[pjet_ntwrs]/I");
  tree_->Branch("pjet_twr_eme",pjet_twr_eme_, "pjet_twr_eme[pjet_ntwrs]/F");
  tree_->Branch("pjet_twr_hade",pjet_twr_hade_, "pjet_twr_hade[pjet_ntwrs]/F");
  tree_->Branch("dijet_deta",&dijet_deta_, "dijet_deta/F");
  tree_->Branch("dijet_dphi",&dijet_dphi_, "dijet_dphi/F");
  tree_->Branch("dijet_balance",&dijet_balance_, "dijet_balance/F");
  tree_->Branch("thirdjet_px",&thirdjet_px_, "thirdjet_px/F");
  tree_->Branch("thirdjet_py",&thirdjet_py_, "thirdjet_py/F");
  
  return;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();
  hPassSel_->Write();
  tree_->Write();
  rootfile_->Close();
}

// helper function

double CalcRespCorrDiJets::deltaR(const reco::Jet* j1, const reco::Jet* j2)
{
  double deta = j1->eta()-j2->eta();
  double dphi = std::fabs(j1->phi()-j2->phi());
  if(dphi>3.1415927) dphi = 2*3.1415927 - dphi;
  return std::sqrt(deta*deta + dphi*dphi);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
