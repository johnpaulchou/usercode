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

  if(handle->size()<2) return; // require at least two jets

  // require that the first two jets are above some minimum,
  // and the rest are below some maximum
  int cntr=0;
  int passSel=0;
  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    reco::CaloJet jet=(*it);
    ++cntr;
    if((cntr==1 || cntr==2) && jet.et()<minJetEt_) passSel |= 0x1;
    if(cntr>=2 && jet.et()>maxThirdJetEt_) passSel |= 0x2;
  }

  // make jet pairs
  const reco::CaloJet* tag=&(*(handle->begin()+0));
  const reco::CaloJet* probe=&(*(handle->begin()+1));
  std::vector<CaloTowerPtr> tagconst = tag->getCaloConstituents();
  std::vector<CaloTowerPtr> probeconst = probe->getCaloConstituents();

  // require that the tag jet is completely in the calibrated region
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=tagconst.begin();
      ctp_it!=tagconst.end(); ++ctp_it) {
    const CaloTower* twr=&(**ctp_it);
    if(std::abs(twr->id().ieta())>maxCalibratedIEta_) passSel |= 0x4;
  }
  
  // require that the probe jet is _not_ completely in the calibrated region
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin();
      ctp_it!=probeconst.end(); ++ctp_it) {
    const CaloTower* twr=&(**ctp_it);
    if(std::abs(twr->id().ieta())<=maxCalibratedIEta_) passSel |= 0x8;
  }
  
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

  hRespIeta_ = new TH2D("hRespIeta","Response Corrections versus ieta",100,0,5,59,-29.5,29.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();

  hRespIeta_->Write();
  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
