#ifndef __ANALYSIS_MAKESKIMS_MYPATMUONSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_MYPATMUONSELECTOR_H__

#include "FWCore/Framework/interface/EDFilter.h"

#include <string>

namespace pat {

  class MyPATMuonSelector : public edm::EDFilter {
  public:
    MyPATMuonSelector(edm::ParameterSet const& params);
    virtual ~MyPATMuonSelector() {}
    virtual void beginJob() {}
    virtual void endJob() {}

    virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected:
    edm::InputTag src_;
    edm::InputTag vertexSrc_;
    std::string isolatedLabel_;
    std::string nonisolatedLabel_;
    std::string muonID_;
    double minPt_;
    double maxEta_;
    unsigned int minNumValidHits_;
    double maxNormChi2_;
    double maxIsolation_;
    double maxTransverseIP_;
  };
}

#endif
