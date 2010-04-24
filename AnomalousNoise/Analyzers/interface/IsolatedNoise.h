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
  void DumpHBHEHitMap(std::vector<HBHEHitMap>&) const;

  // parameters
  edm::InputTag hbheLabel_;
  std::vector<edm::InputTag> ecalLabels_;
  edm::InputTag trackLabel_;
  std::string rootHistFileName_;
  
  // object validator
  ObjectValidator objvalidator_;

  // tracking stuff
  TrackDetectorAssociator trackAssociator_;
  TrackAssociatorParameters trackParameters_;


  // root file/histograms
  TFile* rootfile_;
  TTree* tree_;

  int foundnoise_;
  float noisee_;
  int nnoisehits_;
  int noisetype_;

  int nrbxs_;
  int rbxnhits_[72];
  float rbxe_[72];
  float rbxtrkfide_[72];
  int rbxisolnhcalhits_[72], rbxisolnecalhits_[72], rbxisolntrks_[72];
  float rbxisolhcale_[72], rbxisolecale_[72], rbxisoltrke_[72];

  int nhpds_;
  int hpdnhits_[288];
  float hpde_[288];
  float hpdtrkfide_[288];
  int hpdisolnhcalhits_[288], hpdisolnecalhits_[288], hpdisolntrks_[288];
  float hpdisolhcale_[288], hpdisolecale_[288], hpdisoltrke_[288];

  int ndis_;
  int dinhits_[1000];
  float die_[1000];
  float ditrkfide_[1000];
  int diisolnhcalhits_[1000], diisolnecalhits_[1000], diisolntrks_[1000];
  float diisolhcale_[1000], diisolecale_[1000], diisoltrke_[1000];

  int nmonos_;
  int mononhits_[1000];
  float monoe_[1000];
  float monotrkfide_[1000];
  int monoisolnhcalhits_[1000], monoisolnecalhits_[1000], monoisolntrks_[1000];
  float monoisolhcale_[1000], monoisolecale_[1000], monoisoltrke_[1000];


};

#endif
