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
  minTagJetEta_      = iConfig.getParameter<double>("minTagJetEta");
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

  // eta and phi cuts
  double dAbsEta=std::fabs(std::fabs(tag->eta())-std::fabs(probe->eta()));
  double dPhi=tag->phi()-probe->phi();
  if(dPhi>3.1415) dPhi = 6.2832-dPhi;
  if(dAbsEta>maxDeltaEta_) passSel |= 0x8;
  if(dPhi<minDeltaPhi_) passSel |= 0x10;
  if(fabs(tag->eta())<minTagJetEta_) passSel |= 0x20;

  // emf cuts
  if(tag->emEnergyFraction()>maxJetEMF_) passSel |= 0x40;
  if(probe->emEnergyFraction()>maxJetEMF_) passSel |= 0x80;

  // make the cuts
  hPassSel_->Fill(passSel);
  if(passSel) return;

  // dump
  if(debug_) {
    std::cout << "Run: " << iEvent.id().run() << "; Event: " << iEvent.id().event() << std::endl;
    for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
      const reco::CaloJet *jet=&(*it);
      std::cout << "istag=" << (jet==tag) << "; isprobe=" << (jet==probe) << "; et=" << jet->et() << "; eta=" << jet->eta() << std::endl;
    }
  }

  // calculate the quantities needed for the calibration
  DijetRespCorrDatum datum;

  datum.SetTagEta(tag->eta());
  datum.SetProbeEta(probe->eta());
  datum.SetTagEcalE(tag->emEnergyInEB()+tag->emEnergyInEE());
  datum.SetProbeEcalE(probe->emEnergyInEB()+probe->emEnergyInEE());

  std::vector<CaloTowerPtr> tagconst=tag->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=tagconst.begin(); ctp_it!=tagconst.end(); ++ctp_it) {
    int ieta=(*ctp_it)->id().ieta();
    int ietaAbs=(*ctp_it)->id().ietaAbs();
    double hade=(*ctp_it)->hadEnergy();
    if(ietaAbs>29) hade += (*ctp_it)->emEnergy();
    if(hade>0) datum.AddTagHcalE(hade, ieta);
  }

  std::vector<CaloTowerPtr> probeconst=probe->getCaloConstituents();
  for(std::vector<CaloTowerPtr>::const_iterator ctp_it=probeconst.begin(); ctp_it!=probeconst.end(); ++ctp_it) {
    int ieta=(*ctp_it)->id().ieta();
    int ietaAbs=(*ctp_it)->id().ietaAbs();
    double hade=(*ctp_it)->hadEnergy();
    if(ietaAbs>29) hade += (*ctp_it)->emEnergy();
    if(hade>0) datum.AddProbeHcalE(hade, ieta);
  }

  if(debug_) {
    std::map<Int_t, Double_t> taghcal, probehcal;
    datum.GetTagHcalE(taghcal);
    datum.GetProbeHcalE(probehcal);
    std::cout << "TagEcalE: " << datum.GetTagEcalE()
	      << "; ProbeEcalE: " << datum.GetProbeEcalE() << std::endl;
    for(std::map<Int_t, Double_t>::const_iterator it=taghcal.begin(); it!=taghcal.end(); ++it)
      std::cout << "Tag Hcal (eta, e): " << it->first << "; " << it->second << std::endl;
    for(std::map<Int_t, Double_t>::const_iterator it=probehcal.begin(); it!=probehcal.end(); ++it)
      std::cout << "Probe Hcal (eta, e): " << it->first << "; " << it->second << std::endl;
  }

  hTagEta_->Fill(tag->eta());
  hProbeEta_->Fill(probe->eta());
  hRatioTagEta_->Fill(tag->et()/probe->et(),tag->eta());

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
  hTagEta_ = new TH1D("hTagEta","Tag Eta",100,-5,5);
  hProbeEta_ = new TH1D("hProbeEta","Probe Eta",100,-5,5);
  hRatioTagEta_ = new TH2D("hRatioTagEta","Ratio",100,-5,5,100,-5,5);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();
  hPassSel_->Write();
  hTagEta_->Write();
  hProbeEta_->Write();
  hRatioTagEta_->Write();
  respcorrdata_.Write("respcorrdata");
  rootfile_->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
