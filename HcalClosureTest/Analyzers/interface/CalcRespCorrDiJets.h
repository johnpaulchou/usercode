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

#include "HcalClosureTest/DataFormat/interface/DijetRespCorrData.h"

// forward declarations
class TH1D;
class TH2D;
class TFile;

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
  std::string rootHistFilename_; // name of the histogram file
  double maxDeltaEta_;           // maximum delta-|Eta| between Jets
  double minDeltaPhi_;           // minimum delta-Phi between Jets
  double minTagJetEta_;          // minimum |eta| of the tag jet
  double minJetEt_;              // minimum Jet Et
  double maxThirdJetEt_;         // maximum 3rd jet Et
  double maxJetEMF_;             // maximum EMF of the tag and probe jets

  // root file/histograms
  TFile* rootfile_;

  // response correction stuff
  DijetRespCorrData respcorrdata_;

  TH1D* hPassSel_;
  TH1D* hTagEta_;
  TH1D* hProbeEta_;
  TH2D* hRatioTagEta_;
};


#endif
