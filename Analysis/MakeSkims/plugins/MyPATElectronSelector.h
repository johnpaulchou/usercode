#ifndef __ANALYSIS_MAKESKIMS_MYPATELECTRONSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_MYPATELECTRONSELECTOR_H__

#include "FWCore/Framework/interface/EDFilter.h"

namespace pat {

  class MyPATElectronSelector : public edm::EDFilter {
  public:
    MyPATElectronSelector(edm::ParameterSet const& params);
    virtual ~MyPATElectronSelector() {}
    virtual void beginJob() {}
    virtual void endJob() {}
    
    virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected:
    edm::InputTag src_;
    edm::InputTag vertexSrc_;
    std::string isolatedLabel_;
    std::string nonisolatedLabel_;
    std::string electronID_;
    double minEt_;
    double maxEta_;
    int maxNumLostHits_;
    double maxTransverseIP_;
    double maxIsolation_;
    edm::InputTag muonCleaningSrc_;
    std::vector<std::string> muonCleaningIDs_;
    unsigned int muonCleaningMinNumValidHits_;
    double muonCleaningMaxDr_;
  };
}

#endif
