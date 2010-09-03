#ifndef __ANALYSIS_MAKESKIMS_MYPATELECTRONSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_MYPATELECTRONSELECTOR_H__

#include "FWCore/Utilities/interface/InputTag.h"
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
    edm::InputTag tracksSrc_;
    edm::InputTag EBRecHitsSrc_;
    std::string isolatedLabel_;
    std::string nonisolatedLabel_;
    std::string looseLabel_;
    bool requireCaloDrivenSeed_;
    double minEt_;
    double minSuperClusterEt_;
    double maxEta_;
    double maxSuperClusterEta_;
    int maxNumLostHits_;
    double maxTransverseIP_;
    double maxDeltaZ_;
    double maxSwissCross_;
    double conversionMinSep_;
    double conversionMinDcot_;
    double maxSihihEB_;
    double maxDphiEB_;
    double maxDetaEB_;
    double maxHoeEB_;
    double maxSihihEE_;
    double maxDphiEE_;
    double maxDetaEE_;
    double maxHoeEE_;
    double maxIsolation_;
    edm::InputTag muonCleaningSrc_;
    std::vector<std::string> muonCleaningIDs_;
    unsigned int muonCleaningMinNumValidHits_;
    double muonCleaningMaxDr_;
  };
}

#endif
