#ifndef _ANOMALOUSNOISE_ANALYZERS_ISOLATEDNOISE_H_
#define _ANOMALOUSNOISE_ANALYZERS_ISOLATEDNOISE_H_

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

#include "AnomalousNoise/Algorithms/interface/RBXOrganizer.h"

// forward declarations
class TFile;
class TTree;

//
// class declaration
//

class IsolatedNoise : public edm::EDAnalyzer {
 public:
  explicit IsolatedNoise(const edm::ParameterSet&);
  ~IsolatedNoise();
  
  
 private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  // helper function
  void DumpHitCollectionInfo(std::vector<HitCollectionInfo>&) const;

  // parameters
  edm::InputTag hbheLabel_;
  std::vector<edm::InputTag> ecalLabels_;

  // hit validator
  HitValidator hitvalidator_;

  // root file/histograms
  TFile* rootfile_;

};

#endif
