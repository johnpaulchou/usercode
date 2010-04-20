#ifndef HCALCOMPLICATEDRECONSTRUCTOR_H 
#define HCALCOMPLICATEDRECONSTRUCTOR_H 1

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnomalousNoise/Algorithms/interface/HcalComplicatedRecAlgo.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/HcalObjects/interface/HcalChannelStatus.h"

    /** \class HcalComplicatedReconstructor
	
    $Date: 2009/08/29 18:38:53 $
    $Revision: 1.6 $
    \author J. Temple & E. Yazgan
    ** Based on HcalSimpleReconstructor.h by J. Mans
    */
    class HcalComplicatedReconstructor : public edm::EDProducer {
    public:
      explicit HcalComplicatedReconstructor(const edm::ParameterSet& ps);
      virtual ~HcalComplicatedReconstructor();
      virtual void produce(edm::Event& e, const edm::EventSetup& c);
    private:      
      HcalComplicatedRecAlgo reco_;
      edm::InputTag inputLabel_;
      //std::vector<std::string> channelStatusToDrop_;
      bool correctTiming_; // turn on/off Ken Rossato's algorithm to fix timing
      bool dropZSmarkedPassed_; // turn on/off dropping of zero suppression marked and passed digis
    };

#endif
