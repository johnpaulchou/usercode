//
// EventWeight.cc
//
//    description: Object to determine how much to weight an event
//                 Determined by assumed luminosity, cross section of the process,
//                 other scale factors, and the number of events run over
//

#include "DataHandling/DataFormats/interface/EventWeight.h"

#include <TMath.h>

EventWeight::EventWeight() :
  integratedLumi_(1000.), integratedLumiError_(0.),
  crossSection_(1.), crossSectionError_(0.),
  scaleFactor_(1.), scaleFactorError_(0.),
  numEvents_(1),
  calcWeight_(false), weightTmp_(0.), weightErrorTmp_(0.)
{
}

EventWeight::EventWeight(float lumi, float lumie, float xs, float xse, float sf, float sfe, int numevents) :
  integratedLumi_(lumi), integratedLumiError_(lumie),
  crossSection_(xs), crossSectionError_(xse),
  scaleFactor_(sf), scaleFactorError_(sfe),
  numEvents_(numevents),
  calcWeight_(false), weightTmp_(0.), weightErrorTmp_(0.)
{
}

EventWeight::~EventWeight()
{
}

float EventWeight::weight(void) const
{
  // if we've already calculated the weight, return it now
  if(calcWeight_) return weightTmp_;

  // we haven't calculated the weight, let's calculate it now
  calcWeight_=true;

  // if the number of events is 0, then we have a problem
  if(numEvents_==0) {
    weightTmp_ = 0.;
    weightErrorTmp_ = 0.;
    return weightTmp_;
  }

  // calculate both the weight and the uncertainty
  weightTmp_ = integratedLumi_*crossSection_*scaleFactor_/numEvents_;
  float lumiE = integratedLumiError_*crossSection_*scaleFactor_/numEvents_;
  float xsE   = integratedLumi_*crossSectionError_*scaleFactor_/numEvents_;
  float sfE   = integratedLumi_*crossSection_*scaleFactorError_/numEvents_;
  weightErrorTmp_ = TMath::Sqrt(lumiE*lumiE + xsE*xsE + sfE*sfE);

  return weightTmp_;
}

float EventWeight::weightError(void) const
{
  // if we've already calculated the weight, return the error now
  if(calcWeight_) return weightErrorTmp_;

  // we haven't calculated the weight
  // use the weight function to calculate the weight, and return the error
  weight();
  return weightErrorTmp_;
}
