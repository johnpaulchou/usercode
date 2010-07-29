#ifndef __ANALYSIS_MAKESKIMS_IMPACTPARAMETERCALCULATOR_H__
#define __ANALYSIS_MAKESKIMS_IMPACTPARAMETERCALCULATOR_H__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

class ImpactParameterCalculator
{
 public:
  ImpactParameterCalculator(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::InputTag& VertexSrc);
  virtual ~ImpactParameterCalculator() {}

  std::pair<bool, Measurement1D> absoluteTransverseImpactParameter(const reco::Track& p) const;
  std::pair<bool, Measurement1D> absoluteTransverseImpactParameter(const reco::GsfTrack& p) const;

  std::pair<bool, Measurement1D> absoluteImpactParameter3D(const reco::Track& p) const;
  std::pair<bool, Measurement1D> absoluteImpactParameter3D(const reco::GsfTrack& p) const;

 protected:
  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  const reco::Vertex* theVertex;

};

#endif
