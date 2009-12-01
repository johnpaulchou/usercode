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
#include <map>

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

  // event counting stuff
  TH1D* hEventCount_;
  std::map<int, int> lumiCountMap_;
  TH1D* hLumiBlockCount_;

  // tree
  TTree* tree_;
  int nrbxs_;
  int rbxnhits_[100], rbxnhpds_[100], rbxidnum_[100], rbxnadc0s_[100];
  int rbxnhitshpd0_[100], rbxnhitshpd1_[100], rbxnhitshpd2_[100], rbxnhitshpd3_[100];
  float rbxenergy_[100], rbxcalohade_[100], rbxcaloeme_[100], rbxratio_[100];
  float rbx0ts_[100], rbx1ts_[100], rbx2ts_[100], rbx3ts_[100], rbx4ts_[100], rbx5ts_[100], rbx6ts_[100], rbx7ts_[100], rbx8ts_[100], rbx9ts_[100];
  
  int nhits_;
  int hitieta_[100], hitiphi_[100];
  float hitenergy_[100], hittime_[100];

  int ntracks_;
  float trkpt_[1000], trkp_[1000], trketa_[1000], trkphi_[1000];
  float sumtrkp_;

  int njets_;
  float jetpt_[100], jeteta_[100], jetphi_[100], jetHBhade_[100], jetHEhade_[100], jetEBeme_[100], jetEEeme_[100];

  float hade_, eme_;
  float met_;

};


#endif
