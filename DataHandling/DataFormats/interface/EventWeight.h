#ifndef __EVENT_WEIGHT_H__
#define __EVENT_WEIGHT_H__

//
// EventWeight.h
//
//    description: Object to determine how much to weight an event
//                 Determined by assumed luminosity, cross section of the process,
//                 and the number of events run over
//                 Errors shown are all _absolute_
//

class EventWeight
{
 public:
  EventWeight();
  EventWeight(float lumi, float lumie, float xs, float xse, int numevents);
  ~EventWeight();

  inline float integratedLumi(void) const { return integratedLumi_; }
  inline float integratedLumiError(void) const { return integratedLumiError_; }
  inline float crossSection(void) const { return crossSection_; }
  inline float crossSectionError(void) const { return crossSectionError_; }
  inline int numEvents(void) const { return numEvents_; }

  float weight(void) const;
  float weightError(void) const;

 private:
  float integratedLumi_;
  float integratedLumiError_;
  float crossSection_;
  float crossSectionError_;
  int numEvents_;

  // these are transient types
  // calculated once, they make recall faster
  mutable bool calcWeight_;
  mutable float weightTmp_;
  mutable float weightErrorTmp_;

};



#endif
