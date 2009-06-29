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
#include "FWCore/Utilities/interface/EDMException.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include <vector>

//
// constructors and destructor
//

CalcRespCorrDiJets::CalcRespCorrDiJets(const edm::ParameterSet& iConfig)
{
  // set parameters
  jetCollName_       = iConfig.getParameter<std::string>("jetCollName");
  rootHistFilename_  = iConfig.getParameter<std::string>("rootHistFilename");
  maxDeltaEta_       = iConfig.getParameter<double>("maxDeltaEta");
  minDeltaPhi_       = iConfig.getParameter<double>("minDeltaPhi");
  maxModifiedEMF_    = iConfig.getParameter<double>("maxModifiedEMF");
  minJetEt_          = iConfig.getParameter<double>("minJetEt");
  maxThirdJetEt_     = iConfig.getParameter<double>("maxThirdJetEt");
  maxCalibratedIEta_ = iConfig.getParameter<int>("maxCalibratedIEta");
  respCorr_          = iConfig.getParameter<std::vector<double> >("respCorr");

  if(respCorr_.size()!=83) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " respCorr has " << respCorr_.size() << " elements.  We want 83.";
  }

}
  
  
CalcRespCorrDiJets::~CalcRespCorrDiJets()
{
}
  
  
//
// member functions
//
  
// ------------ method called to for each event  ------------
void
CalcRespCorrDiJets::analyze(const edm::Event& iEvent, const edm::EventSetup&)
{ 
  edm::Handle<reco::CaloJetCollection> handle;
  iEvent.getByLabel(jetCollName_,handle);
  if(!handle.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find CaloJetCollection named " << jetCollName_ << ".\n";
    return;
  }

  //////////////////////////////
  // Event Selection
  //////////////////////////////

  // determine which cut results in failure
  int passSel=0;

  // find highest two et jets
  const reco::CaloJet* tag=0;
  const reco::CaloJet* probe=0;
  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    const reco::CaloJet* jet=&(*it);
    if(!tag)                       tag=jet;
    else if(!probe)                probe=jet;
    else if(tag->et()<jet->et())   tag=jet;
    else if(probe->et()<jet->et()) probe=jet;
  }
  if(!tag || !probe) return;

  // require that the first two jets are above some minimum,
  // and the rest are below some maximum
  if(tag->et()<minJetEt_)   passSel |= 0x1;
  if(probe->et()<minJetEt_) passSel |= 0x2;

  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    const reco::CaloJet *jet=&(*it);
    if(jet!=tag && jet!=probe && jet->et()>maxThirdJetEt_) passSel |= 0x4;
  }

  // find out if the tag/probe are in the calibration regions
  bool tagincalib=true;
  bool probeincalib=true;
  std::vector<CaloTowerPtr> tagconst = tag->getCaloConstituents();
  std::vector<CaloTowerPtr> probeconst = probe->getCaloConstituents();

  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=tagconst.begin();
      ctp_it!=tagconst.end(); ++ctp_it) {
    if((*ctp_it)->id().ietaAbs()>maxCalibratedIEta_) tagincalib=false;
  }
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin();
      ctp_it!=probeconst.end(); ++ctp_it) {
    const CaloTower* twr=&(**ctp_it);
    if((*ctp_it)->id().ietaAbs()>maxCalibratedIEta_) probeincalib=false;
  }  // if the probe is in the calibration region, and the tag is not, swap them
  if(probeincalib && !tagincalib) {
    const reco::CaloJet* temp=tag;
    tag=probe;
    probe=temp;
    probeincalib=false;
    tagincalib=true;
  }
  // require that the tag is in the calib region and the probe is not
  if(!(tagincalib && !probeincalib)) passSel |= 0x08;

  // require that the delta-eta is small, and the delta-phi is large
  if(std::fabs(tag->eta()-probe->eta())>maxDeltaEta_) passSel |= 0x10;
  double dphi = std::fabs(tag->phi()-probe->phi());
  if(dphi>3.1416) dphi = 2.0*3.14156-dphi;
  if(dphi<minDeltaPhi_) passSel |= 0x20;

  // require tha the probe modified emf is small
  // modified means HF is all hadronic and we don't count HO
  double peme = probe->emEnergyInEB()+probe->emEnergyInEE();
  double phade = probe->hadEnergyInHB()+probe->hadEnergyInHE()+probe->emEnergyInHF()+probe->hadEnergyInHF();
  double pemf = phade+peme ? peme/(phade+peme) : 999.;
  double teme = tag->emEnergyInEB()+tag->emEnergyInEE();
  double thade = tag->hadEnergyInHB()+tag->hadEnergyInHE()+tag->emEnergyInHF()+tag->hadEnergyInHF();
  //  double temf = thade+teme ? teme/(thade+teme) : 999.;
  if(pemf>maxModifiedEMF_) passSel |= 0x40;

  // make the cuts
  hPassSel_->Fill(passSel);
  if(passSel) return;
  
  //////////////////////////////
  // calculate response corrs
  //////////////////////////////
  
  double respcorr=(teme+thade-peme)/phade;
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin();
      ctp_it!=probeconst.end(); ++ctp_it) {
    const CaloTower* twr=&(**ctp_it);
    hRespIeta_->Fill(respcorr, twr->id().ieta());
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorrDiJets::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hPassSel_ = new TH1D("hPassSelection", "Selection Pass Failures",200,-0.5,199.5);
  hRespIeta_ = new TH2D("hRespIeta","Response Corrections versus ieta",100,0,5,59,-29.5,29.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();

  hPassSel_->Write();
  hRespIeta_->Write();
  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
