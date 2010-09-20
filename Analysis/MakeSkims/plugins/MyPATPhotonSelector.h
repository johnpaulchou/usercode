#ifndef __ANALYSIS_MAKESKIMS_MYPATPHOTONSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_MYPATPHOTONSELECTOR_H__

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDFilter.h"

namespace pat {
  
  class MyPATPhotonSelector : public edm::EDFilter {
  public:
    MyPATPhotonSelector(edm::ParameterSet const& params);
    virtual ~MyPATPhotonSelector() {}
    virtual void beginJob() {}
    virtual void endJob() {}
    
    virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected:
    edm::InputTag src_;
    edm::InputTag vertexSrc_;
    edm::InputTag EBRecHitsSrc_;
    bool vetoPixelSeed_;
    double minEt_, maxEta_;
    double maxSwissCross_;
    edm::InputTag muonCleaningSrc_;
    std::vector<std::string> muonCleaningIDs_;
    unsigned int muonCleaningMinNumValidHits_;
    double muonCleaningMaxDr_;
    bool embedConversions_;
    
  };
}


#endif
