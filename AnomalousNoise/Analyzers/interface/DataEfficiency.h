#ifndef _ANOMALOUSNOISE_ANALYZERS_DATAEFFICIENCY_H_
#define _ANOMALOUSNOISE_ANALYZERS_DATAEFFICIENCY_H_


//
// DataEfficiency.h
//
//    description: Makes plots to calculate the anomalous noise rates
//
//    author: J.P. Chou, Brown
//
//

// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// forward declarations
class TH1D;
class TFile;
class TTree;

//
// class declaration
//

class DataEfficiency : public edm::EDAnalyzer {
 public:
  explicit DataEfficiency(const edm::ParameterSet&);
  ~DataEfficiency();
  
  
 private:
  virtual void beginJob(const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  // parameters
  std::string rbxCollName_;          // label for the rbx collection
  std::string metCollName_;          // label for the MET collection
  std::string jetCollName_;          // label for the Calo jet collection
  std::string trackCollName_;        // label for tracks
  std::string hbheRecHitCollName_;   // label for the HBHE RecHit collection
  std::string caloTowerCollName_;    // label for the caloTower collection
  std::string rootHistFilename_;     // name of the histogram file

  // root file/histograms
  TFile* rootfile_;  
  TH1D* hMin10GeVHitTime_;
  TH1D* hMax10GeVHitTime_;
  TH1D* hMin25GeVHitTime_;
  TH1D* hMax25GeVHitTime_;
  TH1D* hMinE2Over10TS_;
  TH1D* hMaxZeros_;
  TH1D* hMaxHPDHits_;
  TH1D* hMaxRBXHits_;
  TH1D* hMinHPDEMF_;
  TH1D* hMinRBXEMF_;

  TH1D* hMET_;
  TH1D* hMETLoose_;
  TH1D* hMETTight_;
  TH1D* hMETTrigger_;
  TH1D* hNJets_;
  TH1D* hNJetsLoose_;
  TH1D* hNJetsTight_;
  TH1D* hNJetsTrigger_;
  TH1D* hJetPt_;
  TH1D* hJetPtLoose_;
  TH1D* hJetPtTight_;
  TH1D* hJetPtTrigger_;
  TH1D* hJetEta_;
  TH1D* hJetEtaLoose_;
  TH1D* hJetEtaTight_;
  TH1D* hJetEtaTrigger_;
  TH1D* hCHF_;
  TH1D* hCHE_;
  TH1D* hCalE_;
  
  // tree
  TTree* tree_;
  int ntracks_;
  float trkpt_[100], trkp_[100], trketa_[100], trkphi_[100];
  float sumtrkp_;
  int njets_;
  float jetpt_[100], jeteta_[100], jetphi_[100];
  float hade_, eme_;

};


#endif
