#ifndef __ANALYSIS_MAKESKIMS_MYPATMUONSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_MYPATMUONSELECTOR_H__

#include "FWCore/Utilities/interface/InputTag.h"
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
    double minPt_;
    double maxEta_;
  };
}

#endif
