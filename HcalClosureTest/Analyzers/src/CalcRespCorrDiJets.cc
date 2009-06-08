//
// CalcRespCorr.cc
//
//   description: Calculation of dijet response corrections
//
//   author: J.P. Chou, Brown
//
//

#include "HcalClosureTest/Analyzers/interface/CalcRespCorrDiJets.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include <vector>

//
// constructors and destructor
//

CalcRespCorr::CalcRespCorr(const edm::ParameterSet& iConfig)
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
  
  
CalcRespCorr::~CalcRespCorr()
{
}
  
  
//
// member functions
//
  
// ------------ method called to for each event  ------------
void
CalcRespCorr::analyze(const edm::Event& iEvent, const edm::EventSetup&)
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

  if(handle->size()<2) continue; // require at least two jets

  // require that the first two jets are above some minimum,
  // and the rest are below some maximum
  int cntr=0;
  bool passSel=true;
  for(reco::CaloJetCollection::const_iterator it=handle->begin(); it!=handle->end(); ++it) {
    ++cntr;
    if((cntr==1 || cntr==2) && jet->et()<minJetEt_) passSel=false;
    if(cntr>=2 && jet->et()>maxThirdJetEt_) passSel=false;
  }
  if(!passSel) continue;

  //////////////////////////////
  // make jet pairs
  //////////////////////////////
  const reco::CaloJet* tag=&(*(handle->begin()+0));
  const reco::CaloJet* probe=&(*(handle->begin()+1));

  // require that the tag jet is completely in the calibrated region
  
    


    if(modifiedemf > maxModifiedEMF_) continue;
    if(deltar > maxDeltaR_) continue;

    // Fill cluster plots after selection
    hAfterClstrE_->Fill(clstrP4.e());
    hAfterClstrEoverP_->Fill(clstr.EoverP());
    hAfterClstrEta_->Fill(clstrP4.eta());
    hAfterClstrPhi_->Fill(clstrP4.phi());
    hAfterClstrEEta_->Fill(clstrP4.e(), clstrP4.eta());
    hAfterClstrEPhi_->Fill(clstrP4.e(), clstrP4.phi());
    hAfterClstrEoverPEta_->Fill(eoverp, clstrP4.eta());
    hAfterClstrEoverPPhi_->Fill(eoverp, clstrP4.phi());
    
    // Fill response corrections
    edm::RefVector<CaloTowerCollection>::const_iterator highestctit=caloTowerRef.begin();
    for(edm::RefVector<CaloTowerCollection>::const_iterator ctit=caloTowerRef.begin();
	ctit!=caloTowerRef.end(); ++ctit) {
      if((*ctit)->hadEnergy()>(*highestctit)->hadEnergy()) highestctit=ctit;
      hRespIetaAll_->Fill(correction, (*ctit)->id().ieta());
    }
    hRespIetaHighest_->Fill(correction, (*highestctit)->id().ieta());

    // Fill corrected cluster plots after selection
    math::PtEtaPhiMLorentzVector correctedP4;
    for(edm::RefVector<CaloTowerCollection>::const_iterator ctit=caloTowerRef.begin();
	ctit!=caloTowerRef.end(); ++ctit) {
      int ieta = (*ctit)->id().ieta();
      double K = respCorr_[ieta+41];
      double had = (*ctit)->hadEnergy();
      double em = (*ctit)->emEnergy();
      double correction=1;
      if(ieta>29 || ieta<-29) { // in the HF
	correction = K;
      } else {
	correction = (K*had+em)/(had+em);
      }
      correctedP4 += (*ctit)->p4() * correction;
    }
    double corre = correctedP4.e();
    double correoverp = corre/genpRef->p();
    hCorrClstrE_->Fill(corre);
    hCorrClstrEoverP_->Fill(correoverp);
    hCorrClstrEEta_->Fill(corre, correctedP4.eta());
    hCorrClstrEPhi_->Fill(corre, correctedP4.phi());
    hCorrClstrEoverPEta_->Fill(correoverp, correctedP4.eta());
    hCorrClstrEoverPPhi_->Fill(correoverp, correctedP4.phi());

  }   // done looping over clusters
  return;
}


// ------------ method called once each job just before starting event loop  ------------
void 
CalcRespCorr::beginJob(const edm::EventSetup&)
{
  // book histograms
  rootfile_ = new TFile(rootHistFilename_.c_str(), "RECREATE");

  hGenpE_      = new TH1D("hGenpE","Genp Energy",100,0,100);
  hGenpEta_    = new TH1D("hGenpEta","Genp Eta",100,-5,5);
  hGenpPhi_    = new TH1D("hGenpPhi","Genp Phi",100,-TMath::Pi(),TMath::Pi());
  hGenpEtaPhi_ = new TH2D("hGenpEtaPhi","Genp Eta v. Phi",100,-5,5,100,-TMath::Pi(),TMath::Pi());

  hClstrE_           = new TH1D("hClstrE","Cluster Energy",100,0,100);
  hClstrEta_         = new TH1D("hClstrEta","Cluster Eta",100,-5,5);
  hClstrPhi_         = new TH1D("hClstrPhi","Cluster Phi",100,-TMath::Pi(),TMath::Pi());
  hClstrEtaPhi_      = new TH2D("hClstrEtaPhi","Cluster Eta v. Phi",100,-5,5,100,-TMath::Pi(),TMath::Pi());
  hClstrEMF_         = new TH1D("hClstrEMF","Cluster EMF",100,-2,2);
  hClstrModifiedEMF_ = new TH1D("hClstrModifiedEMF","Cluster Modified EMF",100,0,1);
  hClstrDeltaR_      = new TH1D("hClstrDeltaR","Delta-R between cluster and genp",100,0,5);

  hAfterClstrE_         = new TH1D("hAfterClstrE","Cluster Energy After Selection",100,0,100);
  hAfterClstrEoverP_    = new TH1D("hAfterClstrEoverP","Cluster E/P After Selection",100,0,5);
  hAfterClstrEta_       = new TH1D("hAfterClstrEta","Cluster Eta After Selection",100,-5,5);
  hAfterClstrPhi_       = new TH1D("hhAfterClstrPhi","Cluster Phi After Selection",100,-TMath::Pi(),TMath::Pi());
  hAfterClstrEEta_      = new TH2D("hAfterClstrEEta","Cluster Energy v. Eta After Selection",100,0,100,24,-5,5);
  hAfterClstrEPhi_      = new TH2D("hAfterClstrEPhi","Cluster Energy v. Phi After Selection",100,0,100,24,-TMath::Pi(),TMath::Pi());
  hAfterClstrEoverPEta_ = new TH2D("hAfterClstrEoverPEta","Cluster E/P v. Eta After Selection",100,0,5,24,-5,5);
  hAfterClstrEoverPPhi_ = new TH2D("hAfterClstrEoverPPhi","Cluster E/p v. Phi After Selection",100,0,5,24,-TMath::Pi(),TMath::Pi());

  hRespIetaHighest_ = new TH2D("hRespIetaHighest","Highest Energy Tower Response v. ieta",100,0,5,83,-41.5,41.5);
  hRespIetaCentral_ = new TH2D("hRespIetaCentral","Central Tower Response v. ieta",100,0,5,83,-41.5,41.5);
  hRespIetaAll_     = new TH2D("hRespIetaAll","All Towers Response v. ieta",100,0,5,83,-41.5,41.5);
  
  hCorrClstrE_         = new TH1D("hCorrClstrE","Response Corrected Cluster Energy",100,0,100);
  hCorrClstrEoverP_    = new TH1D("hCorrClstrEoverP","Response Corrected Cluster E/P",100,0,5);
  hCorrClstrEEta_      = new TH2D("hCorrClstrEEta","Response Corrected Cluster Energy v. Eta",100,0,100,24,-5,5);
  hCorrClstrEPhi_      = new TH2D("hCorrClsterEPhi","Response Corrected Cluster Energy v. Phi",100,0,100,24,-TMath::Pi(),TMath::Pi());
  hCorrClstrEoverPEta_ = new TH2D("hCorrClstrEoverPEta","Response Corrected Cluster E/P v. Eta",100,0,5,24,-5,5);
  hCorrClstrEoverPPhi_ = new TH2D("hCorrClsterEoverPPhi","Response Corrected Cluster E/P v. Phi",100,0,5,24,-TMath::Pi(),TMath::Pi());
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CalcRespCorr::endJob() {

  // write histograms
  rootfile_->cd();

  hGenpE_->Write();
  hGenpPhi_->Write();
  hGenpEta_->Write();
  hGenpEtaPhi_->Write();

  hClstrE_->Write();
  hClstrEta_->Write();
  hClstrPhi_->Write();
  hClstrEtaPhi_->Write();
  hClstrEMF_->Write();
  hClstrModifiedEMF_->Write();
  hClstrDeltaR_->Write();

  hAfterClstrE_->Write();
  hAfterClstrEoverP_->Write();
  hAfterClstrEta_->Write();
  hAfterClstrPhi_->Write();
  hAfterClstrEEta_->Write();
  hAfterClstrEPhi_->Write();
  hAfterClstrEoverPEta_->Write();
  hAfterClstrEoverPPhi_->Write();

  hRespIetaHighest_->Write();
  hRespIetaCentral_->Write();
  hRespIetaAll_->Write();
  
  hCorrClstrE_->Write();
  hCorrClstrEoverP_->Write();
  hCorrClstrEEta_->Write();
  hCorrClstrEPhi_->Write();
  hCorrClstrEoverPEta_->Write();
  hCorrClstrEoverPPhi_->Write();

  rootfile_->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(CalcRespCorr);
