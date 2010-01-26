#ifndef _HCALCLOSURETEST_ANALYZERS_CALCRESPCORRDIJETS_H_
#define _HCALCLOSURETEST_ANALYZERS_CALCRESPCORRDIJETS_H_

//
// CalcRespCorrDiJets.h
//
//    description: Makes plots to calculate the response correction using dijets.
//
//    author: J.P. Chou, Brown
//
//

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"

// forward declarations
class TH1D;
class TH2D;
class TFile;
class TTree;

//
// class declaration
//

class CalcRespCorrDiJets : public edm::EDAnalyzer {
 public:
  explicit CalcRespCorrDiJets(const edm::ParameterSet&);
  ~CalcRespCorrDiJets();
  
  
 private:
  virtual void beginJob(const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  
  // parameters
  bool debug_;                   // print debug statements
  std::string jetCollName_;      // label for the jet collection
  std::string jetCorrName_;      // label for the jet correction service
  std::string genJetCollName_;   // label for the genjet collection
  std::string rootHistFilename_; // name of the histogram file
  double maxDeltaEta_;           // maximum delta-|Eta| between Jets
  double minTagJetEta_;          // minimum |eta| of the tag jet
  double maxTagJetEta_;          // maximum |eta| of the tag jet
  double minSumJetEt_;           // minimum Sum of the tag and probe jet Et
  double minJetEt_;              // minimum Jet Et
  double maxThirdJetEt_;         // maximum 3rd jet Et
  double maxJetEMF_;             // maximum EMF of the tag and probe jets

  // root file/histograms
  TFile* rootfile_;

  TH1D* hPassSel_;
  TTree* tree_;
  float tjet_pt_, tjet_p_, tjet_eta_, tjet_phi_, tjet_emf_, tjet_scale_;
  float tjet_gendr_, tjet_genpt_, tjet_genp_;
  float tjet_EBE_, tjet_EEE_, tjet_HBE_, tjet_HEE_, tjet_HFE_;
  int tjet_ntwrs_;
  int tjet_twr_ieta_[100];
  float tjet_twr_eme_[100], tjet_twr_hade_[100];
  float pjet_pt_, pjet_p_, pjet_eta_, pjet_phi_, pjet_emf_, pjet_scale_;
  float pjet_gendr_, pjet_genpt_, pjet_genp_;
  float pjet_EBE_, pjet_EEE_, pjet_HBE_, pjet_HEE_, pjet_HFE_;
  int pjet_ntwrs_;
  int pjet_twr_ieta_[100];
  float pjet_twr_eme_[100], pjet_twr_hade_[100];
  float dijet_deta_, dijet_dphi_, dijet_balance_;
  float thirdjet_px_, thirdjet_py_;

  // helper functions
  double deltaR(const reco::Jet* j1, const reco::Jet* j2);

};


#endif
