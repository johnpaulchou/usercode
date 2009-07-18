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
#include "TClonesArray.h"

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
  minJetEt_          = iConfig.getParameter<double>("minJetEt");
  maxThirdJetEt_     = iConfig.getParameter<double>("maxThirdJetEt");
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

  // force the tag jet to have the smaller |eta|
  if(std::fabs(tag->eta())>std::fabs(probe->eta())) {
    const reco::CaloJet* temp=tag;
    tag=probe;
    probe=temp;
  }

  // make the cuts
  hPassSel_->Fill(passSel);
  if(passSel) return;


  // calculate the quantities needed for the calibration
  double tagecorr = 1./std::cosh(tag->eta());
  double probeecorr = 1./std::cosh(probe->eta());
  double tagEcalE   = tag->emEnergyInEB() + tag->emEnergyInEE();
  double probeEcalE = probe->emEnergyInEB() + probe->emEnergyInEE();
  double tagHBHEE   = tag->hadEnergyInHB() + tag->hadEnergyInHE();
  double probeHBHEE = probe->hadEnergyInHB() + probe->hadEnergyInHE();
  double tagHFE     = tag->emEnergyInHF() + tag->hadEnergyInHF();
  double probeHFE   = probe->emEnergyInHF() + probe->hadEnergyInHF();

  DijetRespCorrDatum datum;

  datum.SetTagEcalEt(tagecorr*tagEcalE);
  datum.SetProbeEcalEt(probeecorr*probeEcalE);
  datum.SetResolution(sqrt(0.07*0.07*(tagEcalE+probeEcalE) +
			   1.15*1.15*(tagHBHEE+probeHBHEE) +
			   1.35*1.35*(tagHFE+probeHFE)));
  
  std::vector<CaloTowerPtr> tagconst=tag->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=tagconst.begin(); ctp_it!=tagconst.end(); ++ctp_it) {
    int ieta=(*ctp_it)->id().ieta();
    int ietaAbs=(*ctp_it)->id().ietaAbs();
    double hadet=(*ctp_it)->hadEnergy()*tagecorr;
    if(ietaAbs>29) hadet += (*ctp_it)->emEnergy()*tagecorr;
    datum.SetTagHcalEt(hadet, ieta);
  }

  std::vector<CaloTowerPtr> probeconst=probe->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin(); ctp_it!=probeconst.end(); ++ctp_it) {
    int ieta=(*ctp_it)->id().ieta();
    int ietaAbs=(*ctp_it)->id().ietaAbs();
    double hadet=(*ctp_it)->hadEnergy()*tagecorr;
    if(ietaAbs>29) hadet += (*ctp_it)->emEnergy()*tagecorr;
    datum.SetProbeHcalEt(hadet, ieta);
  }

  respcorrdata_.push_back(datum);
  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorrDiJets::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hPassSel_ = new TH1D("hPassSelection", "Selection Pass Failures",200,-0.5,199.5);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();
  hPassSel_->Write();
  respcorrdata_.Write("respcorrdata");
  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
