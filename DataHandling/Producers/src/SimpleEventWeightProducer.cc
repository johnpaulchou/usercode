// system include files
#include <memory>

// user include files

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataHandling/Producers/interface/SimpleEventWeightProducer.h"
#include "DataHandling/DataFormats/interface/EventWeight.h"

//
// constructors and destructor
//

SimpleEventWeightProducer::SimpleEventWeightProducer(const edm::ParameterSet& iConfig)
{
  integratedLumi_      = iConfig.getParameter<double>("integratedLumi");
  integratedLumiError_ = iConfig.getParameter<double>("integratedLumiError");
  crossSection_        = iConfig.getParameter<double>("crossSection");
  crossSectionError_   = iConfig.getParameter<double>("crossSectionError");
  scaleFactor_         = iConfig.getParameter<double>("scaleFactor");
  scaleFactorError_    = iConfig.getParameter<double>("scaleFactorError");
  numEvents_           = iConfig.getParameter<int>("numEvents");

  produces<EventWeight>();
}


SimpleEventWeightProducer::~SimpleEventWeightProducer()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SimpleEventWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // create the EventWeight we're going to write
  EventWeight *ew = new EventWeight(static_cast<float>(integratedLumi_), static_cast<float>(integratedLumiError_),
				    static_cast<float>(crossSection_), static_cast<float>(crossSectionError_),
				    static_cast<float>(scaleFactor_), static_cast<float>(scaleFactorError_),
				    numEvents_);
  

  // write the EventWeight to the edm
  std::auto_ptr<EventWeight> result(ew);
  iEvent.put(result);

  return;
}

void
SimpleEventWeightProducer::beginJob(const edm::EventSetup&)
{
}

void
SimpleEventWeightProducer::endJob()
{
}


//define this as a plug-in
DEFINE_FWK_MODULE(SimpleEventWeightProducer);
