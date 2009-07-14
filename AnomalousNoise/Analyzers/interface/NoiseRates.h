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
  std::string rbxCollName_;            // label for the rbx collection
  std::string rootHistFilename_;       // name of the histogram file
  std::vector<std::string> trigNames_; // trigger names
  double minRBXEnergy_;                // RBX energy threshold
  double minHitEnergy_;                // RecHit energy threshold

  // root file/histograms
  TFile* rootfile_;  
  TH1D* hL1Cntr_;
  TH1D* hLumiBlockCount_;
  TH2D* hRBXEnergy_;
  TH2D* hRBXEnergyType1_;
  TH2D* hRBXEnergyType2_;
  TH2D* hRBXEnergyType3_;
  TH2D* hRBXNHits_;

  // count lumi segments
  std::map<int, int> lumiCountMap_;

};


#endif
