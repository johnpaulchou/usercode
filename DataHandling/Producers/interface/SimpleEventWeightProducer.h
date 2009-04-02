#ifndef __SIMPLE_EVENT_WEIGHT_PRODUCER_H__
#define __SIMPLE_EVENT_WEIGHT_PRODUCER_H__

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include <vector>

//
// class declaration
//

class SimpleEventWeightProducer : public edm::EDProducer
{
 public:
  explicit SimpleEventWeightProducer(const edm::ParameterSet&);
  ~SimpleEventWeightProducer();
  
  
 private:

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ---------- parameters ---------------------------

  double integratedLumi_;
  double integratedLumiError_;
  double crossSection_;
  double crossSectionError_;
  double scaleFactor_;
  double scaleFactorError_;
  int numEvents_;  

  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//


#endif
