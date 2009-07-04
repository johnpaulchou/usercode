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
  minTagCalibFrac_   = iConfig.getParameter<double>("minTagCalibFrac");
  maxProbeCalibFrac_ = iConfig.getParameter<double>("maxProbeCalibFrac");
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

  // force the tag jet to have the smaller |eta|
  if(std::fabs(tag->eta())>std::fabs(probe->eta())) {
    const reco::CaloJet* temp=tag;
    tag=probe;
    probe=temp;
  }

  // require that the first two jets are above some minimum,
  // and the rest are below some maximum
  if(tag->et()<minJetEt_)   passSel |= 0x1;
  if(probe->et()<minJetEt_) passSel |= 0x2;

  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    const reco::CaloJet *jet=&(*it);
    if(jet!=tag && jet!=probe && jet->et()>maxThirdJetEt_) passSel |= 0x4;
  }

  // find out if the tag/probe are in the calibration regions
  double tagcalib=0, tagncalib=0;
  double probecalib=0, probencalib=0;
  std::vector<CaloTowerPtr> tagconst = tag->getCaloConstituents();
  std::vector<CaloTowerPtr> probeconst = probe->getCaloConstituents();

  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=tagconst.begin();
      ctp_it!=tagconst.end(); ++ctp_it) {
    double et=(*ctp_it)->et();
    double abseta=(*ctp_it)->id().ietaAbs();
    if(abseta<=maxCalibratedIEta_) tagcalib  += et;
    else                           tagncalib += et;
  }
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin();
      ctp_it!=probeconst.end(); ++ctp_it) {
    double et=(*ctp_it)->et();
    double abseta=(*ctp_it)->id().ietaAbs();
    if(abseta<=maxCalibratedIEta_) probecalib  += et;
    else                           probencalib += et;
  }

  // require that the tag is in the calib region and the probe is not
  bool tagincalib = tagcalib/(tagcalib+tagncalib) > minTagCalibFrac_;
  bool probeincalib = probecalib/(probecalib+probencalib) > maxProbeCalibFrac_;
  if(!tagincalib || probeincalib) passSel |= 0x08;

  // require that the delta-|eta| is small, and the delta-phi is large
  double deta=std::fabs(std::fabs(tag->eta())-std::fabs(probe->eta()));
  if(deta>maxDeltaEta_) passSel |= 0x10;
  double dphi = std::fabs(tag->phi()-probe->phi());
  if(dphi>3.1416) dphi = 2.0*3.14156-dphi;
  if(dphi<minDeltaPhi_) passSel |= 0x20;

  // require tha the probe modified emf is small
  // modified means HF is all hadronic and we don't count HO
  double peme = probe->emEnergyInEB()+probe->emEnergyInEE();
  double phade = probe->hadEnergyInHB()+probe->hadEnergyInHE()+probe->emEnergyInHF()+probe->hadEnergyInHF();
  double pemf = phade+peme ? peme/(phade+peme) : 999.;
  if(pemf>maxModifiedEMF_) passSel |= 0x40;

  // make the cuts
  hPassSel_->Fill(passSel);
  if(passSel) return;
  
  //////////////////////////////
  // calculate response corrs
  //////////////////////////////
  
  double respcorr=2*(tag->et()-probe->et())/(tag->et()+probe->et());
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin();
      ctp_it!=probeconst.end(); ++ctp_it) {
    const CaloTower* twr=&(**ctp_it);

    if(deta<0.5) hRespIeta_->Fill(respcorr, twr->id().ieta());

    if(tag->et()<50 && probe->et()<50) {
      if(deta<0.5) hRespIetaLowEtLowDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.0) hRespIetaLowEtMidDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.5) hRespIetaLowEtTopDEta_->Fill(respcorr, twr->id().ieta());
    }
    else if(tag->et()<80 && probe->et()<80) {
      if(deta<0.5) hRespIetaMidEtLowDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.0) hRespIetaMidEtMidDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.5) hRespIetaMidEtTopDEta_->Fill(respcorr, twr->id().ieta());
    }
    else {
      if(deta<0.5) hRespIetaTopEtLowDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.0) hRespIetaTopEtMidDEta_->Fill(respcorr, twr->id().ieta());
      else if(deta<1.5) hRespIetaTopEtTopDEta_->Fill(respcorr, twr->id().ieta());
    }
  }

  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorrDiJets::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hPassSel_ = new TH1D("hPassSelection", "Selection Pass Failures",200,-0.5,199.5);
  hRespIeta_ = new TH2D("hRespIeta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaLowEtLowDEta_ = new TH2D("hRespIetaLowEtLowDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaMidEtLowDEta_ = new TH2D("hRespIetaMidEtLowDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaTopEtLowDEta_ = new TH2D("hRespIetaTopEtLowDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaLowEtMidDEta_ = new TH2D("hRespIetaLowEtMidDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaMidEtMidDEta_ = new TH2D("hRespIetaMidEtMidDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaTopEtMidDEta_ = new TH2D("hRespIetaTopEtMidDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaLowEtTopDEta_ = new TH2D("hRespIetaLowEtTopDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaMidEtTopDEta_ = new TH2D("hRespIetaMidEtTopDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
  hRespIetaTopEtTopDEta_ = new TH2D("hRespIetaTopEtTopDEta","Response Corrections versus ieta",100,-1,1,83,-41.5,41.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();

  hPassSel_->Write();
  hRespIeta_->Write();
  hRespIetaLowEtLowDEta_->Write();
  hRespIetaMidEtLowDEta_->Write();
  hRespIetaTopEtLowDEta_->Write();
  hRespIetaLowEtMidDEta_->Write();
  hRespIetaMidEtMidDEta_->Write();
  hRespIetaTopEtMidDEta_->Write();
  hRespIetaLowEtTopDEta_->Write();
  hRespIetaMidEtTopDEta_->Write();
  hRespIetaTopEtTopDEta_->Write();
  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
