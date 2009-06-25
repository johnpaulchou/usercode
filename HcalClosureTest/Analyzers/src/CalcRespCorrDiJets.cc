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
  bool passSel=true;
  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    reco::CaloJet jet=(*it);
    ++cntr;
    if((cntr==1 || cntr==2) && jet.et()<minJetEt_) passSel=false;
    if(cntr>=2 && jet.et()>maxThirdJetEt_) passSel=false;
  }
  if(!passSel) return;

  //////////////////////////////
  // make jet pairs
  //////////////////////////////
  const reco::CaloJet* tag=&(*(handle->begin()+0));
  const reco::CaloJet* probe=&(*(handle->begin()+1));

  // require that the tag jet is completely in the calibrated region
  //...
  
  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorrDiJets::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorrDiJets::endJob() {

  // write histograms
  rootfile_->cd();

  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorrDiJets);
