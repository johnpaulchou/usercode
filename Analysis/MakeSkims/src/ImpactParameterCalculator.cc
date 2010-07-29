#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

ImpactParameterCalculator::ImpactParameterCalculator(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::InputTag& VertexSrc)
{
  // transient track builder to determine muon p.v. i.p. sig
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

  // get the primary vertex
  edm::Handle< std::vector<reco::Vertex> > h_vertices;
  iEvent.getByLabel(VertexSrc, h_vertices);
  if(h_vertices.isValid()) {
    // choose the first vertex as this is the one with the highest sum track pT^2
    theVertex = &(*h_vertices->begin());
  } else {
    edm::LogWarning("DataNotFound") << "recoVertexs with tag " << VertexSrc.encode() << " not found in the event.";
    theVertex = 0;
  }
}

std::pair<bool, Measurement1D> ImpactParameterCalculator::absoluteTransverseImpactParameter(const reco::Track& p) const
{
  if(!theVertex) return std::pair<bool, Measurement1D>(false, Measurement1D());
  reco::TransientTrack tt = trackBuilder->build(p);
  return IPTools::absoluteTransverseImpactParameter(tt, *theVertex);
}

std::pair<bool, Measurement1D> ImpactParameterCalculator::absoluteTransverseImpactParameter(const reco::GsfTrack& p) const
{
  if(!theVertex) return std::pair<bool, Measurement1D>(false, Measurement1D());
  reco::TransientTrack tt = trackBuilder->build(p);
  return IPTools::absoluteTransverseImpactParameter(tt, *theVertex);
}

std::pair<bool, Measurement1D> ImpactParameterCalculator::absoluteImpactParameter3D(const reco::Track& p) const
{
  if(!theVertex) return std::pair<bool, Measurement1D>(false, Measurement1D());
  reco::TransientTrack tt = trackBuilder->build(p);
  return IPTools::absoluteImpactParameter3D(tt, *theVertex);
}

std::pair<bool, Measurement1D> ImpactParameterCalculator::absoluteImpactParameter3D(const reco::GsfTrack& p) const
{
  if(!theVertex) return std::pair<bool, Measurement1D>(false, Measurement1D());
  reco::TransientTrack tt = trackBuilder->build(p);
  return IPTools::absoluteImpactParameter3D(tt, *theVertex);
}
