#ifndef _ANOMALOUSNOISE_ANALYZERS_NOISERATES_H_
#define _ANOMALOUSNOISE_ANALYZERS_NOISERATES_H_


//
// NoiseRates.h
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

#include "Geometry/CaloTopology/interface/HcalTopology.h"

// forward declarations
class TH1D;
class TH2D;
class TFile;

//
// class declaration
//

class NoiseRates : public edm::EDAnalyzer {
 public:
  explicit NoiseRates(const edm::ParameterSet&);
  ~NoiseRates();
  
  
 private:
  virtual void beginJob(const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  // parameters
  std::string rbxCollName_;          // label for the rbx collection
  std::string metCollName_;          // label for the MET collection
  std::string jetCollName_;          // label for the Calo jet collection
  std::string hbheRecHitCollName_;   // label for the HBHE RecHit collection
  std::string caloTowerCollName_;    // label for the caloTower collection
  std::string rootHistFilename_;     // name of the histogram file
  bool findTrigger_;                 // whether or not to look for a L1 trigger
  std::string trigName_;             // trigger name
  double minRBXEnergy_;              // RBX energy threshold
  double minHitEnergy_;              // RecHit energy threshold

  // geometry
  HcalTopology theTopology;

  // root file/histograms
  TFile* rootfile_;  
  TH1D* hLumiBlockCount_;
  TH1D* hRBXEnergy_;
  TH1D* hRBXEnergyType1_;
  TH1D* hRBXEnergyType2_;
  TH1D* hRBXEnergyType3_;
  TH1D* hRBXEnergyAfterCut1_;
  TH1D* hRBXEnergyAfterCut2_;
  TH1D* hRBXEnergyAfterCut3_;
  TH1D* hRBXNHits_;

  TH1D* hMET_;
  TH1D* hMETAfterCut1_;
  TH1D* hMETAfterCut2_;
  TH1D* hMETAfterCut3_;
  TH1D* hMETAfterCut4_;

  TH1D* hJetEt_;
  TH1D* hJetEtAfterCut1_;
  TH1D* hJetEtAfterCut2_;
  TH1D* hJetEtAfterCut3_;
  TH1D* hJetEtAfterCut4_;

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

  TH1D* hTrigTowersEnergy_;
  TH1D* hTrigTowersEnergyType1_;
  TH1D* hTrigTowersEnergyType2_;
  TH1D* hTrigTowersEnergyType3_;

  // count lumi segments
  std::map<int, int> lumiCountMap_;

  // trigger tower list
  std::set<std::pair<int, int> > triggerlist_;

};


#endif
