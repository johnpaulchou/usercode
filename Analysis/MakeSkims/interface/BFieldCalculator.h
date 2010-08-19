#ifndef __ANALYSIS_MAKESKIMS_BFIELDCALCULATOR_H__
#define __ANALYSIS_MAKESKIMS_BFIELDCALCULATOR_H__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

class BFieldCalculator
{
 public:
  static double calculate(const edm::Event& iEvent, const edm::EventSetup& iSetup);			  
  
 private:
  BFieldCalculator() {}
  virtual ~BFieldCalculator() {}

};

#endif
