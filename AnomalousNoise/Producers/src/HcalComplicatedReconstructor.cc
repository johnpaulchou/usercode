using namespace std;
#include "AnomalousNoise/Producers/interface/HcalComplicatedReconstructor.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/Common/interface/EDCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Selector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"

#include <iostream>

/*  Hcal Hit reconstructor allows for CaloRecHits with status words */

HcalComplicatedReconstructor::HcalComplicatedReconstructor(edm::ParameterSet const& conf):
  reco_(conf.getParameter<int>("firstSample"),
	conf.getParameter<int>("samplesToAdd"),
	conf.getParameter<bool>("correctForTimeslew"),
	conf.getParameter<bool>("correctForPhaseContainment"),
	conf.getParameter<double>("correctionPhaseNS")),
  inputLabel_(conf.getParameter<edm::InputTag>("digiLabel")),
  correctTiming_(conf.getParameter<bool>("correctTiming")),
  dropZSmarkedPassed_(conf.getParameter<bool>("dropZSmarkedPassed"))
  
{
  produces<HBHERecHitCollection>();
}

HcalComplicatedReconstructor::~HcalComplicatedReconstructor() {
}

void HcalComplicatedReconstructor::produce(edm::Event& e, const edm::EventSetup& eventSetup)
{
  // get conditions
  edm::ESHandle<HcalDbService> conditions;
  eventSetup.get<HcalDbRecord>().get(conditions);
  const HcalQIEShape* shape = conditions->getHcalShape (); // this one is generic
  
  edm::ESHandle<HcalChannelQuality> p;
  eventSetup.get<HcalChannelQualityRcd>().get(p);
  HcalChannelQuality* myqual = new HcalChannelQuality(*p.product());

  edm::ESHandle<HcalSeverityLevelComputer> mycomputer;
  eventSetup.get<HcalSeverityLevelComputerRcd>().get(mycomputer);
  const HcalSeverityLevelComputer* mySeverity = mycomputer.product();
  
  edm::Handle<HBHEDigiCollection> digi;
  e.getByLabel(inputLabel_,digi);
  
  // create empty output
  std::auto_ptr<HBHERecHitCollection> rec(new HBHERecHitCollection);
  rec->reserve(digi->size());
  // run the algorithm
  HBHEDigiCollection::const_iterator i;
  std::vector<HBHEDataFrame> HBDigis;
  std::vector<int> RecHitIndex;
  
  // Vote on majority TS0 CapId
  int favorite_capid = 0; 
  if (correctTiming_) {
    long capid_votes[4] = {0,0,0,0};
    for (i=digi->begin(); i!=digi->end(); i++) {
      capid_votes[(*i)[0].capid()]++;
    }
    for (int k = 0; k < 4; k++)
      if (capid_votes[k] > capid_votes[favorite_capid])
	favorite_capid = k;
  }
  
  for (i=digi->begin(); i!=digi->end(); i++) {
    HcalDetId cell = i->id();
    DetId detcell=(DetId)cell;
    
    // check on cells to be ignored and dropped: (rof,20.Feb.09)
    const HcalChannelStatus* mydigistatus=myqual->getValues(detcell.rawId());
    if (mySeverity->dropChannel(mydigistatus->getValue() ) ) continue;
    if (dropZSmarkedPassed_)
      if (i->zsMarkAndPass()) continue;
    
    const HcalCalibrations& calibrations=conditions->getHcalCalibrations(cell);
    const HcalQIECoder* channelCoder = conditions->getHcalCoder (cell);
    HcalCoderDb coder (*channelCoder, *shape);
    rec->push_back(reco_.reconstruct(*i,coder,calibrations));
    (rec->back()).setFlags(0);
  } // loop over HBHE digis
  // return result
  e.put(rec);

  delete myqual;
} // void HcalComplicatedReconstructor::produce(...)
